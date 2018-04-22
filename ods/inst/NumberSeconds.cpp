#include "NumberSeconds.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

NumberSeconds::NumberSeconds(Abstract *parent, ods::Tag *tag)
: Abstract(parent, parent->ns(), id::NumberSeconds)
{
	if (tag != nullptr)
		Init(tag);
}

NumberSeconds::NumberSeconds(const NumberSeconds &cloner)
: Abstract(cloner)
{}

NumberSeconds::~NumberSeconds() {}

Abstract*
NumberSeconds::Clone(Abstract *parent) const
{
	auto *p = new NumberSeconds(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->number_style_ = number_style_;
	p->number_decimal_places_ = number_decimal_places_;
	
	return p;
}

void
NumberSeconds::Init(ods::Tag *tag)
{
	tag->Copy(ns_->number(), ods::ns::kStyle, number_style_);
	tag->Copy(ns_->number(), ods::ns::kDecimalPlaces, number_decimal_places_);
	ScanString(tag);
}

void
NumberSeconds::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->number(), ods::ns::kStyle, number_style_);
	
	if (number_decimal_places_ != -1)
	{
		Write(xml, ns_->number(), ods::ns::kDecimalPlaces,
			QString::number(number_decimal_places_));
	}
	
	WriteNodes(xml);
}

} // ods::inst::
} // ods::
