#include "CellRef.hpp"

#include "Cell.hpp"
#include "Row.hpp"
#include "Sheet.hpp"

namespace ods { // ods::

CellRef::CellRef() {}

CellRef::~CellRef() {}

CellRef*
CellRef::Clone()
{
	auto *p = new CellRef();
	p->row_ = row_;
	p->col_ = col_;
	p->sheet_ = sheet_;
	
	return p;
}

ods::Cell*
CellRef::FetchCell(ods::Sheet *sheet, const int row_index, const int col)
{
	CHECK_PTR_NULL(sheet);
	ods::Row *row = sheet->GetRow(row_index);
	CHECK_PTR_NULL(row);
	
	return row->GetCell(col);
}

CellRef*
CellRef::New(ods::Sheet *sheet, const int row, const int col)
{
	auto *p = new CellRef();
	p->sheet(sheet);
	p->row(row);
	p->col(col);
	
	return p;
}

} // ods::
