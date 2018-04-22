#include "StyleFooterStyle.hpp"

#include "StyleHeaderFooterProperties.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

StyleFooterStyle::StyleFooterStyle(Abstract *parent, Tag *tag)
: Abstract(parent, parent->ns(), id::StyleFooterStyle)
{
	if (tag != nullptr)
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
	
	return p;
}

void
StyleFooterStyle::Init(ods::Tag *tag)
{
	Scan(tag);
}

void
StyleFooterStyle::Scan(ods::Tag *tag)
{
	foreach (auto *x, tag->nodes())
	{
		if (AddText(x))
			continue;
		
		auto *next = x->as_tag();
		
		if (next->Is(ns_->style(), ods::ns::kHeaderFooterProperties)) {
			Append(new StyleHeaderFooterProperties(this, next));
		} else {
			Scan(next);
		}
	}
}

void
StyleFooterStyle::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
} // ods::

