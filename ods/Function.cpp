#include "Function.hpp"

#include "Reference.hpp"
#include "CellRef.hpp"
#include "err.hpp"
#include "Formula.hpp"
#include "FormulaNode.hpp"
#include "formula.hxx"
#include "function.hh"
#include "ods.hh"
#include "Row.hpp"
#include "Sheet.hpp"

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
Function::AddArg(ods::Reference *a) {
	AddArg(ods::FormulaNode::Reference(a));
}

void Function::AddArg(ods::Cell *cell) {
	ods::Sheet *sheet = cell->row()->sheet();
	auto *ref = sheet->NewReference(cell);
	AddArg(ods::FormulaNode::Reference(ref));
}

void Function::AddArgRange(ods::Cell *from, ods::Cell *till) {
	ods::Sheet *sheet = from->row()->sheet();
	auto *ref = sheet->NewReference(from, till);
	AddArg(ods::FormulaNode::Reference(ref));
}

void
Function::AddArg(double d) {
	AddArg(ods::FormulaNode::Double(d));
}

void
Function::AddArg(ods::Op op) {
	AddArg(ods::FormulaNode::Op(op));
}

ods::Function*
Function::AddArg(const ods::FunctionId id) {
	auto *fn = ods::Function::New(id);
	fn->parent_formula_ = parent_formula_;
	AddArg(ods::FormulaNode::Function(fn));
	return fn;
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
Function::AddArg(QString s) {
	AddArg(ods::FormulaNode::String(new QString(s)));
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
	
	if (DeepCopy(*p, *this))
		return p;
	
	mtl_trace();
	return p;
}

bool
Function::DeepCopy(ods::Function &dest, const ods::Function &src)
{
	dest.meta_ = src.meta_;
	dest.parent_formula_ = src.parent_formula_;
	dest.args_->clear();
	
	if(src.args_ == nullptr) {
		return false;
	}
	
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
	
	return true;
}

ods::Sheet*
Function::default_sheet() const { return parent_formula_->default_sheet(); }

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
		MTL_CHECK_NULL(eval::ReplaceNamedRanges(*subvec));
		while (subvec->size() > 1) {
			MTL_CHECK_NULL(eval::EvalDeepestGroup(*subvec));
		}
		
		MTL_CHECK_NULL((subvec->size() == 1));
		FormulaNode *node = (*subvec)[0];
		fn_args.append(node);
		subvec->clear();
	}

	if (meta_->settings & ods::FlattenOutParamsBit) {
		MTL_CHECK_NULL(eval::FlattenOutArgs(fn_args));
	}
	
	MTL_CHECK_NULL(meta_);
	
#ifdef DEBUG_FORMULA_EVAL
	mtl_info("%sSending %s() to execution%s", MTL_COLOR_YELLOW, meta_->name, MTL_COLOR_DEFAULT);
#endif
	
	
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
	case FunctionId::SumIf: return function::SumIf(fn_args, default_sheet());
	case FunctionId::Concatenate: return function::Concatenate(fn_args);
	case FunctionId::Date: return function::Date(fn_args);
	case FunctionId::Now: return function::Now();
	case FunctionId::Quotient: return function::Quotient(fn_args);
	case FunctionId::Mod: return function::Mod(fn_args);
	case FunctionId::Power: return function::Power(fn_args);
	case FunctionId::If: return function::If(fn_args);
	case FunctionId::Count: return function::Count(fn_args);
	case FunctionId::CountA: return function::CountA(fn_args);
	case FunctionId::CountBlank: return function::CountBlank(fn_args);
	case FunctionId::CountIf: return function::CountIf(fn_args, default_sheet());
	case FunctionId::Average: return function::Average(fn_args);
	case FunctionId::Today: return function::Today();
	case FunctionId::Round: return function::RoundAnyWay(fn_args, RoundType::Round);
	case FunctionId::RoundDown: return function::RoundAnyWay(fn_args, RoundType::Floor);
	case FunctionId::RoundUp: return function::RoundAnyWay(fn_args, RoundType::Ceil);
	case FunctionId::Day: return function::DayMonthYear(fn_args, DMY::Day);
	case FunctionId::Month: return function::DayMonthYear(fn_args, DMY::Month);
	case FunctionId::Year: return function::DayMonthYear(fn_args, DMY::Year);
	case FunctionId::And: return function::And(fn_args);
	case FunctionId::Or: return function::Or(fn_args);
	case FunctionId::Columns: return function::ColumnsRows(fn_args, ods::ColsOrRows::Columns);
	case FunctionId::Rows: return function::ColumnsRows(fn_args, ods::ColsOrRows::Rows);
	case FunctionId::True: return function::Bool(true);
	case FunctionId::False: return function::Bool(false);
	case FunctionId::Not: return function::Not(fn_args);
	case FunctionId::Indirect: return function::Indirect(fn_args, parent_formula_);
	case FunctionId::Offset: return function::Offset(fn_args);
	case FunctionId::Text: return function::Text(fn_args);
	case FunctionId::Match: return function::Match(fn_args);
	case FunctionId::Sin: return function::Sin(fn_args);
	case FunctionId::Cos: return function::Cos(fn_args);
	case FunctionId::Tan: return function::Tan(fn_args);
	case FunctionId::Cot: return function::Cot(fn_args);
	default: { mtl_trace();	return nullptr; }
	}
}

Function*
Function::New(const FunctionId id)
{
	const FunctionMeta *func_meta = eval::FindFunctionMeta(id);
	MTL_CHECK_NULL(func_meta);
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
	MTL_CHECK_VOID(args_);
	QString separator = QString(ods::color_green()) + "|" + ods::color_default();
	
	for (int i = 0; i < args_->size(); i++) {
		QVector<FormulaNode*> *subvec = (*args_)[i];
		QString s = separator;
		for (FormulaNode* node : *subvec) {
			s.append(node->toString()).append(separator);
		}
		auto ba = s.toLocal8Bit();
		mtl_info("%d/%d %s", i, static_cast<int>(args_->size()) - 1, ba.data());
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
Function::TryNew(QStringView s, int &skip, ods::Sheet *default_sheet)
{
	int end_of_func_name = ods::FindNonAscii(s);
	
	if (end_of_func_name == -1) // not found, error!
		return nullptr;

	const QString name = s.mid(0, end_of_func_name).toString().toUpper();
	const FunctionMeta *func_meta = eval::FindFunctionMeta(name);
	
	if (func_meta == nullptr)
		return nullptr; // not a function
#ifdef DEBUG_FORMULA_PARSING
	mtl_info("Found function by name: \"%s\"", func_meta->name);
#endif
	
	// now decode the params.
	QStringView params = s.mid(end_of_func_name);
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
	
	while (Formula::ParseNext(s, chunk, *new_vec, default_sheet, settings))
	{
#ifdef DEBUG_FORMULA_PARSING
		mtl_info("ParseNext()");
#endif
		params_total += chunk;
		s = s.mid(chunk);
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
