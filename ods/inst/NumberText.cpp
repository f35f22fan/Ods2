#include "NumberText.hpp"

namespace ods::inst {

NumberText::NumberText(ods::inst::Abstract *parent, Tag *tag)
: Abstract(parent, parent->ns(), id::NumberText)
{
	if (tag)
		ReadStrings(tag);
}

NumberText::NumberText(const NumberText &cloner)
: Abstract(cloner)
{}

NumberText::~NumberText() {}

Abstract*
NumberText::Clone(Abstract *parent) const
{
	auto *p = new NumberText(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->CloneChildrenOf(this);
	
	return p;
}

void NumberText::ListKeywords(Keywords &list, const LimitTo lt)
{
	AddKeywords({tag_name()}, list);
}

void NumberText::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->number(), list);
}

void NumberText::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
