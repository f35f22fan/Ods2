#include "StyleRegionLeft.hpp"

#include "TextP.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

StyleRegionLeft::StyleRegionLeft(Abstract *parent, Tag *tag)
: Abstract (parent, parent->ns(), id::StyleRegionLeft)
{
	if (tag != nullptr)
		Init(tag);
}

StyleRegionLeft::StyleRegionLeft(const StyleRegionLeft &cloner)
: Abstract(cloner)
{}

StyleRegionLeft::~StyleRegionLeft()
{}

Abstract*
StyleRegionLeft::Clone(Abstract *parent) const
{
	auto *p = new StyleRegionLeft(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	return p;
}

void
StyleRegionLeft::Init(Tag *tag)
{
	Scan(tag);
}

void
StyleRegionLeft::Scan(ods::Tag *tag)
{
	for (auto *x: tag->nodes())
	{
		if (AddText(x))
			continue;
		
		auto *next = x->as_tag();
		
		if (next->Is(ns_->text(), ods::ns::kP)) {
			Append(new TextP(this, next));
		} else {
			Scan(next);
		}
	}
}

void
StyleRegionLeft::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
} // ods::
