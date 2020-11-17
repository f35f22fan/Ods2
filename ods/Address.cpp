#include "Address.hpp"

#include "Book.hpp"
#include "Cell.hpp"
#include "CellRef.hpp"
#include "ods.hh"
#include "Sheet.hpp"

#include <algorithm> // std::swap(..)

namespace ods {

Address::Address() {}

Address::~Address() {
	delete cell_;
	delete end_cell_;
	cell_ = end_cell_ = nullptr;
}

void
Address::DeepCopy(Address &dest, const Address &src) {
	dest.default_sheet_ = src.default_sheet_;
	dest.start_index_ = src.start_index_;
	dest.end_index_ = src.end_index_;
	
	if (src.cell_ != nullptr)
		dest.cell_ = src.cell_->Clone();
	
	if (src.end_cell_ != nullptr)
		dest.end_cell_ = src.end_cell_->Clone();
}

Address::Address(const Address &c)
{
	DeepCopy(*this, c);
}

Address*
Address::Cell(Sheet *default_sheet, CellRef *ref)
{
	Address *address = new Address();
	address->default_sheet_ = default_sheet;
	address->cell_ = ref;
	return address;
}

Address*
Address::Cell(Sheet *default_sheet, QStringRef cell)
{
	Address *address = new Address();
	address->default_sheet_ = default_sheet;
	address->cell_ = ods::CreateCellRef(default_sheet, cell);
	return address;
}

Address*
Address::CellRange(Sheet *default_sheet, CellRef *start, CellRef *end)
{
	Address *address = new Address();
	address->default_sheet_ = default_sheet;
	address->cell_ = start;
	address->end_cell_ = end;
	return address;
}

Address*
Address::CellRange(Sheet *default_sheet, QStringRef start, QStringRef end)
{
	Address *address = new Address();
	address->default_sheet_ = default_sheet;
	address->cell_ = ods::CreateCellRef(default_sheet, start);
	address->end_cell_ = ods::CreateCellRef(default_sheet, end);
	return address;
}

QString
Address::CellToString(const ods::CellRef *cell_ref) const
{
	CHECK_TRUE_QSTR((cell_ref != nullptr));
	ods::Cell *cell = cell_ref->GetCell();
	CHECK_TRUE_QSTR((cell != nullptr));
	ods::Sheet *sheet = cell_ref->sheet();
	CHECK_TRUE_QSTR((sheet != nullptr));
	QString s = QLatin1String(".") + cell->QueryAddress();
	
	if (sheet != default_sheet_) {
		s = QChar('\'') + sheet->name() + QChar('\'') + s;
	}
	
	return s;
}

Address*
Address::Clone()
{
	auto *p = new Address();
	DeepCopy(*p, *this);
	
	return p;
}

bool
Address::GenCells(QVector<ods::Cell*> &cells)
{
	CHECK_PTR(end_cell_);
	ods::Sheet *sheet = cell_->sheet();
	
	const int col_start = std::min(cell_->col(), end_cell_->col());
	const int col_end = std::max(cell_->col(), end_cell_->col());
	
	const int row_start = std::min(cell_->row(), end_cell_->row());
	const int row_end = std::max(cell_->row(), end_cell_->row());
	
	for (int r = row_start; r <= row_end; r++) {
		for (int c = col_start; c <= col_end; c++) {
			ods::Cell *p = CellRef::FetchCell(sheet, r, c);
			CHECK_PTR(p);
			cells.append(p);
		}
	}
	
	return true;
}

QString
Address::toString() const
{
	QString s = "[";
	s.append(CellToString(cell_));
	if (is_cell_range()) {
		s.append(':');
		s.append(CellToString(end_cell_));
	}
	s.append(']');
	
	return s;
}

}
