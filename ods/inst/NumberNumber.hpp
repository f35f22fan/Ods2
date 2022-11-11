#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API NumberNumber : public Abstract
{
public:
	NumberNumber(Abstract *parent, ods::Tag *tag = 0, ndff::Container *cntr = 0);
	NumberNumber(const NumberNumber &cloner);
	virtual ~NumberNumber();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	i8 decimal_places() const { return number_decimal_places_; }
	void decimal_places(const i8 n) { number_decimal_places_ = n; }
	
	i8 grouping() const { return number_grouping_; }
	void grouping(const i8 n) { number_grouping_ = n; }
	
	i8 min_integer_digits() const { return number_min_integer_digits_; }
	void min_integer_digits(const i8 n) { number_min_integer_digits_ = n; }
	
	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	void WriteData(QXmlStreamWriter &xml) override;
	void WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba) override;
	
private:
	void Init(ndff::Container *cntr);
	void Init(Tag *tag);
	
	// <number:number number:decimal-places="2" loext:min-decimal-places="2"
	// number:min-integer-digits="1"/>
	
	i8 number_decimal_places_ = -1;
	i8 number_min_integer_digits_ = -1;
	
	// it's a boolean: -1 not set, 0 false, 1 true
	i8 number_grouping_ = -1;
};

} // ods::inst::
