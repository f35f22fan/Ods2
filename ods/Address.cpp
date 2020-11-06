#include "Address.hpp"

#include "Book.hpp"
#include "CellRef.hpp"
#include "ods.hh"
#include "Sheet.hpp"

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
	dest.orig_str_ = src.orig_str_;
	
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
	address->orig_str_ = cell.toString();
	return address;
}

Address*
Address::Clone()
{
	auto *p = new Address();
	DeepCopy(*p, *this);
	
	return p;
}

Address*
Address::Range(Sheet *default_sheet, QStringRef start, QStringRef end)
{
	Address *address = new Address(default_sheet);
	address->cell_ = ods::CreateCellRef(default_sheet, start);
	address->end_cell_ = ods::CreateCellRef(default_sheet, end);
	address->orig_str_ = start.toString() + QChar(':') + end.toString();
	return address;
}

QString
Address::toString() const
{
	return orig_str_;
}

}
