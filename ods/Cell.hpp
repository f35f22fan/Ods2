#pragma once

#include "cell.hxx"
#include "currency.hxx"
#include "decl.hxx"
#include "err.hpp"
#include "global.hxx"
#include "ods.hh"

#include "inst/Abstract.hpp"
#include "inst/decl.hxx"

#include <QDateTime>
#include <QFileDevice>

namespace ods { // ods::

class ODS_API Cell : public inst::Abstract
{
public:
	Cell(ods::Row *parent, Tag *cell_tag, ndff::Container *cntr = 0);
	Cell(ods::Row *parent);
	Cell(const Cell &cloner);
	virtual ~Cell();
	
	void AppendString(const QString &s);
	bool* as_boolean() const { return (bool*) value_data_; }
	double* as_currency() const { return as_double(); }
	QDate* as_date() const { return (QDate*) value_data_; }
	QDateTime* as_date_time() const { return (QDateTime*) value_data_; }
	double* as_double() const { return (double*) value_data_; }
	double* as_percentage() const { return as_double(); }
	ods::Time* as_time() const { return (ods::Time*) value_data_; }
	void ClearValue(const bool delete_data = true);
	
	virtual inst::Abstract*
	Clone(inst::Abstract *parent = nullptr) const override;
	
	bool covered() const { return bits_ & ods::CoveredBit; }
	void covered(const bool do_set) {
		if (do_set)
			bits_ |= ods::CoveredBit;
		else
			bits_ &= ~ods::CoveredBit;
	}
	
	inst::StyleStyle* FetchStyle();
	ods::Formula* formula() const { return formula_; }
	QString FullName() const override;
	// returns the text value of first TextP
	const QString *GetCellString() const;
	inst::StyleStyle* GetStyle() const { return Get(table_style_name_); }
	inst::StyleStyle* GetStyle(const ods::CreateIfNeeded cin = CreateIfNeeded::No);
	
	bool has_delete_region() const { return delete_region_.start != -1; }
	bool has_formula() const { return formula_ != nullptr; }
	bool is_any_double() const { return is_double() || is_currency() || is_percentage(); }
	bool is_boolean() const { return office_value_type_ == ods::ValueType::Bool; }
	bool is_currency() const { return office_value_type_ == ods::ValueType::Currency; }
	bool is_date() const { return office_value_type_ == ods::ValueType::Date; }
	bool is_date_time() const { return office_value_type_ == ods::ValueType::DateTime; }
	bool is_double() const { return office_value_type_ == ods::ValueType::Double; }
	bool is_time() const { return office_value_type_ == ods::ValueType::Time; }
	bool is_percentage() const { return office_value_type_ == ods::ValueType::Percentage; }
	bool is_string() const { return office_value_type_ == ods::ValueType::String; }
	bool is_value_set() const { return office_value_type_ != ods::ValueType::None; }
	bool is_empty() const { return (formula_ == nullptr) && !is_value_set(); }
	
	bool has_children(const inst::IncludingText itx) const override
		{ return nodes_.size() > 0; }
	void ListKeywords(inst::Keywords &words_hash, const inst::LimitTo lt) override;
	void ListUsedNamespaces(inst::NsHash &list) override;
	
	inst::DrawFrame* NewDrawFrame();
	inst::DrawImage* NewDrawFrame(const QString &full_path, QSize *real_size);
	
	ods::Formula* NewFormula();
	inst::StyleStyle* NewStyle();
	ods::CellRef* NewRef();
	 // The row/col arguments are always relative to this cell's position
	ods::CellRef* NewRef(const i32 row, const i32 col);
	
	int number_columns_repeated() const { return ncr_; }
	void number_columns_repeated(const int n) { ncr_ = n; }
	int ncr() const { return ncr_; }
	void ncr(const int n) { ncr_ = n; }
	
	int number_columns_spanned() const { return ncs_; }
	void number_columns_spanned(const int n);
	int ncs() const { return ncs_; }
	void ncs(const int n) { number_columns_spanned(n); }
	
	int number_rows_spanned() const { return nrs_; }
	void number_rows_spanned(const int n) { nrs_ = n; }
	
	QString QueryAddress() const;
	ods::Currency* QueryCurrencyObject();
	// caller must delete returned value
	Length* QueryDesiredHeight() const;
	
	inst::StyleFontFace*
	QueryFontFace(inst::StyleStyle *cell_style = nullptr,
		inst::TableTableColumn *table_column = nullptr) const;
	
	int QueryStart() const;
	void ReadValue(ods::Tag *tag, NdffAttrs *ndff_attrs);
	ods::Row* row() const { return row_; }
	void SetBoolean(const bool flag);
	void SetBooleanFromString(QStringView s);
	void SetCurrency(const Currency &c);
	void SetDate(QDate *p);
	void SetDateTime(QDateTime *p);
	void SetDouble(const double d);
	void SetTime(Time *p);
	void SetFirstString(const QString &s, bool change_value_type = true);
	void SetRowColSpan(int rows, int cols);
	void SetString(const QString &s) { SetFirstString(s); }
	void SetFormula(ods::Formula *p);
	void SetPercentage(const double d);
	void SetStyle(Abstract *a);
	void SetValue(const QString &s) { SetString(s); }
	void SetValue(const i64 n) { SetDouble(double(n)); }
	void SetValue(void *value)
	{
		ClearValue(true);
		value_data_ = value;
	}
	
	void SetValue(void *value, const ods::ValueType kType)
	{
		ClearValue(true);
		value_data_ = value;
		office_value_type_ = kType;
	}
	
	const QString& table_style_name() const { return table_style_name_; }
	QByteArray TypeAndValueString() const;
	QStringView TypeToString() const { return ods::TypeToString(office_value_type_); }
	QString ValueToString() const;
	ods::ValueType value_type() const { return office_value_type_; }
	void value_type_set(const ods::ValueType kType) { office_value_type_ = kType; }
	void WriteData(QXmlStreamWriter &xml) override;
	void WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba) override;
private:
	
	void* CloneValue() const;
	void delete_region(const DeleteRegion &dr) { delete_region_ = dr; }
	const DeleteRegion& delete_region() const { return delete_region_; }
	void Init(ndff::Container *cntr);
	void Init(ods::Tag *tag);
	void Scan(ods::Tag *tag);
	QString GetAttr(ods::Tag *tag, NdffAttrs *attrs, Prefix *prefix, QStringView key);
	bool selected() const { return bits_ & SelectedBit; }
	
	void selected(const bool do_set) {
		if (do_set)
			bits_ |= SelectedBit;
		else
			bits_ &= ~SelectedBit;
	}
	
	void Set(const QString &stype, const QString &value);
	QString ToSchemaString() const;
	void WriteValue(QXmlStreamWriter &xml);
	void WriteValueNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba);
	
	ods::Row *row_ = nullptr;
	void *value_data_ = nullptr;
	ods::ValueType office_value_type_ = ods::ValueType::None;
	
	int ncr_ = 1; // number columns repeated
	int ncs_ = 1; // number columns spanned
	int nrs_ = 1; // number rows spanned
	
	// table:style-name="ce3" office:value-type="percentage" office:value="0.9"
	// office:currency="USD", office:date-value="1983-12-30"
	// office:time-value="PT22H15M00S", office:boolean-value="true"
	ods::Formula *formula_ = nullptr;
	QString table_style_name_;
	QString office_currency_;
	
	u8 bits_ = 0;
	ods::DeleteRegion delete_region_ = {-1, -1, -1};
	friend class Row;
};

} // ods::
