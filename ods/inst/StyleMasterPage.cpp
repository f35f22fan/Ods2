#include "StyleMasterPage.hpp"

#include "StyleFooter.hpp"
#include "StyleFooterLeft.hpp"
#include "StyleHeader.hpp"
#include "StyleHeaderLeft.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

#include "../ndff/Container.hpp"
#include "../ndff/Property.hpp"

namespace ods::inst {

StyleMasterPage::StyleMasterPage(Abstract *parent, Tag *tag, ndff::Container *cntr)
: Abstract(parent, parent->ns(), id::StyleMasterPage)
{
	if (cntr)
		Init(cntr);
	else if (tag)
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

void StyleMasterPage::Init(ndff::Container *cntr)
{
	using Op = ndff::Op;
	ndff::Property prop;
	QHash<UriId, QVector<ndff::Property>> attrs;
	Op op = cntr->Next(prop, Op::TS, &attrs);
	CopyAttr(attrs, ns_->style(), ns::kName, style_name_);
	CopyAttr(attrs, ns_->style(), ns::kPageLayoutName, style_page_layout_name_);
	if (op == Op::N32_TE)
		return;
	
	if (op == Op::TCF_CMS)
		op = cntr->Next(prop, op);
	
	while (true)
	{
		if (op == Op::TS)
		{
			if (prop.is(ns_->style()))
			{
				if (prop.name == ns::kFooter)
					Append(new StyleFooter(this, 0, cntr), TakeOwnership::Yes);
				else if (prop.name == ns::kFooterLeft)
					Append(new StyleFooterLeft(this, 0, cntr), TakeOwnership::Yes);
				else if (prop.name == ns::kHeader)
					Append(new StyleHeader(this, 0, cntr), TakeOwnership::Yes);
				else if (prop.name == ns::kHeaderLeft)
					Append(new StyleHeaderLeft(this, 0, cntr), TakeOwnership::Yes);
			}
		} else if (ndff::is_text(op)) {
			Append(cntr->NextString());
		} else {
			break;
		}
		
		op = cntr->Next(prop, op);
	}
	
	if (op != Op::SCT)
		mtl_trace("Unexpected op: %d", op);
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
			Append(new StyleFooter(this, next), TakeOwnership::Yes);
		} else if (next->Is(ns_->style(), ns::kFooterLeft)) {
			Append(new StyleFooterLeft(this, next), TakeOwnership::Yes);
		} else if (next->Is(ns_->style(), ns::kHeader)) {
			Append(new StyleHeader(this, next), TakeOwnership::Yes);
		} else if (next->Is(ns_->style(), ns::kHeaderLeft)) {
			Append(new StyleHeaderLeft(this, next), TakeOwnership::Yes);
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
