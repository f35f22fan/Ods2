#include "StyleFooter.hpp"

#include "TextP.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

StyleFooter::StyleFooter(Abstract *parent, Tag *tag)
: Abstract(parent, parent->ns(), id::StyleFooter)
{
	if (tag != nullptr)
		Init(tag);
}

StyleFooter::StyleFooter(const StyleFooter &cloner)
: Abstract(cloner)
{}

StyleFooter::~StyleFooter()
{}

Abstract*
StyleFooter::Clone(Abstract *parent) const
{
	auto *p = new StyleFooter(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	return p;
}

void
StyleFooter::Init(ods::Tag *tag)
{
	Scan(tag);
}

void
StyleFooter::Scan(ods::Tag *tag)
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
StyleFooter::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
} // ods::
