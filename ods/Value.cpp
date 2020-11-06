#include "Value.hpp"

#include "CellRef.hpp"
#include "Cell.hpp"
#include "Duration.hpp"

namespace ods { // ods::

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
	type_ = ValueType::None;
}

Value*
Value::Clone() const
{
	auto *p = new Value();
	p->Copy(*this);
	return p;
}

void
Value::Copy(const ods::Value &rhs)
{
	if (rhs.is_date())
		SetDate(*rhs.as_date());
	else if (rhs.is_double())
		SetDouble(*rhs.as_double());
	else if (rhs.is_string())
		SetString(*rhs.as_string());
	else if (rhs.is_time())
		SetTime(*rhs.as_time());
	else if (rhs.is_none()) {
		mtl_warn();
	} else {
		it_happened();
	}
	
	error_ = rhs.error_;
}

void
Value::Operation(CellRef *cell_ref, const ods::Op op)
{
mtl_trace("To be rewritten");
	Value formula_value;
	formula_value.SetValue(cell_ref);
	
	if (formula_value.has_error())
	{
		mtl_warn();
		error_ = "Operation error";
		return;
	}
	
	Operation(&formula_value, op);
}

void
Value::Operation(Value *value, const ods::Op op)
{
	if (value == nullptr)
	{
		mtl_warn("nullptr");
		error_ = "Operation(): value = nullptr";
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
		
		error_ = "Operation(): !value->is_double()";
		return;
	}
	
	
	if (!is_double())
	{
		if (is_none())
		{
			SetDouble(0.0);
		} else {
			mtl_warn();
			error_ = "Operation(): !self->is_double()";
			return;
		}
	}
	
	double d = *value->as_double();
	auto *p = as_double();
	double r;
	
	if (op == Op::Plus)
	{
		r = *p + d;
		//mtl_line("%.2f + %.2f = %.2f", *p, d, r);
		*p = r;
	} else if (op == Op::Minus) {
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
		error_ = "Unprocessed ods::Op";
	}
	
}

void
Value::SetDate(const QDateTime &p)
{
	Clear();
	type_ = ValueType::Date;
	data_ = new QDateTime(p);
}

void
Value::SetDouble(const double p)
{
	Clear();
	type_ = ValueType::Double;
	data_ = new double();
	*(double*)data_ = p;
}

void
Value::SetString(const QString &p)
{
	Clear();
	type_ = ValueType::String;
	data_ = new QString(p);
}

void
Value::SetTime(const ods::Duration &p)
{
	Clear();
	type_ = ValueType::Duration;
	data_ = p.Clone();
}

void
Value::SetValue(CellRef *cell_ref)
{
	auto *cell = cell_ref->GetCell();
	
	if (cell == nullptr || !cell->is_any_double())
	{
		mtl_warn();
		error_ = "cell == nullptr || !cell->is_any_double()";
		return;
	}
	
	SetDouble(*cell->as_double());
}

void
Value::SetValue(const Value &value)
{
	Copy(value);
}

} // ods::
