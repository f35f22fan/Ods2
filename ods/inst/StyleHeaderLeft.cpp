#include "StyleHeaderLeft.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

#include "../ndff/Container.hpp"
#include "../ndff/Property.hpp"

namespace ods::inst {

StyleHeaderLeft::StyleHeaderLeft(Abstract *parent, Tag *tag, ndff::Container *cntr)
: Abstract(parent, parent->ns(), id::StyleHeaderLeft)
{
	if (cntr)
		Init(cntr);
	else if (tag)
		Init(tag);
}

StyleHeaderLeft::StyleHeaderLeft(const StyleHeaderLeft &cloner)
: Abstract(cloner)
{}

StyleHeaderLeft::~StyleHeaderLeft() {}

Abstract*
StyleHeaderLeft::Clone(Abstract *parent) const
{
	auto *p = new StyleHeaderLeft(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->style_display_ = style_display_;
	
	p->CloneChildrenOf(this);
	
	return p;
}

void StyleHeaderLeft::Init(ndff::Container *cntr)
{
	using Op = ndff::Op;
	ndff::Property prop;
	NdffAttrs attrs;
	Op op = cntr->Next(prop, Op::TS, &attrs);
	CopyAttr(attrs, ns_->style(), ns::kDisplay, style_display_);
	ReadStrings(cntr, op);
}

void StyleHeaderLeft::Init(Tag *tag)
{
	tag->Copy(ns_->style(), ns::kDisplay, style_display_);
	ReadStrings(tag);
}

void StyleHeaderLeft::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name(), ns::kDisplay}, list);
}

void StyleHeaderLeft::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->style(), list);
}

void StyleHeaderLeft::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->style(), ns::kDisplay, style_display_);
	WriteNodes(xml);
}

void StyleHeaderLeft::WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba)
{
	MTL_CHECK_VOID(ba != nullptr);
	WriteTag(kw, *ba);
	WriteNdffProp(kw, *ba, ns_->style(), ns::kDisplay, style_display_);
	CloseBasedOnChildren(h, kw, file, ba);
}

} // ods::inst::
