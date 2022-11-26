#include "StyleFooterLeft.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

#include "../ndff/Container.hpp"
#include "../ndff/Property.hpp"

namespace ods::inst {

StyleFooterLeft::StyleFooterLeft(Abstract *parent, Tag *tag, ndff::Container *cntr)
: Abstract(parent, parent->ns(), id::StyleFooterLeft)
{
	if (cntr)
		Init(cntr);
	else if (tag)
		Init(tag);
}

StyleFooterLeft::StyleFooterLeft(const StyleFooterLeft &cloner)
: Abstract(cloner)
{}

StyleFooterLeft::~StyleFooterLeft() {}

Abstract*
StyleFooterLeft::Clone(Abstract *parent) const
{
	auto *p = new StyleFooterLeft(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->style_display_ = style_display_;
	p->CloneChildrenOf(this);
	
	return p;
}

void StyleFooterLeft::Init(ndff::Container *cntr)
{
	using Op = ndff::Op;
	ndff::Property prop;
	NdffAttrs attrs;
	Op op = cntr->Next(prop, Op::TS, &attrs);
	CopyAttr(attrs, ns_->style(), ns::kDisplay, style_display_);
	ReadStrings(cntr, op);
}

void StyleFooterLeft::Init(Tag *tag)
{
	tag->Copy(ns_->style(), ns::kDisplay, style_display_);
	ReadStrings(tag);
}

void StyleFooterLeft::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name(), ns::kDisplay}, list);
}

void StyleFooterLeft::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->style(), list);
}

void StyleFooterLeft::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->style(), ns::kDisplay, style_display_);
	WriteNodes(xml);
}

void StyleFooterLeft::WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba)
{
	CHECK_TRUE_VOID(ba != nullptr);
	WriteTag(kw, *ba);
	WriteNdffProp(kw, *ba, ns_->style(), ns::kDisplay, style_display_);
	CloseBasedOnChildren(h, kw, file, ba);
}

} // ods::inst::
