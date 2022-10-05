#include "NumberMonth.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods::inst {

NumberMonth::NumberMonth(Abstract *parent, Tag *tag) :
Abstract(parent, parent->ns(), id::NumberMonth)
{
	if (tag != nullptr)
		Init(tag);
}

NumberMonth::NumberMonth(const NumberMonth &cloner)
: Abstract(cloner)
{}

NumberMonth::~NumberMonth() {}

Abstract*
NumberMonth::Clone(Abstract *parent) const
{
	auto *p = new NumberMonth(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->number_style_ = number_style_;
	p->CloneChildrenOf(this, ClonePart::Text);
	
	return p;
}

void NumberMonth::Init(ods::Tag *tag)
{
	tag->Copy(ns_->number(), ns::kStyle, number_style_);
	ScanString(tag);
}

void NumberMonth::ListKeywords(Keywords &list, const LimitTo lt)
{
	AddKeywords({tag_name(), ns::kStyle}, list);
}

void NumberMonth::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->number(), list);
}

void NumberMonth::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->number(), ods::ns::kStyle, number_style_);
	WriteNodes(xml);
}

void NumberMonth::WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba)
{
	CHECK_TRUE_VOID(ba != nullptr);
	WriteTag(kw, *ba);
	WriteNdffProp(kw, *ba, ns_->number(), ns::kStyle, number_style_);
	CloseBasedOnChildren(h, kw, file, ba);
}

} // ods::inst::
