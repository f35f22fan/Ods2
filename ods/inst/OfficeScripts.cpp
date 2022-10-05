#include "OfficeScripts.hpp"

#include "../Ns.hpp"

namespace ods::inst {

OfficeScripts::OfficeScripts(Abstract *parent, Tag *tag)
: Abstract(parent, parent->ns(), id::OfficeScripts)
{
	if (tag != nullptr)
		Init(tag);
	else
		InitDefault();
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

void
OfficeScripts::Init(Tag *tag)
{
	ScanString(tag);
}

void OfficeScripts::InitDefault() {}

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
