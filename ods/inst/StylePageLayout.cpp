#include "StylePageLayout.hpp"

#include "StyleFooterStyle.hpp"
#include "StyleHeaderStyle.hpp"
#include "StylePageLayoutProperties.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

StylePageLayout::StylePageLayout(Abstract *parent, ods::Tag *tag)
: Abstract(parent, parent->ns(), id::StylePageLayout)
{
	if (tag != nullptr)
		Init(tag);
}

StylePageLayout::StylePageLayout(const StylePageLayout &cloner)
: Abstract(cloner)
{}

StylePageLayout::~StylePageLayout()
{}

Abstract*
StylePageLayout::Clone(Abstract *parent) const
{
	auto *p = new StylePageLayout(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->style_name_ = style_name_;
	
	return p;
}

void
StylePageLayout::Init(ods::Tag *tag)
{
	tag->Copy(ns_->style(), ods::ns::kName, style_name_);
	Scan(tag);
}

void
StylePageLayout::Scan(ods::Tag *tag)
{
	foreach (auto *x, tag->nodes())
	{
		if (AddText(x))
			continue;
		
		auto *next = x->as_tag();
		
		if (next->Is(ns_->style(), ods::ns::kPageLayoutProperties))
		{
			Append(new StylePageLayoutProperties(this, next));
		} else if (next->Is(ns_->style(), ods::ns::kFooterStyle)) {
			Append(new StyleFooterStyle(this, next));
		} else if (next->Is(ns_->style(), ods::ns::kHeaderStyle)) {
			Append(new StyleHeaderStyle(this, next));
		} else {
			Scan(next);
		}
	}
}

void
StylePageLayout::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->style(), ods::ns::kName, style_name_);
	WriteNodes(xml);
}

} // ods::inst::
} // ods::
