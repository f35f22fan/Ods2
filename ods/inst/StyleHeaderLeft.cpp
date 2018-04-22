#include "StyleHeaderLeft.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

StyleHeaderLeft::StyleHeaderLeft(Abstract *parent, Tag *tag)
: Abstract(parent, parent->ns(), id::StyleHeaderLeft)
{
	if (tag != nullptr)
		Init(tag);
}

StyleHeaderLeft::StyleHeaderLeft(const StyleHeaderLeft &cloner)
: Abstract(cloner)
{}

StyleHeaderLeft::~StyleHeaderLeft() {}

Abstract*
StyleHeaderLeft::Clone(Abstract *parent) const
{
	auto *p = new StyleHeaderLeft(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->style_display_ = style_display_;
	
	return p;
}

void
StyleHeaderLeft::Init(Tag *tag)
{
	tag->Copy(ns_->style(), ods::ns::kDisplay, style_display_);
	ScanString(tag);
}

void
StyleHeaderLeft::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->style(), ods::ns::kDisplay, style_display_);
	WriteNodes(xml);
}

} // ods::inst::
} // ods::
