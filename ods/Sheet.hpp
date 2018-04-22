#pragma once

#include "decl.hxx"
#include "err.hpp"
#include "global.hxx"

#include "inst/Abstract.hpp"

namespace ods { // ods::

class ODS_API Sheet : public ods::inst::Abstract
{
public:
	Sheet(inst::Abstract *parent, Tag *sheet_tag);
	Sheet(inst::Abstract *parent);
	Sheet(const Sheet &cloner);
	virtual ~Sheet();
	
	ods::Book*
	book() const { return book_; }
	
	virtual inst::Abstract*
	Clone(inst::Abstract *parent = nullptr) const override;
	
	int
	CountColumns();
	
	int
	CountRows() const;
	
	inst::TableTableColumn*
	GetColumn(const int col_num) const;
	
	inst::StyleStyle*
	GetDefaultCellStyle(const Cell *cell) const;
	
	ods::Row*
	GetRow(const int index);
	
	const QString&
	name() const { return table_name_; }
	
	inst::TableTableColumn*
	NewColumnAt(const int index, const int nrr = 1);
	
	inst::TableTableColumn*
	NewColumnInPlaceOf(const int index, const int nrr = 1);
	
	ods::Row*
	NewRowAt(const int row_index, const int nrr = 1);
	
	ods::Row*
	NewRowInPlaceOf(const int row_index, const int nrr = 1);
	
	int
	num_cols() const { return num_cols_; }
	
	int
	QueryRowStart(const Row *row) const;
	
	bool // returns true if successful
	SetName(const QString &name);
	
	void
	WriteData(QXmlStreamWriter &xml) override;
	
private:
	
	Row* At(const int logical_index, int &starts_at,	int &total_li, int &vec_index);
	inst::TableTableColumn* ColumnAt(const int logical_index, int &starts_at,
		int &total_li, int &vec_index);
	void Curtail(const int by_how_much, const int from_where);
	void CurtailCols(const int by_how_much, const int from_where);
	void Init(ods::Tag *sheet_tag);
	void InitDefault();
	void name(const QString &name) { table_name_ = name; }
	Row* NewRow(const int insert_li, const int nrr, const AddMode mode);
	
	inst::TableTableColumn* NewColumn(const int insert_li, const int num,
		const AddMode mode);
	
	void Scan(ods::Tag *tag);
	
	QString table_name_;
	QString table_style_name_;
	QVector<ods::Row*> rows_;
	QVector<inst::TableTableColumn*> columns_;
	int num_cols_ = 0;
	
	const int DefaultColumnCountPerSheet = 1024;
	const int DefaultRowCountPerSheet = 0x0FFFFF; // 1048575
	
	friend class inst::OfficeSpreadsheet;
};

} // ods::
