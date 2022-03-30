#pragma once

#include "decl.hxx"
#include "err.hpp"
#include "ods.hxx"

namespace ods { // ods::

class ODS_API CellRef
{
public:
	virtual ~CellRef();
	
	CellRef* Clone() const;
	
	int
	col() const { return col_; }

	void
	col(const int n) { col_ = n; }
	
	static ods::Cell*
	FetchCell(ods::Sheet *sheet, const int row, const int col);
	
	ods::Cell*
	GetCell() const { return FetchCell(sheet_, row_, col_); }

	static CellRef*
	New(ods::Sheet *sheet, const int row, const int col);
	
	CellRef*
	NewRelativeTo(const int r, const int c);
	
	int
	row() const { return row_; }

	void
	row(const int n) { row_ = n; }

	ods::Sheet*
	sheet() const { return sheet_; }

	void
	sheet(ods::Sheet *s) { sheet_ = s; }
	
	void PrintDebug(const char *msg);

private:
	NO_ASSIGN_COPY_MOVE(CellRef);
	
	CellRef();

	int col_ = -1;
	int row_ = -1;
	ods::Sheet *sheet_ = nullptr;
};

} // ods::

