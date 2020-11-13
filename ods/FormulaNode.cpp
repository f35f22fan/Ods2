#include "FormulaNode.hpp"

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
	if (is_address())
		delete as_address();
	else if (is_function())
		delete as_function();
	else if (is_time())
		delete as_time();
	else if (is_currency())
		delete as_currency();
	
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
		dest.data_.currency = src.data_.currency;
	else if (src.is_date())
		dest.data_.date = src.data_.date;
	else if (src.is_string())
		dest.data_.string = src.data_.string;
	else if (src.is_time())
		dest.data_.time = src.data_.time;
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
FormulaNode::Date(const QDateTime &date)
{
	auto *p = new FormulaNode();
	p->type_ = Type::Date;
	p->data_.date = date;
	return p;
}

/*FormulaNode*
FormulaNode::From(const ods::Value &v)
{
	if (v.is_double())
		return FormulaNode::Double(*v.as_double());
	if (v.is_currency())
		return FormulaNode::Currency(v.as_currency()->Clone());
	if (v.is_date())
		return FormulaNode::Date(*v.as_date());
	if (v.is_string())
		return FormulaNode::String(*v.as_string());
	if (v.is_time())
		return FormulaNode::Time(v.as_time()->Clone());
	if (v.is_percentage())
		return FormulaNode::Percentage(*v.as_percentage());
	if (v.is_bool())
		return FormulaNode::Bool(*v.as_bool());
	if (v.is_none()) {
		mtl_trace();
		return nullptr;
	}
	it_happened();
	return nullptr;
} */

bool
FormulaNode::Operation(const ods::Op op, FormulaNode *rhs)
{
	if (op == Op::Plus || op == Op::Minus)
		return OperationPlusMinus(op, rhs);
	
	if (op == Op::Multiply || op == Op::Divide)
		return OperationMultDivide(op, rhs);
	
	return false;
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
			const ods::Value &rhs_value = rhs->as_function()->Eval();
			if (rhs_value.is_none()) {
				mtl_trace();
				return false;
			}
			if (rhs_value.is_any_double()) {
				rhs_double = rhs_value.as_any_double();
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
	const bool is_any_double = rhs->is_any_double();
	double rhs_double = is_any_double ? rhs->as_any_double() : 0;
	const bool plus = (op == ods::Op::Plus);
	
	if (is_double()) {
		if (is_any_double) {
			if (plus) {
				double result = as_double() + rhs_double; 
				SetDouble(result);
				
			} else
				SetDouble(as_double() - rhs_double);
			return true;
		}
	} else if (is_currency()) {
		ods::Currency *c = as_currency();
		if (is_any_double) {
			if (plus)
				c->qtty += rhs_double;
			else
				c->qtty -= rhs_double;
			return true;
		}
	} else if (is_percentage()) {
		if (is_any_double) {
			if (plus)
				SetPercentage(as_percentage() + rhs_double);
			else
				SetPercentage(as_percentage() - rhs_double);
			return true;
		}
	} else if (is_function()) {
		ods::Value val = as_function()->Eval();
		if (val.is_none()) {
			mtl_trace();
			return false;
		}
		if (val.is_double()) {
			SetDouble(*val.as_double());
			return Operation(op, rhs);
		}
	} else if (is_address()) {
		QVector<FormulaNode*> nodes;
		mtl_warn("TBD");
		//function::ExtractValue
	}
	
	mtl_warn("TBD");
	return false;
}

FormulaNode*
FormulaNode::Percentage(double d)
{
	auto *p = new FormulaNode();
	p->type_ = Type::Percentage;
	p->data_.percentage = d;
	return p;
}

FormulaNode*
FormulaNode::String(const QString &s)
{
	auto *p = new FormulaNode();
	p->type_ = Type::String;
	p->data_.string = s;
	return p;
}

FormulaNode*
FormulaNode::Time(ods::Duration *d)
{
	auto *p = new FormulaNode();
	p->type_ = Type::Time;
	p->data_.time = d;
	return p;
}

QString
FormulaNode::toString() const
{
	if (is_address())
		return data_.address->toString();
	else if (is_function())
		return data_.function->toXmlString();
	else if (is_double())
		return QString::number(as_double(), 'f', FLT_DIG);
	else if (is_currency())
		return QString::number(as_currency()->qtty, 'f', FLT_DIG);
	else if (is_percentage())
		return QString::number(as_percentage(), 'f', FLT_DIG);
	else if (is_op())
		return ods::op::ToString(data_.op);
	else if (is_brace())
		return ods::ToString(data_.brace);
	else if (is_none())
		return QLatin1String("[none!!]");
	else {
		it_happened();
		return "Shouldn't happen!!";
	}
}

}
