#include "DcDate.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods::inst {

DcDate::DcDate(Abstract *parent, Tag *tag, ndff::Container *cntr)
: Abstract(parent, parent->ns(), id::DcDate)
{
	if (cntr)
		ReadStrings(cntr);
	else if (tag)
		ReadStrings(tag);
}

DcDate::DcDate(const DcDate &cloner)
: Abstract(cloner)
{}

DcDate::~DcDate() {}

Abstract*
DcDate::Clone(Abstract *parent) const
{
	auto *p = new DcDate(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	return p;
}

void DcDate::ListKeywords(inst::Keywords &list, const inst::LimitTo lt)
{
	inst::AddKeywords({tag_name()}, list);
}

void DcDate::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->dc(), list);
}

void DcDate::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
