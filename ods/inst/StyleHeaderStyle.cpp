#include "StyleHeaderStyle.hpp"

#include "StyleHeaderFooterProperties.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

StyleHeaderStyle::StyleHeaderStyle(Abstract *parent, ods::Tag *tag)
: Abstract(parent, parent->ns(), id::StyleHeaderStyle)
{
	if (tag != nullptr)
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

	return p;
}

void
StyleHeaderStyle::Init(ods::Tag *tag)
{
	tag->Copy(ns_->style(), ods::ns::kName, style_name_);
	Scan(tag);
}

void
StyleHeaderStyle::Scan(ods::Tag *tag)
{
	foreach (auto *x, tag->nodes())
	{
		if (AddText(x))
			continue;
		
		auto *next = x->as_tag();
		
		if (next->Is(ns_->style(), ods::ns::kHeaderFooterProperties))
		{
			Append(new StyleHeaderFooterProperties(this, next));
		} else {
			Scan(next);
		}
	}
}

void
StyleHeaderStyle::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->style(), ods::ns::kName, style_name_);
	WriteNodes(xml);
}

} // ods::inst::
} // ods::
