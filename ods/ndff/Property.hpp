#pragma once

#include "decl.hxx"
#include "../decl.hxx"
#include "../inst/decl.hxx"

#include <QString>

namespace ods::ndff {

cu8 EmptyBit = 1 << 0;
cu8 StringBit = 1 << 1;

class Property {
public:
	
	QString value;
	QString name;
	i32 name_id = -1;
	UriId uri_id = 0;
	
	bool is(ods::Prefix *prefix) const;
	
	bool empty() const { return bits_ & EmptyBit; }
	void empty(cbool b) { if (b) bits_ |= EmptyBit; else bits_ &= ~EmptyBit; }
	
	bool string() const { return bits_ & StringBit; }
	void string(cbool b) { if (b) bits_ |= StringBit; else bits_ &= ~StringBit; }
private:
	u8 bits_ = 0;
};


}
