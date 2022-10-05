#include "StyleMap.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods::inst {

StyleMap::StyleMap(Abstract *parent, ods::Tag *tag)
: Abstract(parent, parent->ns(), id::StyleMap)
{
	if (tag != nullptr)
		Init(tag);
}

StyleMap::StyleMap(const StyleMap &cloner)
: Abstract(cloner)
{}

StyleMap::~StyleMap() {}

Abstract*
StyleMap::Clone(Abstract *parent) const
{
	auto *p = new StyleMap(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->style_condition_ = style_condition_;
	p->style_apply_style_name_ = style_apply_style_name_;
	p->CloneChildrenOf(this, ClonePart::Text);
	
	return p;
}

void StyleMap::Init(ods::Tag *tag)
{
	tag->Copy(ns_->style(), ns::kCondition, style_condition_);
	tag->Copy(ns_->style(), ns::kApplyStyleName, style_apply_style_name_);
	ScanString(tag);
}

void StyleMap::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name(), ns::kCondition,
		ns::kApplyStyleName}, list);
}

void StyleMap::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->style(), list);
}

void StyleMap::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->style(), ns::kCondition, style_condition_);
	Write(xml, ns_->style(), ns::kApplyStyleName, style_apply_style_name_);
	WriteNodes(xml);
}

void StyleMap::WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba)
{
	CHECK_TRUE_VOID(ba != nullptr);
	WriteTag(kw, *ba);
	WriteNdffProp(kw, *ba, ns_->style(), ns::kCondition, style_condition_);
	WriteNdffProp(kw, *ba, ns_->style(), ns::kApplyStyleName, style_apply_style_name_);
	CloseBasedOnChildren(h, kw, file, ba);
}

} // ods::inst::
