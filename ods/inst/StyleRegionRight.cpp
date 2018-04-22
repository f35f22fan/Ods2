#include "StyleRegionRight.hpp"

#include "TextP.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

StyleRegionRight::StyleRegionRight(Abstract *parent, Tag *tag)
: Abstract (parent, parent->ns(), id::StyleRegionRight)
{
	if (tag != nullptr)
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
	
	return p;
}

void
StyleRegionRight::Init(Tag *tag)
{
	Scan(tag);
}

void
StyleRegionRight::Scan(ods::Tag *tag)
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
StyleRegionRight::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
} // ods::
