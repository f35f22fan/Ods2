#pragma once

#include "cell.hxx"
#include "decl.hxx"
#include "err.hpp"
#include "global.hxx"

#include "inst/Abstract.hpp"

namespace ods {

class ODS_API Row : public ods::inst::Abstract
{
public:
	Row(ods::Sheet *parent, ods::Tag *row_tag, ndff::Container *cntr = 0);
	Row(Sheet *parent);
	Row(const Row &cloner);
	virtual ~Row();
	
	QVector<ods::Cell*>&
	cells() { return cells_; }
	
	virtual inst::Abstract*
	Clone(inst::Abstract *parent = nullptr) const override;
	
	bool covered() const { return bits_ & ods::CoveredBit; }
	void covered(const bool do_set) {
		if (do_set)
			bits_ |= CoveredBit;
		else
			bits_ &= ~CoveredBit;
	}
	
	ods::Cell*
	GetCell(cint place);
	
	int GetColumnIndex(const ods::Cell *cell) const;
	
	inst::StyleStyle*
	GetStyle() const;
	
	bool has_children(const inst::IncludingText itx) const override {
		return cells_.size() > 0 || Abstract::has_children(itx);
	}
	void ListChildren(QVector<StringOrInst *> &vec, const Recursively r) override;
	void ListKeywords(inst::Keywords &list, const inst::LimitTo lt) override;
	void ListUsedNamespaces(inst::NsHash &list) override;
	
	Cell*
	NewCellAt(cint place, cint ncr = 1, cint ncs = 1);
	
	inst::StyleStyle*
	NewStyle();
	
	int
	num() const { return nrr_; }
	
	void
	num(cint n) { nrr_ = n; }
	
	int
	number_rows_repeated() const { return nrr_; }
	
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
	
	void WriteData(QXmlStreamWriter &xml) override;
	void WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba) override;
private:
	void delete_region(const DeleteRegion &dr) {
		delete_region_ = dr;
	}
	
	const DeleteRegion&
	delete_region() const { return delete_region_; }
	
	bool has_delete_region() const { return delete_region_.start != -1; }
	
	Cell* At(cint place, int &vec_index);
	void DeleteCellRegion(ods::Cell *cell, cint vec_index);
	void MarkDeleteRegion(cint from, cint remaining);
	void MarkCoveredCellsAfter(ods::Cell *cell, cint vec_index);
	void Init(ndff::Container *cntr);
	void Init(ods::Tag *tag);
	void InitDefault();
	void Scan(ods::Tag *tag);
	QString ToSchemaString() const;
	
	bool selected() const { return bits_ & SelectedBit; }
	
	void selected(const bool do_set) {
		if (do_set)
			bits_ |= SelectedBit;
		else
			bits_ &= ~SelectedBit;
	}
	
	QVector<ods::Cell*> cells_;
	ods::Sheet *sheet_ = nullptr;
	int nrr_ = 1;
	QString table_style_name_;
	u8 bits_ = 0;
	ods::DeleteRegion delete_region_ = {-1, -1, -1};
	
	friend class Cell;
	friend class Sheet;
};

} // ods::
