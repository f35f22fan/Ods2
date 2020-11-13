#include "Function.hpp"

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
	params_ = new QVector<QVector<FormulaNode*>*>();
}

Function::~Function() {
	
	if (params_ != nullptr) {
		for (QVector<FormulaNode*> *next: *params_) {
			for (auto k : *next) {
				delete k;
			}
			delete next;
		}
		delete params_;
		params_ = nullptr;
	}
	
	delete value_;
	value_ = nullptr;
}
Function::Function(const Function &src)
{
	DeepCopy(*this, src);
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
	dest.params_->clear();
	
	for (QVector<ods::FormulaNode*> *subvec: *src.params_) {
		auto *v = new QVector<ods::FormulaNode*>();
		for (auto next: *subvec) {
			v->append(next->Clone());
		}
		dest.params_->append(v);
	}
}

bool
Function::Eval()
{
	value_.Clear();
	QVector<ods::Value*> values;
	ods::AutoDeleteVec ad(values);
	
	for (int i = 0; i < params_->size(); i++) {
		QVector<FormulaNode*> *subvec = (*params_)[i];
		while (subvec->size() > 1) {
			CHECK_TRUE(function::EvalDeepestGroup(*subvec));
		}
	}
	
	CHECK_PTR(meta_);
	function::PrintValuesInOneLine(values);
	switch (meta_->id) {
	case FunctionId::Sum: return function::Sum(values, value_);
	case FunctionId::Max: return function::Max(values, value_);
	case FunctionId::Min: return function::Min(values, value_);
	default: { mtl_trace();	return false; }
	}
}

QString
Function::toString() const {
	if (meta_ == nullptr) {
		mtl_trace();
		return QString();
	}
	
	QString s = meta_->name;
	
	QString func = (params_ == nullptr) ? QString() :
		QString::number(params_->size());
	s += " {" + func + '}';
	
	return s;
}

QString
Function::toXmlString() const {
	QString s = meta_->name;
	s.append('(');
	const int count = params_->size();
	
	for (int i = 0; i < count; i++) {
		QVector<ods::FormulaNode*>* vec = (*params_)[i];
		QString expression;
		for (ods::FormulaNode *node: *vec) {
			expression.append(node->toString());
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
#ifdef DEBUG_FORMULA
	mtl_line("Found function by name: \"%s\"", func_meta->name);
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
	param_nodes->append(new_vec);
	
	while (Formula::DecodeNext(s, chunk, *new_vec,
		default_sheet, settings))
	{
#ifdef DEBUG_FORMULA
		mtl_line("DecodeNext()");
#endif
		s = s.mid(chunk);
		params_total += chunk;
		chunk = 0;

		if (settings & ReachedFunctionEnd) {
#ifdef DEBUG_FORMULA
			mtl_line("End of function %s()", func_meta->name);
#endif
			break;
		}
		
		if (settings & ReachedParamSeparator) {
			new_vec = new QVector<FormulaNode*>();
			param_nodes->append(new_vec);
			settings &= ~ReachedParamSeparator;
		}
	}
//"SUM([.B1]+0.3;20.9;-2.4+3*MAX(18;7);[.B2];[.C1:.C2];MIN([.A1];5))*(-3+2)"
//"SUM([.B1]+0.3;20.9;-2.4+3*MAX(18;7);[.B2];[.C1:.C2];MIN([.A1];5))*(-3+2)"
	if (!(settings & ReachedFunctionEnd) && param_nodes != nullptr) {
mtl_line();

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
	
	skip += end_of_func_name + past_whitespaces + params_total;
	Function *f = new Function();
	f->meta_ = func_meta;
	f->params_ = param_nodes;
	return f;
}

} // ods::function::
