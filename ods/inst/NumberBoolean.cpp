#include "NumberBoolean.hpp"

#include "../Ns.hpp"
#include "../Tag.hpp"

namespace ods::inst {

NumberBoolean::NumberBoolean(Abstract *parent, ods::Tag *tag) :
Abstract(parent, parent->ns(), id::NumberBoolean)
{
//	if (tag != nullptr)
//		Init(tag); // cannot have any params
}

NumberBoolean::NumberBoolean(const NumberBoolean &cloner)
: Abstract(cloner)
{}

NumberBoolean::~NumberBoolean() {}

Abstract*
NumberBoolean::Clone(Abstract *parent) const
{
	auto *p = new NumberBoolean(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	return p;
}

void NumberBoolean::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name()}, list);
}

void NumberBoolean::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->number(), list);
}

void NumberBoolean::WriteData(QXmlStreamWriter &xml)
{
//	WriteNodes(xml); // cannot have any children
}

} // ods::inst::
