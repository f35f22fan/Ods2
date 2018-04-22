#include "StyleFooterLeft.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

StyleFooterLeft::StyleFooterLeft(Abstract *parent, Tag *tag)
: Abstract(parent, parent->ns(), id::StyleFooterLeft)
{
	if (tag != nullptr)
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
	
	return p;
}

void
StyleFooterLeft::Init(Tag *tag)
{
	tag->Copy(ns_->style(), ods::ns::kDisplay, style_display_);
	ScanString(tag);
}

void
StyleFooterLeft::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->style(), ods::ns::kDisplay, style_display_);
	WriteNodes(xml);
}

} // ods::inst::
} // ods::
