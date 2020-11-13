#pragma once

#include "decl.hxx"
#include "err.hpp"
#include "types.hxx"

#include <QString>

namespace ods {

class Address {
public:
	virtual ~Address();
	Address(const Address &c);
	Address* Clone();
	
	ods::CellRef* cell() const { return cell_; }
	ods::CellRef* end_cell() const { return end_cell_; }
	
	static Address*
	Cell(ods::Sheet *default_sheet, QStringRef cell);
	
	bool
	GenCells(QVector<ods::Cell*> &cells);
	
	static Address
	Invalid() { return Address(nullptr); }
	
	bool is_cell_range() const { return end_cell_ != nullptr; }
	
	static Address*
	Range(ods::Sheet *default_sheet, QStringRef start, QStringRef end);
	
	bool valid() const { return cell_ != nullptr; }
	
	QString toString() const;
	
private:
	Address();
	Address(ods::Sheet *default_sheet);
	QString CellToString(const CellRef *cell_ref) const;
	static void DeepCopy(Address &dest, const Address &src);
	ods::Sheet *default_sheet_ = nullptr;
	ods::CellRef *cell_ = nullptr;
	ods::CellRef *end_cell_ = nullptr;
	int start_index_ = -1, end_index_ = -1;
};


}
