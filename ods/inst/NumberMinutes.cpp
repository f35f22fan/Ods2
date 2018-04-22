#include "NumberMinutes.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

NumberMinutes::NumberMinutes(Abstract *parent, Tag *tag) :
Abstract(parent, parent->ns(), id::NumberMinutes)
{
	if (tag != nullptr)
		Init(tag);
}

NumberMinutes::NumberMinutes(const NumberMinutes &cloner)
: Abstract(cloner)
{}

NumberMinutes::~NumberMinutes() {}

Abstract*
NumberMinutes::Clone(Abstract *parent) const
{
	auto *p = new NumberMinutes(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->number_style_ = number_style_;
	return p;
}

void
NumberMinutes::Init(ods::Tag *tag)
{
	tag->Copy(ns_->number(), ods::ns::kStyle, number_style_);
	ScanString(tag);
}

void
NumberMinutes::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->number(), ods::ns::kStyle, number_style_);
	WriteNodes(xml);
}

} // ods::inst::
} // ods::
