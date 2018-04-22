#pragma once

#include "../decl.hxx"
#include "../err.hpp"
#include "../ods.hxx"

#include "decl.hxx"

namespace ods { // ods::
namespace formula { // ods::formula::

class CellRef
{
public:
	virtual ~CellRef();

	int
	col() const { return col_; }

	void
	col(const int n) { col_ = n; }
	
	ods::Cell*
	GetCell() const;

	static CellRef*
	New(ods::Sheet *sheet, const int row, const int col);
	
	int
	row() const { return row_; }

	void
	row(const int n) { row_ = n; }

	ods::Sheet*
	sheet() const { return sheet_; }

	void
	sheet(ods::Sheet *s) { sheet_ = s; }

private:
	NO_ASSIGN_COPY_MOVE(CellRef);
	
	CellRef();

	int col_ = -1;
	int row_ = -1;
	ods::Sheet *sheet_ = nullptr;
};

} // ods::formula::
} // ods::

