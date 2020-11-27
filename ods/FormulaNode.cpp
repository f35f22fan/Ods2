#include "FormulaNode.hpp"

#include "Cell.hpp"
#include "CellRef.hpp"
#include "eval.hh"
#include "FormulaError.hpp"
#include "ods.hh"
#include "Time.hpp"
#include "inst/TableNamedRange.hpp"

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
FormulaNode::Reference(ods::Reference *a)
{
	auto *p = new FormulaNode();
	p->type_ = Type::Reference;
	p->data_.reference = a;
	return p;
}

void
FormulaNode::AdoptDefaultValueFrom(const FormulaNode &rhs)
{
	if (rhs.is_double()) {
		SetDouble(0);
	} else if (rhs.is_currency()) {
		auto *c = rhs.as_currency()->Clone();
		c->qtty = 0;
		SetCurrency(c);
	} else if (rhs.is_percentage())
		SetPercentage(0);
	else if (rhs.is_string()) {
		SetString(new QString());
	} else if (is_time()) {
		SetTime(new ods::Time());
	} else if (rhs.is_date()) {
		SetDate(new QDate(0, 0, 0));
	} else if (rhs.is_date_time()) {
		auto *p = new QDateTime();
		p->setDate(QDate(0, 0, 0));
		p->setTime(QTime(0, 0, 0));
		SetDateTime(p);
	} else {
		it_happened();
	}
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
	if (is_reference())
		delete as_reference();
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
	else if (is_error())
		delete as_error();
	
	type_ = Type::None;
}

FormulaNode*
FormulaNode::Clone() const
{
	auto *p = new FormulaNode();
	DeepCopy(*p, *this);
	return p;
}

bool
FormulaNode::ConvertFunctionOrAddressToValue()
{
	if (is_reference()) {
		ods::Reference *reference = as_reference();
		if (reference->is_cell_range()) {
			mtl_warn("Cell range not implemented");
			return false;
		}
		
		ods::Cell *cell = reference->cell()->GetCell();
		CHECK_PTR(cell);
		CHECK_TRUE(eval::ExtractCellValue(cell, *this));
	} else if (is_function()) {
		auto *f = as_function();
		const ods::FormulaNode *val = f->Eval();
		ods::AutoDelete ad(val);
		CHECK_TRUE((val != nullptr));
		DeepCopy(*this, *val);
	}
	
	return true;
}

