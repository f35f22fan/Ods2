#include "StyleFooterLeft.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods::inst {

StyleFooterLeft::StyleFooterLeft(Abstract *parent, Tag *tag)
: Abstract(parent, parent->ns(), id::StyleFooterLeft)
{
	if (tag)
		Init(tag);
}

StyleFooterLeft::StyleFooterLeft(const StyleFooterLeft &cloner)
: Abstract(cloner)
{}

StyleFooterLeft::~StyleFooterLeft() {}

Abstract*
StyleFooterLeft::Clone(Abstract *parent) const
{
	auto *p = new StyleFooterLeft(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->style_display_ = style_display_;
	p->CloneChildrenOf(this);
	
	return p;
}

void StyleFooterLeft::Init(Tag *tag)
{
	tag->Copy(ns_->style(), ns::kDisplay, style_display_);
	ReadStrings(tag);
}

void StyleFooterLeft::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name(), ns::kDisplay}, list);
}

void StyleFooterLeft::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->style(), list);
}

void StyleFooterLeft::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->style(), ns::kDisplay, style_display_);
	WriteNodes(xml);
}

} // ods::inst::
