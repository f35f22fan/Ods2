#include "OfficeScripts.hpp"

#include "../Ns.hpp"

namespace ods::inst {

OfficeScripts::OfficeScripts(Abstract *parent, Tag *tag,
	ndff::Container *cntr)
: Abstract(parent, parent->ns(), id::OfficeScripts)
{
	if (cntr)
		ReadStrings(cntr);
	else if (tag)
		ReadStrings(tag);
}

OfficeScripts::OfficeScripts(const OfficeScripts &cloner)
: Abstract(cloner)
{}

OfficeScripts::~OfficeScripts() {}

Abstract*
OfficeScripts::Clone(Abstract *parent) const
{
	auto *p = new OfficeScripts(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->CloneChildrenOf(this);
	
	return p;
}

void OfficeScripts::ListKeywords(inst::Keywords &list, const inst::LimitTo lt)
{
	inst::AddKeywords({tag_name()}, list);
}

void OfficeScripts::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->office(), list);
}

void OfficeScripts::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
