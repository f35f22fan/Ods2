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
	GetCell(const int place);
	
	int
	GetColumnIndex(const ods::Cell *cell) const;
	
	inst::StyleStyle*
	GetStyle() const;
	
	Cell*
	NewCellAt(const int place, const int ncr = 1, const int ncs = 1);
	
	inst::StyleStyle*
	NewStyle();
	
	int
	num() const { return number_rows_repeated_; }
	
	void
	num(const int n) { number_rows_repeated_ = n; }
	
	int
	number_rows_repeated() const { return number_rows_repeated_; }
	
	int
	number_rows_spanned() const { return num_rows_spanned_; }
	
	void Print() const;
	
	int
	QueryCellStart(const Cell *cell) const;
	
	Length*
	QueryOptimalHeight() const;
	
	int
	QueryStart() const;
	
	void
	SetOptimalHeight();
	
	void
	SetStyle(const ods::Row *p);
	
	void
	SetStyle(inst::StyleStyle *p);
	
	ods::Sheet*
	sheet() const { return sheet_; }
	
	void
	WriteData(QXmlStreamWriter &xml) override;
	
private:
	
	Cell* At(const int place, int &vec_index);
	void DeleteRegion(ods::Cell *cell, const int vec_index);
	void MarkDeleteRegion(int from, int remaining);
	void MarkCoveredCellsAfter(ods::Cell *cell, const int vec_index);
	void Init(ods::Tag *tag);
	void InitDefault();
	void Scan(ods::Tag *tag);
	
	QVector<ods::Cell*> cells_;
	ods::Sheet *sheet_ = nullptr;
	int number_rows_repeated_ = 1;
	int num_rows_spanned_ = 1;
	QString table_style_name_;
	
	friend class Cell;
};

} // ods::
