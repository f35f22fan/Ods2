#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

class ODS_API NumberNumber : public Abstract
{
public:
	NumberNumber(Abstract *parent, ods::Tag *tag = nullptr);
	NumberNumber(const NumberNumber &cloner);
	virtual ~NumberNumber();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	int8_t
	decimal_places() const { return number_decimal_places_; }
	
	void
	decimal_places(const int8_t n) { number_decimal_places_ = n; }
	
	int8_t
	grouping() const { return number_grouping_; }
	
	void
	grouping(const int8_t n) { number_grouping_ = n; }
	
	int8_t
	min_decimal_places() const { return loext_min_decimal_places_; }
	
	void
	min_decimal_places(const int8_t n) { loext_min_decimal_places_ = n; }

	int8_t
	min_integer_digits() const { return number_min_integer_digits_; }
	
	void
	min_integer_digits(const int8_t n) { number_min_integer_digits_ = n; }
	
	void
	WriteData(QXmlStreamWriter &xml) override;
	
private:
	
	void Init(ods::Tag *tag);
	
	// <number:number number:decimal-places="2" loext:min-decimal-places="2"
	// number:min-integer-digits="1"/>
	
	int8_t loext_min_decimal_places_ = -1;
	int8_t number_decimal_places_ = -1;
	int8_t number_min_integer_digits_ = -1;
	
	// really a boolean, -1 not set, 0 false, 1 true
	int8_t number_grouping_ = -1;
};

} // ods::inst::
} // ods::
