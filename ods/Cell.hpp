#pragma once

#include "decl.hxx"
#include "err.hpp"
#include "global.hxx"
#include "ods.hh"
#include "value.hxx"

#include "inst/Abstract.hpp"
#include "inst/decl.hxx"

#include <QDateTime>

namespace ods { // ods::

class ODS_API Cell : public inst::Abstract
{
public:
	Cell(ods::Row *parent, Tag *cell_tag);
	Cell(ods::Row *parent);
	Cell(const Cell &cloner);
	virtual ~Cell();
	
	void
	AppendString(const QString &s);
	
	bool*
	as_boolean() const { return (bool*) value_data_; }

	double*
	as_currency() const { return as_double(); }
	
	QDateTime*
	as_date() const { return (QDateTime*) value_data_; }
	
	double*
	as_double() const { return (double*) value_data_; }

	ods::Duration*
	as_duration() const { return (ods::Duration*) value_data_; }
	
	double*
	as_percentage() const { return as_double(); }
	
	void
	ClearValue(const bool delete_data = true);
	
	virtual inst::Abstract*
	Clone(inst::Abstract *parent = nullptr) const override;
	
	ods::Formula*
	formula() const { return formula_; }
	
	// returns the text value of first TextP
	QString*
	GetFirstString() const;
	
	inst::StyleStyle*
	GetStyle() const;
	
	bool
	has_formula() const { return formula_ != nullptr; }
	
	bool
	is_any_double() const { return is_double() || is_currency() ||
		is_percentage(); }
	
	bool
	is_boolean() const { return office_value_type_ == ods::value::Type::Bool; }
	
	bool
	is_currency() const { return office_value_type_ == ods::value::Type::Currency; }
	
	bool
	is_date() const { return office_value_type_ == ods::value::Type::Date; }
	
	bool
	is_double() const { return office_value_type_ == ods::value::Type::Double; }

	bool
	is_duration() const { return office_value_type_ == ods::value::Type::Duration; }
	
	bool
	is_percentage() const { return office_value_type_ == ods::value::Type::Percentage; }
	
	bool
	is_string() const { return office_value_type_ == ods::value::Type::String; }
	
	bool
	is_value_set() const { return office_value_type_ != ods::value::Type::None; }
	
	int
	ncr() const { return number_columns_repeated_; }
	
	void
	ncr(const int n) { number_columns_repeated_ = n; }
	
	ods::Formula*
	NewFormula();
	
	inst::DrawFrame*
	NewDrawFrame();
	
	inst::StyleStyle*
	NewStyle();
	
	int
	number_columns_repeated() const { return number_columns_repeated_; }
	
	void
	number_columns_repeated(const int n) { number_columns_repeated_ = n; }
	
	int
	number_columns_spanned() const { return number_columns_spanned_; }
	
	void
	number_columns_spanned(const int n) { number_columns_spanned_ = n; }
	
	int
	number_rows_spanned() const { return number_rows_spanned_; }
	
	void
	number_rows_spanned(const int n) { number_rows_spanned_ = n; }
	
	QString
	QueryAddress() const;
	
	Length* // caller must delete returned value
	QueryDesiredHeight() const;
	
	inst::StyleFontFace*
	QueryFontFace(inst::StyleStyle *cell_style = nullptr,
		inst::TableTableColumn *table_column = nullptr) const;
	
	int
	QueryStart() const;
	
	void
	ReadValue(ods::Tag *tag);
	
	ods::Row*
	row() const { return row_; }
	
	void
	SetBoolean(const bool flag);
	
	void
	SetBooleanFromString(const QString &s);

	void
	SetCurrency(const double d, const QString str);

	void
	SetDate(const QDateTime *p);
	
	void
	SetDouble(const double d);

	void
	SetDuration(const Duration *p);
	
	void
	SetFirstString(const QString &s);

	void
	SetFormula(ods::Formula *p);
	
	void
	SetPercentage(const double d);
	
	void
	SetStyle(Abstract *a);
	
	const QString&
	table_style_name() const { return table_style_name_; }
	
	QByteArray
	TypeAndValueString() const;
	
	QString
	ValueToString() const;
	
	ods::value::Type
	value_type() const { return office_value_type_; }
	
	void
	value_type_set(const ods::value::Type kType) { office_value_type_ = kType; }
	
	const char*
	TypeToString() const { return ods::TypeToString(office_value_type_); }
	
	void
	WriteData(QXmlStreamWriter &xml) override;

private:
	
	void* CloneValue() const;
	void Init(ods::Tag *tag);
	void Scan(ods::Tag *tag);
	void Set(const QString &stype, const QString &value);
	
	void
	SetValue(void *value)
	{
		ClearValue(true);
		value_data_ = value;
	}
	
	void
	SetValue(void *value, const ods::value::Type kType)
	{
		ClearValue(true);
		value_data_ = value;
		office_value_type_ = kType;
	}
	
	void
	WriteValue(QXmlStreamWriter &xml);
	
	ods::Row *row_ = nullptr;
	void *value_data_ = nullptr;
	ods::value::Type office_value_type_ = ods::value::Type::None;
	
	int number_columns_repeated_ = 1;
	int number_columns_spanned_ = 1;
	int number_rows_spanned_ = 1;
	
	// table:style-name="ce3" office:value-type="percentage" office:value="0.9"
	// office:currency="USD", office:date-value="1983-12-30"
	// office:time-value="PT22H15M00S", office:boolean-value="true"
	ods::Formula *formula_ = nullptr;
	QString table_style_name_;
	QString office_currency_;
};

} // ods::
