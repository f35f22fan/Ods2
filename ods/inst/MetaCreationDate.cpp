#include "MetaCreationDate.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods::inst {

MetaCreationDate::MetaCreationDate(Abstract *parent, Tag *tag)
: Abstract(parent, parent->ns(), id::MetaCreationDate)
{
	if (tag != nullptr)
		Init(tag);
}

MetaCreationDate::MetaCreationDate(const MetaCreationDate &cloner)
: Abstract(cloner)
{}

MetaCreationDate::~MetaCreationDate() {}

Abstract*
MetaCreationDate::Clone(Abstract *parent) const
{
	auto *p = new MetaCreationDate(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	return p;
}

void MetaCreationDate::Init(Tag *tag)
{
	ReadStrings(tag);
}

void MetaCreationDate::ListKeywords(inst::Keywords &list, const inst::LimitTo lt)
{
	inst::AddKeywords({tag_name()}, list);
}

void MetaCreationDate::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->meta(), list);
}

void MetaCreationDate::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
