#include "MetaGenerator.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods::inst {

MetaGenerator::MetaGenerator(Abstract *parent, Tag *tag)
: Abstract(parent, parent->ns(), id::MetaGenerator)
{
	if (tag)
		ReadStrings(tag);
}

MetaGenerator::MetaGenerator(const MetaGenerator &cloner)
: Abstract(cloner)
{}

MetaGenerator::~MetaGenerator() {}

Abstract*
MetaGenerator::Clone(Abstract *parent) const
{
	auto *p = new MetaGenerator(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	return p;
}

void MetaGenerator::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name()}, list);
}

void MetaGenerator::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->meta(), list);
}

void MetaGenerator::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
