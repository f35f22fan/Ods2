#include "CellRef.hpp"

#include "../Cell.hpp"
#include "../Row.hpp"
#include "../Sheet.hpp"

namespace ods { // ods::
namespace formula { // ods::formula::

CellRef::CellRef() {}

CellRef::~CellRef() {}

ods::Cell*
CellRef::GetCell() const
{
	if (sheet_ == nullptr)
	{
		mtl_warn();
		return nullptr;
	}
	
	ods::Row *row = sheet_->GetRow(row_);
	
	if (row == nullptr)
		return nullptr;
	
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

} // ods::formula::
} // ods::
