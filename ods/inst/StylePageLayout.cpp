#include "StylePageLayout.hpp"

#include "StyleFooterStyle.hpp"
#include "StyleHeaderStyle.hpp"
#include "StylePageLayoutProperties.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods::inst {

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
	p->CloneChildrenOf(this);
	
	return p;
}

void StylePageLayout::Init(ods::Tag *tag)
{
	tag->Copy(ns_->style(), ns::kName, style_name_);
	Scan(tag);
}

void StylePageLayout::ListKeywords(inst::Keywords &list, const inst::LimitTo lt)
{
	inst::AddKeywords({tag_name(), ns::kName}, list);
}

void StylePageLayout::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->style(), list);
}

void StylePageLayout::Scan(ods::Tag *tag)
{
	foreach (auto *x, tag->nodes())
	{
		if (AddText(x))
			continue;
		
		auto *next = x->as_tag();
		
		if (next->Is(ns_->style(), ns::kPageLayoutProperties))
		{
			Append(new StylePageLayoutProperties(this, next));
		} else if (next->Is(ns_->style(), ns::kFooterStyle)) {
			Append(new StyleFooterStyle(this, next));
		} else if (next->Is(ns_->style(), ns::kHeaderStyle)) {
			Append(new StyleHeaderStyle(this, next));
		} else {
			Scan(next);
		}
	}
}

void StylePageLayout::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->style(), ns::kName, style_name_);
	WriteNodes(xml);
}

void StylePageLayout::WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba)
{
	CHECK_TRUE_VOID(ba != nullptr);
	WriteTag(kw, *ba);
	WriteNdffProp(kw, *ba, ns_->style(), ns::kName, style_name_);
	CloseBasedOnChildren(h, kw, file, ba);
}

} // ods::inst::
