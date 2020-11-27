#include "CellRef.hpp"

#include "Cell.hpp"
#include "Row.hpp"
#include "Sheet.hpp"

namespace ods { // ods::

CellRef::CellRef() {}

CellRef::~CellRef() {}

CellRef*
CellRef::Clone() const
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
	auto *c = row->GetCell(col);
	return c;
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

CellRef*
CellRef::NewRelativeTo(const int r, const int c)
{
	return New(sheet_, row_ + r, col_ + c);
}

void
CellRef::PrintDebug(const char *msg)
{
	auto ba = sheet_->name().toLocal8Bit();
	mtl_info("%s col: %d, row: %d, sheet: %s", msg, col(), row(), ba.data());
}


} // ods::
