#include "StyleRegionRight.hpp"

#include "TextP.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods::inst {

StyleRegionRight::StyleRegionRight(Abstract *parent, Tag *tag)
: Abstract (parent, parent->ns(), id::StyleRegionRight)
{
	if (tag)
		Init(tag);
}

StyleRegionRight::StyleRegionRight(const StyleRegionRight &cloner)
: Abstract(cloner)
{}

StyleRegionRight::~StyleRegionRight()
{}

Abstract*
StyleRegionRight::Clone(Abstract *parent) const
{
	auto *p = new StyleRegionRight(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->CloneChildrenOf(this);
	
	return p;
}

void StyleRegionRight::Init(Tag *tag)
{
	Scan(tag);
}

void StyleRegionRight::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name()}, list);
}

void StyleRegionRight::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->style(), list);
}

void StyleRegionRight::Scan(ods::Tag *tag)
{
	for (auto *x: tag->nodes())
	{
		if (AddText(x))
			continue;
		
		auto *next = x->as_tag();
		
		if (next->Is(ns_->text(), ods::ns::kP)) {
			Append(new TextP(this, next), TakeOwnership::Yes);
		} else {
			Scan(next);
		}
	}
}

void StyleRegionRight::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
