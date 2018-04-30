#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

class ODS_API TableTableColumn : public Abstract
{
public:
	TableTableColumn(Abstract *parent, Tag *tag = nullptr);
	TableTableColumn(const TableTableColumn &cloner);
	virtual ~TableTableColumn();
	
	virtual inst::Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	inst::StyleTableColumnProperties*
	FetchTableColumnProperties();
	
	StyleStyle*
	GetDefaultCellStyle() const;
	
	inst::StyleStyle*
	GetStyle() const;
	
	int
	num() const { return table_number_columns_repeated_; }
	
	void
	num(const int n) { table_number_columns_repeated_ = n; }
	
	int
	number_columns_repeated() const { return table_number_columns_repeated_; }
	
	int
	number_columns_repeated(const int n)
	{
		table_number_columns_repeated_ = n;
	}

	Length* // caller must not delete returned value
	QueryColumnWidth() const;
	
	void
	SetStyle(inst::StyleStyle *style);
	
	void
	SetWidth(Length *length);
	
	void
	WriteData(QXmlStreamWriter &xml) override;
	
private:
	
	void Init(Tag *tag);
	
	QString table_style_name_;
	int table_number_columns_repeated_ = 1;
	QString table_default_cell_style_name_;
};

} // ods::inst::
} // ods::
