#include "NumberPercentageStyle.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

#include "NumberNumber.hpp"
#include "NumberText.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

NumberPercentageStyle::NumberPercentageStyle(ods::inst::Abstract *parent,
	ods::Tag *tag)
: Abstract(parent, parent->ns(), id::NumberPercentageStyle)
{
	if (tag != nullptr)
		Init(tag);
}

NumberPercentageStyle::NumberPercentageStyle(const NumberPercentageStyle &cloner)
: Abstract(cloner)
{}

NumberPercentageStyle::~NumberPercentageStyle()
{}

Abstract*
NumberPercentageStyle::Clone(Abstract *parent) const
{
	auto *p = new NumberPercentageStyle(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->style_name_ = style_name_;
	
	return p;
}

inst::NumberNumber*
NumberPercentageStyle::FetchNumberStyle()
{
	auto *p = Get(Id::NumberNumber);
	
	if (p == nullptr)
		return NewNumberStyle();
	
	return (inst::NumberNumber*) p;
}

inst::NumberText*
NumberPercentageStyle::FetchNumberText()
{
	auto *p = Get(Id::NumberText);
	
	if (p == nullptr) 
		return NewNumberText();
		
	return (inst::NumberText*) p;
}

inst::NumberNumber*
NumberPercentageStyle::GetNumberStyle() const
{
	auto *p = Get(Id::NumberNumber);
	
	if (p == nullptr)
		return nullptr;
	
	return (inst::NumberNumber*) p;
}

inst::NumberText*
NumberPercentageStyle::GetNumberText() const
{
	auto *p = Get(Id::NumberText);
	
	if (p == nullptr)
		return nullptr;
	
	return (inst::NumberText*) p;
}

void
NumberPercentageStyle::Init(ods::Tag *tag)
{
	tag->Copy(ns_->style(), ods::ns::kName, style_name_);
	Scan(tag);
}

inst::NumberNumber*
NumberPercentageStyle::NewNumberStyle()
{
	auto *p = new NumberNumber(this);
	Append(p);
	return p;
}

NumberText*
NumberPercentageStyle::NewNumberText()
{
	auto *nt = new NumberText(this);
	Append(nt);
	return nt;
}

void
NumberPercentageStyle::Scan(ods::Tag *tag)
{
	foreach (auto *x, tag->nodes())
	{
		if (AddText(x))
			continue;
		
		auto *next = x->as_tag();
		
		if (next->Is(ns_->number(), ods::ns::kNumber))
		{
			Append(new NumberNumber(this, next));
		} else if (next->Is(ns_->number(), ods::ns::kText)) {
			Append(new NumberText(this, next));
		} else {
			Scan(next);
		}
	}
}

void
NumberPercentageStyle::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->style(), ods::ns::kName, style_name_);
	WriteNodes(xml);
}

} // ods::inst::
} // ods::
