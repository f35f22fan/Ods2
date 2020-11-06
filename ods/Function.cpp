#include "Function.hpp"

#include "CellRef.hpp"
#include "err.hpp"
#include "Formula.hpp"
#include "FormulaNode.hpp"
#include "ods.hh"

#include <QRegularExpression>
#include <QRegularExpressionMatch>

namespace ods {

Function::Function() {}
Function::~Function() {}
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
	dest.params_.clear();
	
	for (auto &item: src.params_)
		dest.params_.append(item);
}

const ods::Value&
Function::Eval()
{
	if (meta_ == nullptr) {
		mtl_trace();
		return value_;
	}
	
	const auto id = meta_->id;
	
	switch (id) {
	case FunctionId::Sum: Sum(); break;
		
	default: {
		mtl_trace();
	}
	}
	
	return value_;
}

void
Function::Sum()
{
	double d = 0.0;
	
	for (int i = 0; i < params_.size(); i++) {
		const FormulaNode &node = params_[i];
		
		if (node.is_address()) {
			ods::Address *a = node.as_address();
			if (a->cell_range()) {
				mtl_warn("Cell range not implemented yet");
				return;
			} else {
				CellRef *cell_ref = a->cell();
				ods::Cell *cell = cell_ref->GetCell();
				
				if (cell == nullptr) {
					mtl_trace();
					return;
				}
				
				if (cell->is_any_double()) {
					d += *cell->as_double();
				} else {
					mtl_trace("Cell value isn't a double");
					return;
				}
			}
		}
	}
	
	value_.SetDouble(d);
}

QString
Function::toString() const {
	if (meta_ == nullptr) {
		mtl_trace();
		return QString();
	}
	
	QString s = meta_->name;
	s += " {" + QString::number(params_.size()) + '}';
	return s;
}

int
Function::TryNew(QStringRef s, Formula *host, Function &retval)
{
	QStringRef original_str = s;
	int end_of_func_name = ods::FindNonAscii(s);
	
	if (end_of_func_name == -1) // not found, error!
		return -1;
	
	const QString name = s.mid(0, end_of_func_name).toString().toUpper();
	const FunctionMeta *fi = FindFunctionMeta(name);
	
	if (fi == nullptr)
		return -1; // not a function
	
	retval.meta_ = fi;
	mtl_line("Found function by name: \"%s\"", fi->name);
	
	// now decode the params.
	QStringRef params = s.mid(end_of_func_name);
	int parenthesis_loc = params.indexOf('(');
	
	if (parenthesis_loc == -1)
		return -1;
	
	int at2 = ods::FindNonWhitespace(params, parenthesis_loc + 1);
	
	if (at2 == -1) {
		mtl_trace();
		return -1;
	}
	
	s = params.mid(at2);
	auto ba = s.toLocal8Bit();
	mtl_line("Function params: \"%s\"", ba.data());
	int at3 = 0;
	
	static const QRegularExpression number_regex =
		QRegularExpression("\\d+(\\.\\d+)?");
	QRegularExpressionMatch number_match;
	ods::Function next_function;
	int next_func_skip;
	
	while (at3 < s.size()) {
		QStringRef next = s.mid(at3);
		if (next.startsWith(')')) {
			at3++;
			const int total = end_of_func_name + at2 + at3;
			auto ba = original_str.mid(0, total).toLocal8Bit();
			mtl_line("RETURNING from FUNC \"%s\" (length: %d)", ba.data(), total);
			return total;
		} else if (next.startsWith('[')) {
			Address *pa = Formula::CellAddressOrRange(next.mid(1),
				at3, host->default_sheet_);
			if (pa != nullptr) {
				auto pm = FormulaNode::Address(pa);
				retval.params_.append(pm);
				at3++; // accounting for '['
				auto addr_ba = pm.as_address()->toString().toLocal8Bit();
				mtl_line("Address: \"%s\"", addr_ba.data());
			} else {
				mtl_trace();
				return -1;
			}
		} else if (next.startsWith(';')) {
			// param separator, skip and continue
			mtl_line("Separator ;");
			at3++;
		} else if ((next.toString().indexOf(number_regex, 0, &number_match)) == 0) {
			QString number_str = number_match.captured(0);
			bool ok;
			double d = number_str.toDouble(&ok);
			
			if (!ok) {
				mtl_trace();
				return -1;
			}
			
			retval.params_.append(FormulaNode::Number(d));
			auto ba = number_str.toLocal8Bit();
			mtl_line("Number: \"%s\"", ba.data());
			at3 += number_str.size();
		} else if ((next_func_skip = ods::Function::TryNew(next, host, next_function)) != -1) {
			// add function to params:
			auto p = FormulaNode::Function(new ods::Function(next_function));
			retval.params_.append(p);
			at3 += next_func_skip;
		} else {
			auto ba = next.toLocal8Bit();
			mtl_warn("Don't know what to do with: \"%s\"", ba.data());
			return -1;
		}
		
		at3 = ods::FindNonWhitespace(s, at3);
		auto ba = s.mid(at3).toLocal8Bit();
		mtl_line("Now: \"%s\"", ba.data());
	}
	
	return -1;
}

namespace function {
int Min(QStringRef s, int start_at)
{
	mtl_trace();
	return -1;
}

int Sum(QStringRef s, int start_at)
{
	mtl_trace();
	return -1;
}
} // function::

const FunctionMeta*
FindFunctionMeta(const QString &name)
{
	for (const FunctionMeta &fi: GetSupportedFunctions()) {
		if (name == fi.name) {
			return &fi;
		}
	}
	
	return nullptr;
}

const QVector<FunctionMeta>&
GetSupportedFunctions() {
	static QVector<FunctionMeta> v = {
		FunctionMeta {"MIN", FunctionId::Min, function::Min},
		FunctionMeta {"SUM", FunctionId::Sum, function::Sum},
	};
	return v;
}


} // ods::function::
