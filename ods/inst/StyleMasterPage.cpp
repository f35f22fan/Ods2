#include "StyleMasterPage.hpp"

#include "StyleFooter.hpp"
#include "StyleFooterLeft.hpp"
#include "StyleHeader.hpp"
#include "StyleHeaderLeft.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

StyleMasterPage::StyleMasterPage(Abstract *parent, Tag *tag)
: Abstract(parent, parent->ns(), id::StyleMasterPage)
{
	if (tag != nullptr)
		Init(tag);
}

StyleMasterPage::StyleMasterPage(const StyleMasterPage &cloner)
: Abstract(cloner)
{}

StyleMasterPage::~StyleMasterPage()
{}

Abstract*
StyleMasterPage::Clone(Abstract *parent) const
{
	auto *p = new StyleMasterPage(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->style_name_ = style_name_;
	p->style_page_layout_name_ = style_page_layout_name_;
	
	return p;
}

void
StyleMasterPage::Init(Tag *tag)
{
	tag->Copy(ns_->style(), ods::ns::kName, style_name_);
	tag->Copy(ns_->style(), ods::ns::kPageLayoutName, style_page_layout_name_);
	Scan(tag);
}

void
StyleMasterPage::Scan(Tag *tag)
{
	for (auto *x: tag->nodes())
	{
		if (AddText(x))
			continue;
		
		auto *next = x->as_tag();
		
		if (next->Is(ns_->style(), ods::ns::kFooter)) {
			Append(new StyleFooter(this, next));
		} else if (next->Is(ns_->style(), ods::ns::kFooterLeft)) {
			Append(new StyleFooterLeft(this, next));
		} else if (next->Is(ns_->style(), ods::ns::kHeader)) {
			Append(new StyleHeader(this, next));
		} else if (next->Is(ns_->style(), ods::ns::kHeaderLeft)) {
			Append(new StyleHeaderLeft(this, next));
		} else {
			Scan(next);
		}
	}
}

void
StyleMasterPage::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->style(), ods::ns::kName, style_name_);
	Write(xml, ns_->style(), ods::ns::kPageLayoutName, style_page_layout_name_);
	WriteNodes(xml);
}

} // ods::inst::
} // ods::
