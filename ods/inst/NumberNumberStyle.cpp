#include "NumberNumberStyle.hpp"

#include "NumberNumber.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

NumberNumberStyle::NumberNumberStyle(ods::inst::Abstract *parent, Tag *tag) :
Abstract(parent, parent->ns(), id::NumberNumberStyle)
{
	if (tag != nullptr)
		Init(tag);
}

NumberNumberStyle::NumberNumberStyle(const NumberNumberStyle &cloner)
: Abstract(cloner)
{}

NumberNumberStyle::~NumberNumberStyle()
{}

Abstract*
NumberNumberStyle::Clone(Abstract *parent) const
{
	auto *p = new NumberNumberStyle(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->style_name_ = style_name_;
	
	return p;
}

void
NumberNumberStyle::Init(ods::Tag *tag)
{
	tag->Copy(ns_->style(), ods::ns::kName, style_name_);
	Scan(tag);
}

void
NumberNumberStyle::Scan(ods::Tag *tag)
{
	foreach (auto *x, tag->nodes())
	{
		if (AddText(x))
			continue;
		
		auto *next = x->as_tag();
		
		if (next->Is(ns_->number(), ods::ns::kNumber))
		{
			Append(new NumberNumber(this, next));
		} else {
			Scan(next);
		}
	}
}

void
NumberNumberStyle::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->style(), ods::ns::kName, style_name_);
	
	WriteNodes(xml);
}

} // ods::inst::
} // ods::
