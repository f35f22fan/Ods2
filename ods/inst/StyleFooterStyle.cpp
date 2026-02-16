#include "StyleFooterStyle.hpp"

#include "StyleHeaderFooterProperties.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods::inst {

StyleFooterStyle::StyleFooterStyle(Abstract *parent, Tag *tag)
: Abstract(parent, parent->ns(), id::StyleFooterStyle)
{
	if (tag)
		Init(tag);
}

StyleFooterStyle::StyleFooterStyle(const StyleFooterStyle &cloner)
: Abstract(cloner)
{}

StyleFooterStyle::~StyleFooterStyle()
{}

Abstract*
StyleFooterStyle::Clone(Abstract *parent) const
{
	auto *p = new StyleFooterStyle(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->CloneChildrenOf(this);
	
	return p;
}

void StyleFooterStyle::Init(ods::Tag *tag)
{
	Scan(tag);
}

void StyleFooterStyle::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name()}, list);
}

void StyleFooterStyle::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->style(), list);
}

void StyleFooterStyle::Scan(ods::Tag *tag)
{
	foreach (auto *x, tag->nodes())
	{
		if (AddText(x))
			continue;
		
		auto *next = x->as_tag();
		
		if (next->Is(ns_->style(), ns::kHeaderFooterProperties)) {
			Append(new StyleHeaderFooterProperties(this, next), TakeOwnership::Yes);
		} else {
			Scan(next);
		}
	}
}

void StyleFooterStyle::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::

