#pragma once

#include "decl.hxx"
#include "err.hpp"
#include "global.hxx"

#include "inst/Abstract.hpp"

namespace ods { // ods::

class ODS_API Row : public ods::inst::Abstract
{
public:
	Row(ods::Sheet *parent, ods::Tag *row_tag);
	Row(Sheet *parent);
	Row(const Row &cloner);
	virtual ~Row();
	
	QVector<ods::Cell*>&
	cells() { return cells_; }
	
	virtual inst::Abstract*
	Clone(inst::Abstract *parent = nullptr) const override;
	
	ods::Cell*
	GetCell(const int index);
	
	int
	GetColumnIndex(const ods::Cell *cell) const;
	
	ods::Cell*
	NewCellAt(const int col_index, const int number_columns_repeated = 1);
	
	ods::Cell*
	NewCellInPlaceOf(const int col_index, const int ncr);
	
	int
	num() const { return number_rows_repeated_; }
	
	void
	num(const int n) { number_rows_repeated_ = n; }
	
	int
	number_rows_repeated() const { return number_rows_repeated_; }
	
	int
	number_rows_spanned() const { return num_rows_spanned_; }
	
	int
	QueryCellStart(const Cell *cell) const;
	
	Length*
	QueryOptimalHeight() const;
	
	int
	QueryStart() const;
	
	void
	SetStyle(const ods::Row *p);
	
	void
	SetStyle(inst::StyleStyle *p);
	
	ods::Sheet*
	sheet() const { return sheet_; }
	
	void
	WriteData(QXmlStreamWriter &xml) override;
	
private:
	
	Cell* At(const int logical_index, int &starts_at, int &total_li, int &vec_index);
	void Curtail(const int by_how_much, const int from_where);
	void Init(ods::Tag *tag);
	void InitDefault();
	Cell* NewCell(const int insert_li, const int nrr, const AddMode mode);
	void Scan(ods::Tag *tag);
	void SetCellAt(ods::Cell *new_cell, const int insert_at);
	
	QVector<ods::Cell*> cells_;
	ods::Sheet *sheet_ = nullptr;
	int number_rows_repeated_ = 1;
	int num_rows_spanned_ = 1;
	QString table_style_name_;
};

} // ods::
