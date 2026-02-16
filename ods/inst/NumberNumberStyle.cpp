#include "NumberNumberStyle.hpp"

#include "NumberNumber.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods::inst {

NumberNumberStyle::NumberNumberStyle(ods::inst::Abstract *parent, Tag *tag):
Abstract(parent, parent->ns(), id::NumberNumberStyle)
{
	if (tag)
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
	p->CloneChildrenOf(this);
	
	return p;
}

void NumberNumberStyle::Init(ods::Tag *tag)
{
	tag->Copy(ns_->style(), ns::kName, style_name_);
	Scan(tag);
}

void NumberNumberStyle::ListKeywords(inst::Keywords &list, const inst::LimitTo lt)
{
	inst::AddKeywords({tag_name(), ns::kName}, list);
}

void NumberNumberStyle::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->number(), list);
	Add(ns_->style(), list);
}

void NumberNumberStyle::Scan(ods::Tag *tag)
{
	foreach (auto *x, tag->nodes())
	{
		if (AddText(x))
			continue;
		
		auto *next = x->as_tag();
		
		if (next->Is(ns_->number(), ns::kNumber))
		{
			Append(new NumberNumber(this, next), TakeOwnership::Yes);
		} else {
			Scan(next);
		}
	}
}

void NumberNumberStyle::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->style(), ods::ns::kName, style_name_);
	WriteNodes(xml);
}

} // ods::inst::
