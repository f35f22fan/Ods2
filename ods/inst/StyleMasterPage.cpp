#include "StyleMasterPage.hpp"

#include "StyleFooter.hpp"
#include "StyleFooterLeft.hpp"
#include "StyleHeader.hpp"
#include "StyleHeaderLeft.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods::inst {

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
	p->CloneChildrenOf(this);
	
	return p;
}

void StyleMasterPage::Init(Tag *tag)
{
	tag->Copy(ns_->style(), ns::kName, style_name_);
	tag->Copy(ns_->style(), ns::kPageLayoutName, style_page_layout_name_);
	Scan(tag);
}

void StyleMasterPage::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name(), ns::kName, ns::kPageLayoutName}, list);
}

void StyleMasterPage::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->style(), list);
}

void StyleMasterPage::Scan(Tag *tag)
{
	for (auto *x: tag->nodes())
	{
		if (AddText(x))
			continue;
		
		auto *next = x->as_tag();
		
		if (next->Is(ns_->style(), ns::kFooter)) {
			Append(new StyleFooter(this, next));
		} else if (next->Is(ns_->style(), ns::kFooterLeft)) {
			Append(new StyleFooterLeft(this, next));
		} else if (next->Is(ns_->style(), ns::kHeader)) {
			Append(new StyleHeader(this, next));
		} else if (next->Is(ns_->style(), ns::kHeaderLeft)) {
			Append(new StyleHeaderLeft(this, next));
		} else {
			Scan(next);
		}
	}
}

void StyleMasterPage::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->style(), ns::kName, style_name_);
	Write(xml, ns_->style(), ns::kPageLayoutName, style_page_layout_name_);
	WriteNodes(xml);
}

void StyleMasterPage::WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba)
{
	CHECK_TRUE_VOID(ba != nullptr);
	WriteTag(kw, *ba);
	WriteNdffProp(kw, *ba, ns_->style(), ns::kName, style_name_);
	WriteNdffProp(kw, *ba, ns_->style(), ns::kPageLayoutName, style_page_layout_name_);
	CloseBasedOnChildren(h, kw, file, ba);
}

} // ods::inst::
