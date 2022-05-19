#pragma once

#include "decl.hxx"
#include "err.hpp"
#include "global.hxx"
#include "types.hxx"

#include <QString>

namespace ods {

namespace reference {
	const u8 RelativeBit = 1u << 0;
	const u8 R1C1Bit = 1u << 1;
}

class ODS_API Reference {
public:
	virtual ~Reference();
	Reference(const Reference &c);
	Reference* Clone() const;
	
	ods::CellRef* cell();
	ods::CellRef* end_cell() const { return end_cell_ref_; }
	void end_cell(ods::CellRef *cr) { end_cell_ref_ = cr; }
	
	static Reference*
	Cell(ods::Sheet *default_sheet, CellRef *ref);

	static Reference*
	Cell(ods::Sheet *default_sheet, QStringView cell_str);

	static Reference*
	CellRange(Sheet *default_sheet, CellRef *start, CellRef *end);

	static Reference*
	CellRange(ods::Sheet *default_sheet, QStringView start, QStringView end);

	static Reference*
	From(QStringView str, Sheet *default_sheet);

	static Reference*
	R1C1From(QStringView str, Formula *formula);

	bool
	GenCells(QVector<ods::Cell*> &cells);
	
	bool is_cell_range() const { return end_cell_ref_ != nullptr && !r1c1(); }
	
	Reference*
	Offset(int row_off, int col_off, int new_h, int new_w) const;
	
	void PrintR1C1() const;
	
	bool relative() const { return bits_ & reference::RelativeBit; }
	
	void relative(const bool flag) {
		if (flag)
			bits_ |= reference::RelativeBit;
		else
			bits_ &= ~reference::RelativeBit;
	}
	
	bool r1c1() const { return bits_ & reference::R1C1Bit; }
	
	void r1c1(const bool flag) {
		if (flag)
			bits_ |= reference::R1C1Bit;
		else
			bits_ &= ~reference::R1C1Bit;
	}
	
	QString toString() const;
	
private:
	Reference();
	QString CellToString(const CellRef *cell_ref) const;
	static void DeepCopy(Reference &dest, const Reference &src);
	ods::Sheet *default_sheet_ = nullptr;
	
	/* In a non-R1C1 reference cell_ref_ and end_cell_ref_ are used as implied.
	   Otherwise cell_ref_ is the anchor refcell and end_cell_ref_ is the
	final computed and cached refcell that r_ and c_ refer to. */
	ods::CellRef *cell_ref_ = nullptr;
	ods::CellRef *end_cell_ref_ = nullptr;
	
	i32 r_ = -1, c_ = -1;
	u8 bits_ = 0;
};


}
