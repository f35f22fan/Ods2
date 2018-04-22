#include "NumberYear.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

NumberYear::NumberYear(Abstract *parent, ods::Tag *tag)
: Abstract(parent, parent->ns(), id::NumberYear)
{
	if (tag != nullptr)
		Init(tag);
}

NumberYear::NumberYear(const NumberYear &cloner)
: Abstract(cloner)
{}

NumberYear::~NumberYear() {}

Abstract*
NumberYear::Clone(Abstract *parent) const
{
	auto *p = new NumberYear(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->number_style_ = number_style_;
	
	return p;
}

void
NumberYear::Init(ods::Tag *tag)
{
	tag->Copy(ns_->number(), ods::ns::kStyle, number_style_);
	ScanString(tag);
}

void
NumberYear::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->number(), ods::ns::kStyle, number_style_);
	WriteNodes(xml);
}

} // ods::inst::
} // ods::
