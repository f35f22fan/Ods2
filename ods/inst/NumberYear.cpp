#include "NumberYear.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods::inst {

NumberYear::NumberYear(Abstract *parent, ods::Tag *tag)
: Abstract(parent, parent->ns(), id::NumberYear)
{
	if (tag)
		Init(tag);
}

NumberYear::NumberYear(const NumberYear &cloner)
: Abstract(cloner)
{}

NumberYear::~NumberYear() {}

Abstract*
NumberYear::Clone(Abstract *parent) const
{
	auto *p = new NumberYear(*this);
	
	if (parent)
		p->parent(parent);
	
	p->number_style_ = number_style_;
	p->CloneChildrenOf(this, ClonePart::Text);
	
	return p;
}

void NumberYear::Init(ods::Tag *tag)
{
	tag->Copy(ns_->number(), ns::kStyle, number_style_);
	ReadStrings(tag);
}

void NumberYear::ListKeywords(inst::Keywords &list, const inst::LimitTo lt)
{
	inst::AddKeywords({tag_name(), ns::kStyle}, list);
}

void NumberYear::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->number(), list);
}

void NumberYear::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->number(), ods::ns::kStyle, number_style_);
	WriteNodes(xml);
}

} // ods::inst::
