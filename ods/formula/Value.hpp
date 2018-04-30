#pragma once

#include "decl.hxx"

#include "../decl.hxx"
#include "../err.hpp"
#include "../ods.hxx"
#include "../value.hxx"

namespace ods { // ods::
namespace formula { // ods::formula::

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
	Copy(const formula::Value *rhs);
	
	ods::value::Error
	error() const { return error_; }
	
	void
	error(const ods::value::Error e) { error_ = e; }
	
	bool
	has_error() const { return error_ != value::Error::None; }
	
	bool
	is_date() const { return type_ == value::Type::Date; }
	
	bool
	is_none() const { return type_ == value::Type::None; }
	
	bool
	is_double() const { return type_ == value::Type::Double
		|| type_ == value::Type::Currency
		|| type_ == value::Type::Percentage; }
	
	bool
	is_string() const { return type_ == value::Type::String; }
	
	bool
	is_time() const { return type_ == value::Type::Duration; }
	
	void
	Operation(formula::CellRef *cell_ref, const ods::Op op);
	
	void
	Operation(formula::Value *value, const ods::Op op);
	
	void
	SetDate(const QDateTime *p);
	
	void
	SetDouble(const double p);
	
	void
	SetString(const QString *p);
	
	void
	SetTime(const ods::Duration *p);
	
	void
	SetValue(formula::CellRef *cell_ref);
	
	void
	SetValue(formula::Value *value);
	
private:
	NO_ASSIGN_COPY_MOVE(Value);
	
	ods::value::Type type_ = value::Type::None;
	void *data_ = nullptr;
	value::Error error_ = ods::value::Error::None;
};

}} // ods::formula::
