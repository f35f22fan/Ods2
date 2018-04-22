#include "MetaEditingCycles.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

MetaEditingCycles::MetaEditingCycles(Abstract *parent, Tag *tag)
: Abstract(parent, parent->ns(), id::MetaEditingCycles)
{
	if (tag != nullptr)
		Init(tag);
}

MetaEditingCycles::MetaEditingCycles(const MetaEditingCycles &cloner)
: Abstract(cloner)
{}

MetaEditingCycles::~MetaEditingCycles() {}

Abstract*
MetaEditingCycles::Clone(Abstract *parent) const
{
	auto *p = new MetaEditingCycles(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	return p;
}

void
MetaEditingCycles::Init(Tag *tag)
{
	ScanString(tag);
}

void
MetaEditingCycles::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
} // ods::
