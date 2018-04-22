#include "OfficeScripts.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

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
	
	return p;
}

void
OfficeScripts::Init(Tag *tag)
{
	ScanString(tag);
}

void
OfficeScripts::InitDefault()
{}

void
OfficeScripts::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
} // ods::
