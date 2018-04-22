#include "NumberDay.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

NumberDay::NumberDay(ods::inst::Abstract *parent, ods::Tag *tag)
 : Abstract(parent, parent->ns(), id::NumberDay)
{
	if (tag != nullptr)
		Init(tag);
}

NumberDay::NumberDay(const NumberDay &cloner)
: Abstract(cloner)
{}

NumberDay::~NumberDay() {}

Abstract*
NumberDay::Clone(Abstract *parent) const
{
	auto *p = new NumberDay(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->number_style_ = number_style_;
	
	return p;
}

void
NumberDay::Init(ods::Tag *tag)
{
	tag->Copy(ns_->number(), ods::ns::kStyle, number_style_);
	
	ScanString(tag);
}

void
NumberDay::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->number(), ods::ns::kStyle, number_style_);
	
	WriteNodes(xml);
}

} // ods::inst::
} // ods::
