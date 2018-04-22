#include "MetaEditingDuration.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

MetaEditingDuration::MetaEditingDuration(Abstract *parent, Tag *tag)
: Abstract(parent, parent->ns(), id::MetaEditingDuration)
{
	if (tag != nullptr)
		Init(tag);
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

void
MetaEditingDuration::Init(Tag *tag)
{
	ScanString(tag);
}

void
MetaEditingDuration::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
} // ods::
