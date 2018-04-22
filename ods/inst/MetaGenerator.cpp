#include "MetaGenerator.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

MetaGenerator::MetaGenerator(Abstract *parent, Tag *tag)
: Abstract(parent, parent->ns(), id::MetaGenerator)
{
	if (tag != nullptr)
		Init(tag);
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

void
MetaGenerator::Init(Tag *tag)
{
	ScanString(tag);
}

void
MetaGenerator::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
} // ods::
