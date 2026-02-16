#include "StyleHeaderStyle.hpp"

#include "StyleHeaderFooterProperties.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods::inst {

StyleHeaderStyle::StyleHeaderStyle(Abstract *parent, ods::Tag *tag)
: Abstract(parent, parent->ns(), id::StyleHeaderStyle)
{
	if (tag)
		Init(tag);
}

StyleHeaderStyle::StyleHeaderStyle(const StyleHeaderStyle &cloner)
: Abstract(cloner)
{}

StyleHeaderStyle::~StyleHeaderStyle()
{}

Abstract*
StyleHeaderStyle::Clone(Abstract *parent) const
{
	auto *p = new StyleHeaderStyle(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->style_name_ = style_name_;
	p->CloneChildrenOf(this);
	
	return p;
}

void StyleHeaderStyle::Init(ods::Tag *tag)
{
	tag->Copy(ns_->style(), ns::kName, style_name_);
	Scan(tag);
}

void StyleHeaderStyle::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name(), ns::kName}, list);
}

void StyleHeaderStyle::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->number(), list);
}

void StyleHeaderStyle::Scan(ods::Tag *tag)
{
	foreach (auto *x, tag->nodes())
	{
		if (AddText(x))
			continue;
		
		auto *next = x->as_tag();
		
		if (next->Is(ns_->style(), ns::kHeaderFooterProperties))
		{
			Append(new StyleHeaderFooterProperties(this, next), TakeOwnership::Yes);
		} else {
			Scan(next);
		}
	}
}

void StyleHeaderStyle::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->style(), ns::kName, style_name_);
	WriteNodes(xml);
}

} // ods::inst::
