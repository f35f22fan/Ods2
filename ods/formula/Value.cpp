#include "Value.hpp"

#include "CellRef.hpp"

#include "../Cell.hpp"
#include "../Duration.hpp"

#include <QDateTime>

namespace ods { // ods::
namespace formula { // ods::formula::

Value::Value() {}

Value::Value(const double d)
{
	SetDouble(d);
}

Value::~Value()
{
	Clear();
}

void
Value::Clear()
{
	if (is_double())
		delete as_double();
	else if (is_date())
		delete as_date();
	else if (is_time())
		delete as_time();
	else if (is_none()) {
		
	} else {
		it_happened();
	}
	
	data_ = nullptr;
	type_ = value::Type::None;
}

Value*
Value::Clone() const
{
	auto *p = new Value();
	p->Copy(this);
	return p;
}

void
Value::Copy(const formula::Value *rhs)
{
	if (rhs->is_date())
		SetDate(rhs->as_date());
	else if (rhs->is_double())
		SetDouble(*rhs->as_double());
	else if (rhs->is_string())
		SetString(rhs->as_string());
	else if (rhs->is_time())
		SetTime(rhs->as_time());
	else if (rhs->is_none()) {
		mtl_warn();
	} else {
		it_happened();
	}
	
	error_ = rhs->error_;
}

void
Value::Operation(formula::CellRef *cell_ref, const ods::Op op)
{
	formula::Value formula_value;
	formula_value.SetValue(cell_ref);
	
	if (formula_value.has_error())
	{
		mtl_warn();
		error_ = value::Error::BadEvaluation;
		return;
	}
	
	Operation(&formula_value, op);
}

void
Value::Operation(formula::Value *value, const ods::Op op)
{
	if (value == nullptr)
	{
		mtl_warn("nullptr");
		error_ = value::Error::BadEvaluation;
		return;
	}
	
	if (!value->is_double())
	{
		mtl_warn("not a double");
		
		if (value->is_date())
			mtl_line();
		else if (value->is_none())
			mtl_line();
		else if (value->is_string())
			mtl_line();
		else if (value->is_time())
			mtl_line();
		
		error_ = value::Error::BadEvaluation;
		return;
	}
	
	
	if (!is_double())
	{
		if (is_none())
		{
			SetDouble(0.0);
		} else {
			mtl_warn();
			error_ = value::Error::BadEvaluation;
			return;
		}
	}
	
	double d = *value->as_double();
	auto *p = as_double();
	double r;
	
	if (op == Op::Add)
	{
		r = *p + d;
		//mtl_line("%.2f + %.2f = %.2f", *p, d, r);
		*p = r;
	} else if (op == Op::Subtract) {
		r = *p - d;
		//mtl_line("%.2f - %.2f = %.2f", *p, d, r);
		*p = r;
	} else if (op == Op::Multiply) {
		r = *p * d;
		//mtl_line("%.2f * %.2f = %.2f", *p, d, r);
		*p = r;
	} else if (op == Op::Divide) {
		r = *p / d;
		//mtl_line("%.2f / %.2f = %.2f", *p, d, r);
		*p = r;
	} else {
		it_happened();
		error_ = value::Error::BadEvaluation;
	}
	
}

void
Value::SetDate(const QDateTime *p)
{
	Clear();
	
	if (p == nullptr)
		return;
	
	type_ = value::Type::Date;
	data_ = new QDateTime(*p);
}

void
Value::SetDouble(const double p)
{
	Clear();
	
	type_ = value::Type::Double;
	data_ = new double();
	*(double*)data_ = p;
}

void
Value::SetString(const QString *p)
{
	Clear();
	
	if (p == nullptr)
		return;
	
	type_ = value::Type::String;
	data_ = new QString(*p);
}

void
Value::SetTime(const ods::Duration *p)
{
	Clear();
	
	if (p == nullptr)
		return;
	
	type_ = value::Type::Duration;
	data_ = p->Clone();
}

void
Value::SetValue(formula::CellRef *cell_ref)
{
	auto *cell = cell_ref->GetCell();
	
	if (cell == nullptr || !cell->is_any_double())
	{
		mtl_warn();
		error_ = value::Error::BadEvaluation;
		return;
	}
	
	SetDouble(*cell->as_double());
}

void
Value::SetValue(formula::Value *value)
{
	Copy(value);
}

}} // ods::formula::
