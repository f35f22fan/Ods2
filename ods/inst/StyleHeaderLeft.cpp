#include "StyleHeaderLeft.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods::inst {

StyleHeaderLeft::StyleHeaderLeft(Abstract *parent, Tag *tag)
: Abstract(parent, parent->ns(), id::StyleHeaderLeft)
{
	if (tag != nullptr)
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

void StyleHeaderLeft::Init(Tag *tag)
{
	tag->Copy(ns_->style(), ns::kDisplay, style_display_);
	ScanString(tag);
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
	CHECK_TRUE_VOID(ba != nullptr);
	WriteTag(kw, *ba);
	WriteNdffProp(kw, *ba, ns_->style(), ns::kDisplay, style_display_);
	CloseBasedOnChildren(h, kw, file, ba);
}

} // ods::inst::
