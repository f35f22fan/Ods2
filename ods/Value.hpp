#pragma once

#include "decl.hxx"
#include "err.hpp"
#include "ods.hxx"
#include "op.hh"
#include "value.hxx"

#include <QDateTime>

namespace ods { // ods::

class ODS_API Value
{
public:
	Value();
	Value(const double d);
	virtual ~Value();
	
	QDateTime*
	as_date() const { return (QDateTime*) data_; }
	
	double*
	as_double() const { return (double*) data_; }
	
	QString*
	as_string() const { return (QString*) data_; }
	
	ods::Duration*
	as_time() const { return (ods::Duration*) data_; }
	
	void
	Clear();
	
	Value*
	Clone() const;
	
	void
	Copy(const Value &rhs);
	
	const QString&
	error() const { return error_; }
	
	void
	error(const QString &s) { error_ = s; }
	
	bool
	has_error() const { return error_.size() > 0; }
	
	bool
	is_date() const { return type_ == ValueType::Date; }
	
	bool
	is_none() const { return type_ == ValueType::None; }
	
	bool
	is_double() const { return type_ == ValueType::Double
		|| type_ == ValueType::Currency
		|| type_ == ValueType::Percentage; }
	
	bool
	is_string() const { return type_ == ValueType::String; }
	
	bool
	is_time() const { return type_ == ValueType::Duration; }
	
	bool
	ok() const { return !has_error() && !is_none(); }
	
	void
	Operation(CellRef *cell_ref, const ods::Op op);
	
	void
	Operation(Value *value, const ods::Op op);
	
	void
	SetDate(const QDateTime &p);
	
	void
	SetDouble(const double p);
	
	void
	SetString(const QString &p);
	
	void
	SetTime(const Duration &p);
	
	void
	SetValue(CellRef *cell_ref);
	
	void
	SetValue(const Value &value);
	
private:
	NO_ASSIGN_COPY_MOVE(Value);
	
	ValueType type_ = ValueType::None;
	void *data_ = nullptr;
	QString error_;
};

} // ods::
