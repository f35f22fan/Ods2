#include "NumberHours.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods::inst {

NumberHours::NumberHours(Abstract *parent, Tag *tag)
: Abstract(parent, parent->ns(), id::NumberHours)
{
	if (tag != nullptr)
		Init(tag);
}

NumberHours::NumberHours(const NumberHours &cloner)
: Abstract(cloner)
{}

NumberHours::~NumberHours() {}

Abstract*
NumberHours::Clone(Abstract *parent) const
{
	auto *p = new NumberHours(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->number_style_ = number_style_;
	p->CloneChildrenOf(this, ClonePart::Text);
	
	return p;
}

void NumberHours::Init(ods::Tag *tag)
{
	tag->Copy(ns_->number(), ns::kStyle, number_style_);
	ScanString(tag);
}

void NumberHours::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name(), ns::kStyle}, list);
}

void NumberHours::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->number(), list);
}

void NumberHours::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->number(), ns::kStyle, number_style_);
	WriteNodes(xml);
}

void NumberHours::WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba)
{
	CHECK_TRUE_VOID(ba != nullptr);
	WriteTag(kw, *ba);
	WriteNdffProp(kw, *ba, ns_->number(), ns::kStyle, number_style_);
	CloseBasedOnChildren(h, kw, file, ba);
}

} // ods::inst::
