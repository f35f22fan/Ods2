#include "NumberMonth.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

NumberMonth::NumberMonth(Abstract *parent, Tag *tag) :
Abstract(parent, parent->ns(), id::NumberMonth)
{
	if (tag != nullptr)
		Init(tag);
}

NumberMonth::NumberMonth(const NumberMonth &cloner)
: Abstract(cloner)
{}

NumberMonth::~NumberMonth() {}

Abstract*
NumberMonth::Clone(Abstract *parent) const
{
	auto *p = new NumberMonth(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->number_style_ = number_style_;
	
	return p;
}

void
NumberMonth::Init(ods::Tag *tag)
{
	tag->Copy(ns_->number(), ods::ns::kStyle, number_style_);
	ScanString(tag);
}

void
NumberMonth::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->number(), ods::ns::kStyle, number_style_);
	WriteNodes(xml);
}

} // ods::inst::
} // ods::
