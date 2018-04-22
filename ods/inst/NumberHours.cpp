#include "NumberHours.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

NumberHours::NumberHours(Abstract *parent, Tag *tag)
: Abstract(parent, parent->ns(), id::NumberHours)
{
	if (tag != nullptr)
		Init(tag);
}

NumberHours::NumberHours(const NumberHours &cloner)
: Abstract(cloner)
{}

NumberHours::~NumberHours() {}

Abstract*
NumberHours::Clone(Abstract *parent) const
{
	auto *p = new NumberHours(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->number_style_ = number_style_;
	
	return p;
}

void
NumberHours::Init(ods::Tag *tag)
{
	tag->Copy(ns_->number(), ods::ns::kStyle, number_style_);
	ScanString(tag);
}

void
NumberHours::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->number(), ods::ns::kStyle, number_style_);
	WriteNodes(xml);
}

} // ods::inst::
} // ods::
