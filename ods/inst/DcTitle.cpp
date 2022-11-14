#include "DcTitle.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods::inst {

DcTitle::DcTitle(Abstract *parent, Tag *tag, ndff::Container *cntr)
: Abstract(parent, parent->ns(), id::DcTitle)
{
	if (cntr)
		ReadStrings(cntr);
	else if (tag)
		ReadStrings(tag);
}

DcTitle::DcTitle(const DcTitle &cloner) : Abstract(cloner)
{}

DcTitle::~DcTitle() {}

Abstract*
DcTitle::Clone(Abstract *parent) const
{
	auto *p = new DcTitle(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	return p;
}

void DcTitle::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name()}, list);
}

void DcTitle::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->dc(), list);
}

void DcTitle::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
