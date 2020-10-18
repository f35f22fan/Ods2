#pragma once

#include "cell.hxx"
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
	
	bool
	covered() const { return bits_ & ods::CoveredBit; }
	
	void
	covered(const bool do_set) {
		if (do_set)
			bits_ |= CoveredBit;
		else
			bits_ &= ~CoveredBit;
	}
	
	void delete_region(const DeleteRegion &dr) {
		delete_region_ = dr;
	}
	
	const DeleteRegion&
	delete_region() const { return delete_region_; }
	
	ods::Cell*
	GetCell(const int place);
	
	int
	GetColumnIndex(const ods::Cell *cell) const;
	
	inst::StyleStyle*
	GetStyle() const;
	
	bool
	has_delete_region() const { return delete_region_.start != -1; }
	
	Cell*
	NewCellAt(const int place, const int ncr = 1, const int ncs = 1);
	
	inst::StyleStyle*
	NewStyle();
	
	int
	num() const { return nrr_; }
	
	void
	num(const int n) { nrr_ = n; }
	
	int
	number_rows_repeated() const { return nrr_; }
	
	void Print() const;
	
	int
	QueryCellStart(const Cell *cell) const;
	
	Length*
	QueryOptimalHeight() const;
	
	int
	QueryStart() const;
	
	bool selected() const { return bits_ & SelectedBit; }
	
	void selected(const bool do_set) {
		if (do_set)
			bits_ |= SelectedBit;
		else
			bits_ &= ~SelectedBit;
	}
	
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
	void DeleteCellRegion(ods::Cell *cell, const int vec_index);
	void MarkDeleteRegion(int from, int remaining);
	void MarkCoveredCellsAfter(ods::Cell *cell, const int vec_index);
	void Init(ods::Tag *tag);
	void InitDefault();
	void Scan(ods::Tag *tag);
	QString ToSchemaString() const;
	
	QVector<ods::Cell*> cells_;
	ods::Sheet *sheet_ = nullptr;
	int nrr_ = 1;
	QString table_style_name_;
	u8 bits_ = 0;
	ods::DeleteRegion delete_region_ = {-1, -1, -1};
	
	friend class Cell;
};

} // ods::
