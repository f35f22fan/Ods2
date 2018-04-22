#include "StyleDefaultStyle.hpp"

#include "StyleParagraphProperties.hpp"
#include "StyleTextProperties.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

StyleDefaultStyle::StyleDefaultStyle(ods::inst::Abstract *parent, Tag *tag)
: Abstract(parent, parent->ns(), id::StyleDefaultStyle)
{
	if (tag != nullptr)
		Init(tag);
}

StyleDefaultStyle::StyleDefaultStyle(const StyleDefaultStyle &cloner)
: Abstract(cloner)
{}

StyleDefaultStyle::~StyleDefaultStyle()
{}

Abstract*
StyleDefaultStyle::Clone(Abstract *parent) const
{
	auto *p = new StyleDefaultStyle(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->style_family_ = style_family_;
	
	return p;
}

void
StyleDefaultStyle::Init(ods::Tag *tag)
{
	tag->Copy(ns_->style(), ods::ns::kFamily, style_family_);
	Scan(tag);
}

void
StyleDefaultStyle::Scan(ods::Tag *tag)
{
	foreach (auto *x, tag->nodes())
	{
		if (AddText(x))
			continue;
		
		auto *next = x->as_tag();
		
		if (next->Is(ns_->style(), ods::ns::kParagraphProperties))
		{
			Append(new StyleParagraphProperties(this, next));
		} else if (next->Is(ns_->style(), ods::ns::kTextProperties)) {
			Append(new StyleTextProperties(this, next));
		} else {
			Scan(next);
		}
	}
}

void
StyleDefaultStyle::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->style(), ods::ns::kFamily, style_family_);
	WriteNodes(xml);
}

} // ods::inst::
} // ods::
