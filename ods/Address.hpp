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
	
	bool cell_range() const { return end_cell_ != nullptr; }
	
	static Address*
	Cell(ods::Sheet *default_sheet, QStringRef cell);
	
	static Address
	Invalid() { return Address(nullptr); }
	
	static Address*
	Range(ods::Sheet *default_sheet, QStringRef start, QStringRef end);
	
	bool valid() const { return cell_ != nullptr; }
	
	QString toString() const;
	
private:
	Address();
	Address(ods::Sheet *default_sheet);
	static void DeepCopy(Address &dest, const Address &src);
	QString orig_str_;
	ods::Sheet *default_sheet_ = nullptr;
	ods::CellRef *cell_ = nullptr;
	ods::CellRef *end_cell_ = nullptr;
	int start_index_ = -1, end_index_ = -1;
};


}
