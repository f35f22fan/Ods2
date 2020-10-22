#include "NumberNumber.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../str.hxx"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

NumberNumber::NumberNumber(Abstract *parent, Tag *tag) :
Abstract(parent, parent->ns(), id::NumberNumber)
{
	if (tag != nullptr)
		Init(tag);
}

NumberNumber::NumberNumber(const NumberNumber &cloner)
: Abstract(cloner)
{}

NumberNumber::~NumberNumber() {}

Abstract*
NumberNumber::Clone(Abstract *parent) const
{
	auto *p = new NumberNumber(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	//p->loext_min_decimal_places_ = loext_min_decimal_places_;
	p->number_decimal_places_ = number_decimal_places_;
	p->number_min_integer_digits_ = number_min_integer_digits_;
	p->number_grouping_ = number_grouping_;

	return p;
}

void
NumberNumber::Init(ods::Tag *tag)
{
	//tag->Copy(ns_->loext(), ods::ns::kMinDecimalPlaces, loext_min_decimal_places_);
	tag->Copy(ns_->number(), ods::ns::kDecimalPlaces, number_decimal_places_);
	tag->Copy(ns_->number(), ods::ns::kMinIntegerDigits, number_min_integer_digits_);
	
	QString str;
	tag->Copy(ns_->number(), ods::ns::kGrouping, str);
	
	if (!str.isEmpty())
		number_grouping_ = (str == ods::str::True) ? 1 : 0;
	
	ScanString(tag);
}

void
NumberNumber::WriteData(QXmlStreamWriter &xml)
{
//	if (loext_min_decimal_places_ != -1)
//		Write(xml, ns_->loext(), ods::ns::kMinDecimalPlaces,
//			QString::number(loext_min_decimal_places_));
	
	if (number_decimal_places_ != -1)
		Write(xml, ns_->number(), ods::ns::kDecimalPlaces,
			QString::number(number_decimal_places_));
	
	if (number_min_integer_digits_ != -1)
		Write(xml, ns_->number(), ods::ns::kMinIntegerDigits,
			QString::number(number_min_integer_digits_));
	
	if (number_grouping_ != -1)
	{
		Write(xml, ns_->number(), ods::ns::kGrouping,
			(number_grouping_ == 1) ? ods::str::True : ods::str::False);
	}
	
	WriteNodes(xml);
}

} // ods::inst::
} // ods::