void
FormulaNode::DeepCopy(FormulaNode &dest, const FormulaNode &src)
{
	dest.Clear();
	dest.type_ = src.type_;
	
	if (src.is_reference()) {
		dest.data_.reference = src.data_.reference->Clone();
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
	else if (src.is_named_range())
		dest.data_.named_range = src.data_.named_range;// not cloned because not owned
	else if (src.is_error())
		dest.data_.error = src.data_.error->Clone();
	else if (src.is_none()) {
		// a Clone() on empty object, that's fine.
	} else {
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

FormulaNode*
FormulaNode::Error(ods::FormError e)
{
	auto *node = new FormulaNode();
	node->type_ = Type::Error;
	node->data_.error = new ods::FormulaError(e);
	return node;
}

FormulaNode*
FormulaNode::Error(ods::FormError e, const QString &s)
{
	auto *node = Error(e);
	if (!s.isEmpty())
		node->as_error()->more_info(s);
	return node;
}

FormulaNode*
FormulaNode::Error(ods::FormulaError *e)
{
mtl_trace();
	auto *node = new FormulaNode();
	node->type_ = Type::Error;
	node->data_.error = e;
	return node;
}

bool
FormulaNode::InterpretAsBoolean() const
{
	if (is_empty())
		return false;
	if (is_bool())
		return as_bool();
	if (is_any_double())
		return (as_any_double() != 0);
	if (is_string())
		return !as_string()->isEmpty();
	
	return true;
}

FormulaNode*
FormulaNode::NamedRange(inst::TableNamedRange *p)
{
	auto *node = new FormulaNode();
	node->type_ = Type::NamedRange;
	node->data_.named_range = p;
	return node;
}

bool
FormulaNode::Operation(const ods::Op op, FormulaNode *rhs)
{
	switch (op) {
	case Op::Plus:
	case Op::Minus: return OperationPlusMinus(op, rhs);
	case Op::Multiply:
	case Op::Divide: return OperationMultDivide(op, rhs);
	case Op::Ampersand: return OperationAmpersand(op, rhs);
	case Op::Equals:
	case Op::NotEquals:
	case Op::Greater:
	case Op::GreaterOrEqual:
	case Op::Less:
	case Op::LessOrEqual: return OperationEquality(op, rhs);
	default: {
		auto ba = ods::op::ToString(op).toLocal8Bit();
		mtl_trace("Op: \"%s\"", ba.data());
		return false;
	}
	}
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
FormulaNode::OperationEquality(const ods::Op op, FormulaNode *rhs_node)
{
	if (is_reference()) {
		ods::Reference *reference = as_reference();
		if (reference->is_cell_range()) {
			mtl_warn("Cell range not implemented");
			return false;
		}
	
		ods::Cell *cell = reference->cell()->GetCell();
		CHECK_PTR(cell);
		CHECK_TRUE(eval::ExtractCellValue(cell, *this));
		return OperationEquality(op, rhs_node);
	}
	
	if (rhs_node->is_reference()) {
		ods::Reference *reference = rhs_node->as_reference();
		if (reference->is_cell_range()) {
			mtl_warn("Cell range not implemented");
			return false;
		}
	
		ods::Cell *cell = reference->cell()->GetCell();
		CHECK_PTR(cell);
		CHECK_TRUE(eval::ExtractCellValue(cell, *rhs_node));
		return OperationEquality(op, rhs_node);
	}
	
	if (is_function()) {
		auto *node = as_function()->Eval();
		CHECK_PTR(node);
		bool flag = node->OperationEquality(op, rhs_node);
		delete node;
		return flag;
	}
	
	if (rhs_node->is_function()) {
		auto *node = rhs_node->as_function()->Eval();
		CHECK_PTR(node);
		bool flag = OperationEquality(op, node);
		delete node;
		return flag;
	}
	
	if (is_any_double()) {
		if (rhs_node->is_any_double()) {
			const double lhs = as_any_double();
			const double rhs = rhs_node->as_any_double();
			bool flag;
			if (op == Op::Equals)
				flag = eval::IsNearlyEqual(lhs, rhs);
			else if (op == Op::Greater)
				flag = lhs > rhs;
			else if (op == Op::GreaterOrEqual)
				flag = lhs >= rhs;
			else if (op == Op::Less)
				flag = lhs < rhs;
			else if (op == Op::LessOrEqual)
				flag = lhs <= rhs;
			else if (op == Op::NotEquals)
				flag = lhs != rhs;
			SetBool(flag);
			return true;
		} else {
// Legit use case: SUMIF(.., cond, ...) can test if a
// string equals to a number which is OK, the result is bool=false.
			SetBool(false);
			return true;
		}
	}
	
	if (is_date()) {
		if (rhs_node->is_date()) {
			const QDate *lhs = as_date();
			const QDate *rhs = rhs_node->as_date();
			bool flag;
			
			if (op == Op::Equals)
				flag = lhs == rhs;
			else if (op == Op::Greater)
				flag = lhs > rhs;
			else if (op == Op::GreaterOrEqual)
				flag = lhs >= rhs;
			else if (op == Op::Less)
				flag = lhs < rhs;
			else if (op == Op::LessOrEqual)
				flag = lhs <= rhs;
			else if (op == Op::NotEquals)
				flag = *lhs != *rhs;
			else {
				it_happened();
				return false;
			}
			
			SetBool(flag);
			return true;
		} else {
			SetBool(false);
			return true;
		}
	}
	
	if (is_date_time()) {
		if (rhs_node->is_date_time()) {
			const QDateTime *lhs = as_date_time();
			const QDateTime *rhs = rhs_node->as_date_time();
			bool flag;
			
			if (op == Op::Equals)
				flag = *lhs == *rhs;
			else if (op == Op::Greater)
				flag = *lhs > *rhs;
			else if (op == Op::GreaterOrEqual)
				flag = *lhs >= *rhs;
			else if (op == Op::Less)
				flag = *lhs < *rhs;
			else if (op == Op::LessOrEqual)
				flag = *lhs <= *rhs;
			else if (op == Op::NotEquals)
				flag = *lhs != *rhs;
			else {
				it_happened();
				return false;
			}
			
			SetBool(flag);
			return true;
		} else {
			SetBool(false);
			return true;
		}
	}
	
	if (is_string()) {
		if (rhs_node->is_string()) {
			QString lhs = *as_string();
			QString rhs = rhs_node->toString();
			
			bool flag;
			
			if (op == Op::Equals)
				flag = lhs == rhs;
			else if (op == Op::Greater)
				flag = lhs > rhs;
			else if (op == Op::GreaterOrEqual)
				flag = lhs >= rhs;
			else if (op == Op::Less)
				flag = lhs < rhs;
			else if (op == Op::LessOrEqual)
				flag = lhs <= rhs;
			else if (op == Op::NotEquals)
				flag = lhs != rhs;
			else {
				it_happened();
				return false;
			}
			
			SetBool(flag);
			return true;
		} else {
			SetBool(false);
			return true;
		}
	}
	
	if (is_time()) {
		if (rhs_node->is_time()) {
			const ods::Time &lhs = *as_time();
			const ods::Time &rhs = *rhs_node->as_time();
			bool flag = false;
			
			if (op == Op::Equals)
				flag = lhs == rhs;
			else if (op == Op::Greater)
				flag = lhs > rhs;
			else if (op == Op::GreaterOrEqual)
				flag = lhs >= rhs;
			else if (op == Op::Less)
				flag = lhs < rhs;
			else if (op == Op::LessOrEqual)
				flag = lhs <= rhs;
			else if (op == Op::NotEquals)
				flag = lhs != rhs;
			else {
				it_happened();
				return false;
			}
			
			SetBool(flag);
			return true;
		} else {
			SetBool(false);
			return true;
		}
	}
	
	if (is_bool()) {
		if (rhs_node->is_bool()) {
			bool flag = false;
			if (op == Op::Equals)
				flag = as_bool() == rhs_node->as_bool();
			else if (op == Op::NotEquals)
				flag = as_bool() != rhs_node->as_bool();
			else {
				mtl_trace();
			}
			SetBool(flag);
			return true;
		} else {
			SetBool(false);
			return true;
		}
	}
	
	mtl_warn("TBD");
	SetBool(true);
	return false;
}

bool
FormulaNode::OperationMultDivide(const ods::Op op, FormulaNode *rhs)
{
	CHECK_TRUE(ConvertFunctionOrAddressToValue());
	CHECK_TRUE(rhs->ConvertFunctionOrAddressToValue());
	
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
	CHECK_TRUE(ConvertFunctionOrAddressToValue());
	CHECK_TRUE(rhs->ConvertFunctionOrAddressToValue());
	const bool plus = (op == ods::Op::Plus);
	
	if (is_double()) {
		if (rhs->is_any_double()) {
			if (plus) {
				double result = as_double() + rhs->as_any_double(); 
				SetDouble(result);
				
			} else
				SetDouble(as_double() - rhs->as_any_double());
			return true;
		} else {
			mtl_trace();
		}
	} else if (is_currency()) {
		ods::Currency *c = as_currency();
		if (rhs->is_any_double()) {
			if (plus)
				c->qtty += rhs->as_any_double();
			else
				c->qtty -= rhs->as_any_double();
			return true;
		} else {
			mtl_trace();
		}
	} else if (is_percentage()) {
		if (rhs->is_any_double()) {
			if (plus)
				SetPercentage(as_percentage() + rhs->as_any_double());
			else
				SetPercentage(as_percentage() - rhs->as_any_double());
			return true;
		} else {
			mtl_trace();
		}
	} else if (is_time()) {
		
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

void
FormulaNode::PrintError() const {
	if (is_error())
		as_error()->PrintError();
}

FormulaNode*
FormulaNode::String(QString *s)
{
	auto *p = new FormulaNode();
	p->SetString(s);
	return p;
}

FormulaNode*
FormulaNode::Time(ods::Time *d)
{
	auto *p = new FormulaNode();
	p->SetTime(d);
	return p;
}

QString
FormulaNode::toString(const ods::ToStringArgs args) const
{
	if (is_reference())
		return data_.reference->toString();
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
	} else if (is_time()) {
		return as_time()->toString();
	} else if (is_date()) {
		return as_date()->toString(Qt::ISODate);
	} else if (is_date_time()) {
		return as_date_time()->toString(Qt::ISODate);
	} else if (is_bool()) {
		return as_bool() ? QLatin1String("true") : QLatin1String("false");
	} else if (is_named_range()) {
		return as_named_range()->name();
	} else if (is_error()) {
		return as_error()->toString();
	} else if (is_none())
		return QString();
	else {
		it_happened();
		return QString();
	}
}

}
