#include "NumberTextContent.hpp"
#include "../Ns.hpp"

namespace ods::inst {

NumberTextContent::NumberTextContent(Abstract *parent, Tag *tag, ndff::Container *cntr)
: Abstract(parent, parent->ns(), id::NumberTextContent)
{
	if (cntr)
		ReadStrings(cntr);
	else if (tag)
		ReadStrings(tag);
}

NumberTextContent::NumberTextContent(const NumberTextContent &cloner)
: Abstract(cloner)
{}

NumberTextContent::~NumberTextContent() {}

Abstract*
NumberTextContent::Clone(Abstract *parent) const
{
	auto *p = new NumberTextContent(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->CloneChildrenOf(this);
	
	return p;
}

void NumberTextContent::ListKeywords(inst::Keywords &list, const inst::LimitTo lt)
{
	inst::AddKeywords({tag_name()}, list);
}

void NumberTextContent::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->number(), list);
}

void NumberTextContent::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
