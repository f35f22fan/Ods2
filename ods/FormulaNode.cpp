#include "FormulaNode.hpp"

#include "Cell.hpp"
#include "CellRef.hpp"
#include "Duration.hpp"
#include "ods.hh"

#include <float.h>

namespace ods {

FormulaNode::FormulaNode() {}
FormulaNode::~FormulaNode() {
	Clear();
}

FormulaNode::FormulaNode(const FormulaNode &rhs)
{
	DeepCopy(*this, rhs);
}

FormulaNode&
FormulaNode::operator=(const FormulaNode &rhs)
{
	DeepCopy(*this, rhs);
	return *this;
}

FormulaNode*
FormulaNode::Address(ods::Address *a)
{
	auto *p = new FormulaNode();
	p->type_ = Type::Address;
	p->data_.address = a;
	return p;
}

bool
FormulaNode::ApplyMinus() {
	// return false if failed
	
	if (is_double()) {
		SetDouble(-as_double());
		return true;
	}
	
	if (is_currency()) {
		ods::Currency *c = as_currency();
		c->qtty = -c->qtty;
		return true;
	}
	
	if (is_percentage()) {
		SetPercentage(-as_percentage());
		return true;
	}
	
	mtl_trace();
	return false;
}

double
FormulaNode::as_any_double() const
{
	if (is_double())
		return as_double();
	if (is_currency())
		return as_currency()->qtty;
	if (is_percentage())
		return as_percentage();
	
	mtl_trace();
	return 0.0;
}

void
FormulaNode::Clear()
{
	if (is_any_double()) {
		type_ = Type::None;
		return;
	}
	if (is_address())
		delete as_address();
	else if (is_function())
		delete as_function();
	else if (is_currency())
		delete as_currency();
	else if (is_date_time())
		delete as_date_time();
	else if (is_date())
		delete as_date();
	else if (is_time())
		delete as_time();
	else if (is_string())
		delete as_string();
	
	type_ = Type::None;
}

FormulaNode*
FormulaNode::Clone()
{
	auto *p = new FormulaNode();
	DeepCopy(*p, *this);
	return p;
}

void
FormulaNode::DeepCopy(FormulaNode &dest, const FormulaNode &src)
{
	dest.type_ = src.type_;
	
	if (src.is_address()) {
		dest.data_.address = src.data_.address->Clone();
	} else if (src.is_function())
		dest.data_.function = src.data_.function->Clone();
	else if (src.is_double())
		dest.data_.number = src.data_.number;
	else if (src.is_op())
		dest.data_.op = src.data_.op;
	else if (src.is_brace())
		dest.data_.brace = src.data_.brace;
	else if (src.is_bool())
		dest.data_.flag = src.data_.flag;
	else if (src.is_percentage())
		dest.data_.percentage = src.data_.percentage;
	else if (src.is_currency())
		dest.data_.currency = src.data_.currency->Clone();
	else if (src.is_date_time())
		dest.data_.date_time = new QDateTime(*src.data_.date_time);
	else if (src.is_date())
		dest.data_.date = new QDate(*src.data_.date);
	else if (src.is_string())
		dest.data_.s = new QString(*src.data_.s);
	else if (src.is_time())
		dest.data_.time = src.data_.time->Clone();
	else {
		mtl_trace();
	}
}

FormulaNode*
FormulaNode::Function(ods::Function *f)
{
	auto *p = new FormulaNode();
	p->type_ = Type::Function;
	p->data_.function = f;
	return p;
}

FormulaNode*
FormulaNode::Double(const double d)
{
	auto *p = new FormulaNode();
	p->type_ = Type::Double;
	p->data_.number = d;
	return p;
}

FormulaNode*
FormulaNode::Op(const ods::Op op)
{
	auto *p = new FormulaNode();
	p->type_ = Type::Op;
	p->data_.op = op;
	return p;
}

FormulaNode*
FormulaNode::Bool(bool b)
{
	auto *p = new FormulaNode();
	p->type_ = Type::Bool;
	p->data_.flag = b;
	return p;
}

FormulaNode*
FormulaNode::Brace(ods::Brace b)
{
	auto *p = new FormulaNode();
	p->type_ = Type::Brace;
	p->data_.brace = b;
	return p;
}

FormulaNode*
FormulaNode::Currency(ods::Currency *c)
{
	auto *p = new FormulaNode();
	p->type_ = Type::Currency;
	p->data_.currency = c;
	return p;
}

FormulaNode*
FormulaNode::Date(QDate *date)
{
	auto *p = new FormulaNode();
	p->type_ = Type::Date;
	p->data_.date = date;
	return p;
}

FormulaNode*
FormulaNode::DateTime(QDateTime *p)
{
	auto *node = new FormulaNode();
	node->type_ = Type::DateTime;
	node->data_.date_time = p;
	return node;
}

bool
FormulaNode::Operation(const ods::Op op, FormulaNode *rhs)
{
	if (op == Op::Plus || op == Op::Minus)
		return OperationPlusMinus(op, rhs);
	
	if (op == Op::Multiply || op == Op::Divide)
		return OperationMultDivide(op, rhs);
	
	if (op == Op::Ampersand)
		return OperationAmpersand(op, rhs);
	
	mtl_trace();
	return false;
}

bool
FormulaNode::OperationAmpersand(const ods::Op op, FormulaNode *rhs)
{
	QString *s = new QString();
	s->append(toString());
	s->append(rhs->toString());
	SetString(s);
	return true;
}

bool
FormulaNode::OperationMultDivide(const ods::Op op, FormulaNode *rhs)
{
	const bool is_rhs_any_double = rhs->is_any_double();
	double rhs_double = is_rhs_any_double ? rhs->as_any_double() : 0;
	const bool mult = (op == ods::Op::Multiply);
	
	if (is_double()) {
		if (is_rhs_any_double) {
			if (mult) {
				SetDouble(as_double() * rhs_double);
			} else {
				SetDouble(as_double() / rhs_double);
			}
			return true;
		} else if (rhs->is_function()) {
			auto *f = rhs->as_function();
			const ods::FormulaNode *rhs_value = f->Eval();
			ods::AutoDelete ad(rhs_value);
			CHECK_TRUE((rhs_value != nullptr));
			CHECK_TRUE((!rhs_value->is_none()))
			if (rhs_value->is_any_double()) {
				rhs_double = rhs_value->as_any_double();
				if (mult) {
					SetDouble(as_double() * rhs_double);
				} else {
					SetDouble(as_double() / rhs_double);
				}
				return true;
			} else {
				mtl_trace("TBD: rhs value op");
				return false;
			}
		} else {
			mtl_warn("TBD");
		}
	} else if (is_currency()) {
		ods::Currency *c = as_currency();
		if (is_rhs_any_double) {
			if (mult)
				c->qtty *= rhs_double;
			else
				c->qtty /= rhs_double;
			return true;
		} else {
			mtl_warn("TBD");
		}
	} else if (is_percentage()) {
		if (is_rhs_any_double) {
			if (mult)
				SetPercentage(as_percentage() * rhs_double);
			else
				SetPercentage(as_percentage() / rhs_double);
			return true;
		} else {
			mtl_warn("TBD");
		}
	}
	
	mtl_warn("TBD");
	return false;
}

bool
FormulaNode::OperationPlusMinus(const ods::Op op, FormulaNode *rhs)
{
	const bool plus = (op == ods::Op::Plus);
	
	if (is_double()) {
		if (rhs->is_any_double()) {
			if (plus) {
				double result = as_double() + rhs->as_any_double(); 
				SetDouble(result);
				
			} else
				SetDouble(as_double() - rhs->as_any_double());
			return true;
		} else if (rhs->is_address()) {
			QVector<FormulaNode*> values;
			CHECK_TRUE(function::ExtractAddressValues(rhs, values));
			if (values.size() != 1) {
				mtl_warn("Don't know what to do when values count = %d", values.size());
				return false;
			}
			return OperationPlusMinus(op, values[0]);
		}
	} else if (is_currency()) {
		ods::Currency *c = as_currency();
		if (rhs->is_any_double()) {
			if (plus)
				c->qtty += rhs->as_any_double();
			else
				c->qtty -= rhs->as_any_double();
			return true;
		}
	} else if (is_percentage()) {
		if (rhs->is_any_double()) {
			if (plus)
				SetPercentage(as_percentage() + rhs->as_any_double());
			else
				SetPercentage(as_percentage() - rhs->as_any_double());
			return true;
		}
	} else if (is_function()) {
		auto *f = as_function();
		const ods::FormulaNode *val = f->Eval();
		ods::AutoDelete ad(val);
		CHECK_TRUE(((val != nullptr) && (!val->is_none())))
		if (val->is_double()) {
			SetDouble(val->as_double());
			return Operation(op, rhs);
		}
	} else if (is_address()) {
		ods::Address *address = as_address();
		if (address->is_cell_range()) {
			mtl_warn("Cell range not implemented");
			return false;
		}
		
		ods::Cell *cell = address->cell()->GetCell();
		CHECK_PTR(cell);
		FormulaNode result;
		CHECK_TRUE(function::ExtractCellValue(cell, result));
		if (cell->is_double()) {
			SetDouble(*cell->as_double());
		} else if (cell->is_currency()) {
			ods::Currency *c = cell->QueryCurrencyObject();
			CHECK_PTR(c);
			SetCurrency(c);
		} else if (cell->is_percentage()) {
			SetPercentage(*cell->as_percentage());
		} else {
			mtl_trace();
			return false;
		}
		
		return OperationPlusMinus(op, rhs);
	}
	
	auto self_ba = toString().toLocal8Bit();
	auto rhs_ba = rhs->toString().toLocal8Bit();
	mtl_warn("TBD: this: \"%s\", rhs: \"%s\"", self_ba.data(), rhs_ba.data());
	return false;
}

FormulaNode*
FormulaNode::Percentage(double d)
{
	auto *p = new FormulaNode();
	p->SetPercentage(d);
	return p;
}

FormulaNode*
FormulaNode::String(QString *s)
{
	auto *p = new FormulaNode();
	p->SetString(s);
	return p;
}

FormulaNode*
FormulaNode::Time(ods::Duration *d)
{
	auto *p = new FormulaNode();
	p->SetTime(d);
	return p;
}

QString
FormulaNode::toString(const ods::ToStringArgs args) const
{
	if (is_address())
		return data_.address->toString();
	else if (is_function())
		return data_.function->toXmlString();
	else if (is_double())
		return QString::number(as_double());
	else if (is_currency())
		return QString::number(as_currency()->qtty);
	else if (is_percentage())
		return QString::number(as_percentage());
	else if (is_op())
		return ods::op::ToString(data_.op);
	else if (is_brace())
		return ods::ToString(data_.brace);
	else if (is_string()) {
		if (args == ToStringArgs::IncludeQuotMarks) {
			const QChar quot = QChar('\"');
			return quot + *as_string() + quot;
		}
		return *as_string();
	} else if (is_date()) {
		return as_date()->toString();
	} else if (is_date_time()) {
		return as_date_time()->toString();
	} else if (is_none())
		return QLatin1String("[none!!]");
	else {
		it_happened();
		return QLatin1String("Shouldn't happen!");
	}
}

}
