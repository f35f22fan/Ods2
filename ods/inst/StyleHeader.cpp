#include "StyleHeader.hpp"

#include "StyleRegionLeft.hpp"
#include "StyleRegionRight.hpp"
#include "TextP.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods::inst {

StyleHeader::StyleHeader(Abstract *parent, Tag *tag)
: Abstract(parent, parent->ns(), id::StyleHeader)
{
	if (tag != nullptr)
		Init(tag);
}

StyleHeader::StyleHeader(const StyleHeader &cloner)
: Abstract(cloner)
{}

StyleHeader::~StyleHeader()
{}

Abstract*
StyleHeader::Clone(Abstract *parent) const
{
	auto *p = new StyleHeader(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->CloneChildrenOf(this);
	
	return p;
}

void StyleHeader::Init(Tag *tag)
{
	Scan(tag);
}

void StyleHeader::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name()}, list);
}

void StyleHeader::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->style(), list);
}

void StyleHeader::Scan(Tag *tag)
{
	for (auto *x: tag->nodes())
	{
		if (AddText(x))
			continue;
		
		auto *next = x->as_tag();
		
		if (next->Is(ns_->style(), ods::ns::kRegionLeft)) {
			Append(new StyleRegionLeft(this, next), TakeOwnership::Yes);
		} else if (next->Is(ns_->style(), ods::ns::kRegionRight)) {
			Append(new StyleRegionRight(this, next), TakeOwnership::Yes);
		} else if (next->Is(ns_->text(), ods::ns::kP)) {
			Append(new TextP(this, next), TakeOwnership::Yes);
		} else {
			Scan(next);
		}
	}
}

void
StyleHeader::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
