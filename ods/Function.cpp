#include "Function.hpp"

#include "Address.hpp"
#include "CellRef.hpp"
#include "err.hpp"
#include "Formula.hpp"
#include "FormulaNode.hpp"
#include "function.hh"
#include "ods.hh"

#include <QRegularExpression>
#include <QRegularExpressionMatch>

namespace ods {

Function::Function() {
	args_ = new QVector<QVector<FormulaNode*>*>();
}

Function::~Function() {
	if (args_ != nullptr) {
		for (QVector<FormulaNode*> *subvec: *args_) {
			for (auto k: *subvec) {
				delete k;
			}
			delete subvec;
		}
		delete args_;
		args_ = nullptr;
	}
}

Function::Function(const Function &src)
{
	DeepCopy(*this, src);
}

void
Function::AddArg(FormulaNode *node)
{
	auto *v = new QVector<FormulaNode*>();
	v->append(node);
	args_->append(v);
}

void
Function::AddArg(ods::Address *a) {
	AddArg(ods::FormulaNode::Address(a));
}

void
Function::AddArg(double d) {
	AddArg(ods::FormulaNode::Double(d));
}

void
Function::AddArg(Function *f) {
	AddArg(ods::FormulaNode::Function(f));
}

void
Function::AddArg(ods::Brace b) {
	AddArg(ods::FormulaNode::Brace(b));
}

void
Function::AddArg(ods::Currency *c) {
	AddArg(ods::FormulaNode::Currency(c));
}

void
Function::AddArg(QString *s) {
	AddArg(ods::FormulaNode::String(s));
}

void
Function::AddArg(QVector<FormulaNode*> *subvec)
{
	args_->append(subvec);
}

Function*
Function::Clone()
{
	auto *p = new Function();
	DeepCopy(*p, *this);
	return p;
}

void
Function::DeepCopy(ods::Function &dest, const ods::Function &src)
{
	dest.meta_ = src.meta_;
	dest.args_->clear();
	
	for (QVector<ods::FormulaNode*> *subvec: *src.args_) {
		if (subvec->isEmpty())
			continue;
		
		auto *new_vec = new QVector<ods::FormulaNode*>();
		for (auto *item: *subvec) {
			auto *cloned = item->Clone();
			new_vec->append(cloned);
		}
		dest.args_->append(new_vec);
	}
}

FormulaNode*
Function::Eval()
{
	QVector<ods::FormulaNode*> fn_args;
	
	if (args_->isEmpty()) { // function takes no params
		return ExecOpenFormulaFunction(fn_args);
	}
	
	ods::AutoDeleteVec ad(fn_args);
	
	for (int i = 0; i < args_->size(); i++) {
		QVector<FormulaNode*> *subvec = (*args_)[i];
		
		while (subvec->size() > 1) {
			CHECK_TRUE_NULL(function::EvalDeepestGroup(*subvec));
		}
		CHECK_TRUE_NULL((subvec->size() == 1));
		FormulaNode *node = (*subvec)[0];
		fn_args.append(node);
		subvec->clear();
	}
#ifdef DEBUG_FORMULA_EVAL
	mtl_info("%sInside %s()%s", MTL_COLOR_YELLOW, meta_->name, MTL_COLOR_DEFAULT);
#endif
	function::FlattenOutArgs(fn_args);
	CHECK_PTR_NULL(meta_);
	
	return ExecOpenFormulaFunction(fn_args);
}

FormulaNode*
Function::ExecOpenFormulaFunction(QVector<ods::FormulaNode*> &fn_args)
{
	switch (meta_->id) {
	case FunctionId::Sum: return function::Sum(fn_args);
	case FunctionId::Product: return function::Product(fn_args);
	case FunctionId::Max: return function::Max(fn_args);
	case FunctionId::Min: return function::Min(fn_args);
	case FunctionId::Concatenate: return function::Concatenate(fn_args);
	case FunctionId::Date: return function::Date(fn_args);
	case FunctionId::Now: return function::Now();
	case FunctionId::Quotient: return function::Quotient(fn_args);
	case FunctionId::Mod: return function::Mod(fn_args);
	case FunctionId::Power: return function::Power(fn_args);
	default: { mtl_trace();	return nullptr; }
	}
}

Function*
Function::New(const FunctionId id)
{
	const FunctionMeta *func_meta = function::FindFunctionMeta(id);
	CHECK_PTR_NULL(func_meta);
	Function *f = new Function();
	f->meta_ = func_meta;
	return f;
}

void
Function::PrintArgs(const QString &msg) const
{
	if (msg.size() > 0) {
		auto ba = msg.toLocal8Bit();
		mtl_info("%s", ba.data());
	} else {
		mtl_info("%s() Args:", meta_->name);
	}
	CHECK_PTR_VOID(args_);
	QString separator = QString(MTL_COLOR_GREEN) + "|" + MTL_COLOR_DEFAULT;
	
	for (int i = 0; i < args_->size(); i++) {
		QVector<FormulaNode*> *subvec = (*args_)[i];
		QString s = separator;
		for (FormulaNode* node : *subvec) {
			s.append(node->toString()).append(separator);
		}
		auto ba = s.toLocal8Bit();
		mtl_info("%d/%d %s", i, args_->size() - 1, ba.data());
	}
	
}

QString
Function::toString() const {
	if (meta_ == nullptr) {
		mtl_trace();
		return QString();
	}
	
	QString s = meta_->name;
	
	QString func = (args_ == nullptr) ? QString() :
		QString::number(args_->size());
	s += " {" + func + '}';
	
	return s;
}

QString
Function::toXmlString() const {
	QString s = meta_->name;
	s.append('(');
	const int count = args_->size();
	
	for (int i = 0; i < count; i++) {
		QVector<ods::FormulaNode*>* vec = (*args_)[i];
		QString expression;
		for (ods::FormulaNode *node: *vec) {
			expression.append(node->toString(ods::ToStringArgs::IncludeQuotMarks));
		}
		
		if (i < count - 1) {
			expression.append(';');
		}
		
		s.append(expression);
	}
	
	s.append(')');
	return s;
}

Function*
Function::TryNew(QStringRef s, int &skip, ods::Sheet *default_sheet)
{
	int end_of_func_name = ods::FindNonAscii(s);
	
	if (end_of_func_name == -1) // not found, error!
		return nullptr;

	const QString name = s.mid(0, end_of_func_name).toString().toUpper();
	const FunctionMeta *func_meta = function::FindFunctionMeta(name);
	
	if (func_meta == nullptr)
		return nullptr; // not a function
#ifdef DEBUG_FORMULA_PARSING
	mtl_info("Found function by name: \"%s\"", func_meta->name);
#endif
	
	// now decode the params.
	QStringRef params = s.mid(end_of_func_name);
	int parenthesis_loc = params.indexOf('(');
	
	if (parenthesis_loc == -1)
		return nullptr;
	
	int past_whitespaces = ods::FindNonWhitespace(params, parenthesis_loc + 1);
	
	if (past_whitespaces == -1) {
		mtl_trace();
		return nullptr;
	}
	
	s = params.mid(past_whitespaces);
	QVector<QVector<ods::FormulaNode*>*> *param_nodes
		= new QVector<QVector<ods::FormulaNode*>*>();
	int params_total = 0, chunk = 0;
	u8 settings = ParsingFunctionParams;
	auto *new_vec = new QVector<FormulaNode*>();
	
	while (Formula::ParseNext(s, chunk, *new_vec,
		default_sheet, settings))
	{
#ifdef DEBUG_FORMULA_PARSING
		mtl_info("ParseNext()");
#endif
		s = s.mid(chunk);
		params_total += chunk;
		chunk = 0;

		if (settings & ReachedFunctionEnd) {
#ifdef DEBUG_FORMULA_PARSING
			mtl_info("End of function %s()", func_meta->name);
#endif
			break;
		}
		
		if (settings & ReachedParamSeparator) {
			param_nodes->append(new_vec);
			new_vec = new QVector<FormulaNode*>();
			settings &= ~ReachedParamSeparator;
		}
	}
//"SUM([.B1]+0.3;20.9;-2.4+3*MAX(18;7);[.B2];[.C1:.C2];MIN([.A1];5))*(-3+2)"
//"SUM([.B1]+0.3;20.9;-2.4+3*MAX(18;7);[.B2];[.C1:.C2];MIN([.A1];5))*(-3+2)"
	if (!(settings & ReachedFunctionEnd) && param_nodes != nullptr) {
		mtl_trace();
		for (QVector<FormulaNode*> *next: *param_nodes) {
			for (auto k : *next) {
				delete k;
			}
			delete next;
		}
		return nullptr;
	} else {
		settings &= ~ReachedFunctionEnd;
	}
	
	if (new_vec->isEmpty()) {
		delete new_vec;
		new_vec = nullptr;
	} else {
		param_nodes->append(new_vec);
	}
	
	skip += end_of_func_name + past_whitespaces + params_total;
	Function *f = new Function();
	f->meta_ = func_meta;
	f->args_ = param_nodes;
	return f;
}

} // ods::function::
