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
CellRef::GetCell() const
{
	if (sheet_ == nullptr)
	{
		mtl_trace();
		return nullptr;
	}
	
	ods::Row *row = sheet_->GetRow(row_);
	
	if (row == nullptr) {
		mtl_trace();
		return nullptr;
	}
	
	return row->GetCell(col_);
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
