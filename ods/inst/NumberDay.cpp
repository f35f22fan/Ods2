#include "NumberDay.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods::inst {

NumberDay::NumberDay(ods::inst::Abstract *parent, ods::Tag *tag)
 : Abstract(parent, parent->ns(), id::NumberDay)
{
	if (tag != nullptr)
		Init(tag);
}

NumberDay::NumberDay(const NumberDay &cloner)
: Abstract(cloner)
{}

NumberDay::~NumberDay() {}

Abstract*
NumberDay::Clone(Abstract *parent) const
{
	auto *p = new NumberDay(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->number_style_ = number_style_;
	p->CloneChildrenOf(this, ClonePart::Text);
	
	return p;
}

void NumberDay::Init(ods::Tag *tag)
{
	tag->Copy(ns_->number(), ns::kStyle, number_style_);
	ScanString(tag);
}

void NumberDay::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name(), ns::kStyle}, list);
}

void NumberDay::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->number(), list);
}

void NumberDay::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->number(), ns::kStyle, number_style_);
	
	WriteNodes(xml);
}

void NumberDay::WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba)
{
	CHECK_TRUE_VOID(ba != nullptr);
	WriteTag(kw, *ba);
	WriteNdffProp(kw, *ba, ns_->number(), ns::kStyle, number_style_);
	CloseBasedOnChildren(h, kw, file, ba);
}

} // ods::inst::
