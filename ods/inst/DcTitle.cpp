#include "DcTitle.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

DcTitle::DcTitle(Abstract *parent, Tag *tag)
: Abstract(parent, parent->ns(), id::DcTitle)
{
	if (tag != nullptr)
		Init(tag);
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

void
DcTitle::Init(Tag *tag)
{
	ScanString(tag);
}

void
DcTitle::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
} // ods::
