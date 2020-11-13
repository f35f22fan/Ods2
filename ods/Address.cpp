#include "Address.hpp"

#include "Book.hpp"
#include "Cell.hpp"
#include "CellRef.hpp"
#include "ods.hh"
#include "Sheet.hpp"

#include <algorithm> // std::swap(..)

namespace ods {

Address::Address() {}
Address::Address(Sheet *default_sheet) :
	default_sheet_(default_sheet)
{}

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
Address::Cell(Sheet *default_sheet, QStringRef cell)
{
	Address *address = new Address(default_sheet);
	address->cell_ = ods::CreateCellRef(default_sheet, cell);
	return address;
}

QString
Address::CellToString(const ods::CellRef *cell_ref) const
{
	if (cell_ref == nullptr) {
		mtl_trace("CELL_REF = NULLPTR ==========");
		return QString();
	}
	
	ods::Cell *cell = cell_ref->GetCell();
	if (cell == nullptr) {
		mtl_trace();
		return QString();
	}
	ods::Sheet *sheet = cell_ref->sheet();
	if (sheet == nullptr) {
		mtl_trace("SHEET == NULLPTR ============");
		return QString();
	}
	QString s = QLatin1String(".") + cell->QueryAddress();
	
	if (sheet != default_sheet_)
		s = sheet->name() + s;
	
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
	int start, end;
	
	if (cell_->col() == end_cell_->col()) {
		start = cell_->row();
		end = end_cell_->row();
		
		if (start > end)
			std::swap(start, end);
		
		for (int i = start; i <= end; i++) {
			ods::Cell *p = CellRef::FetchCell(sheet, i, cell_->col());
			CHECK_PTR(p);
			cells.append(p);
		}
	} else {
		start = cell_->col();
		end = end_cell_->col();
		
		if (start > end)
			std::swap(start, end);
		
		for (int i = start; i <= end; i++) {
			ods::Cell *p = CellRef::FetchCell(sheet, cell_->row(), i);
			CHECK_PTR(p);
			cells.append(p);
		}
	}
	
	return true;
}

Address*
Address::Range(Sheet *default_sheet, QStringRef start, QStringRef end)
{
	Address *address = new Address(default_sheet);
	address->cell_ = ods::CreateCellRef(default_sheet, start);
	address->end_cell_ = ods::CreateCellRef(default_sheet, end);
	return address;
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
