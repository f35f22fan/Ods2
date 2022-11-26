#include "MetaEditingDuration.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods::inst {

MetaEditingDuration::MetaEditingDuration(Abstract *parent, Tag *tag, ndff::Container *cntr)
: Abstract(parent, parent->ns(), id::MetaEditingDuration)
{
	if (cntr)
		ReadStrings(cntr);
	else if (tag)
		ReadStrings(tag);
}

MetaEditingDuration::MetaEditingDuration(const MetaEditingDuration &cloner)
: Abstract(cloner)
{}

MetaEditingDuration::~MetaEditingDuration() {}

Abstract*
MetaEditingDuration::Clone(Abstract *parent) const
{
	auto *p = new MetaEditingDuration(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	return p;
}

void MetaEditingDuration::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name()}, list);
}

void MetaEditingDuration::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->meta(), list);
}

void MetaEditingDuration::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
