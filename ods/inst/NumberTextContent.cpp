#include "NumberTextContent.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

NumberTextContent::NumberTextContent(Abstract *parent, Tag *tag)
: Abstract(parent, parent->ns(), id::NumberTextContent)
{
	if (tag != nullptr)
		Init(tag);
}

NumberTextContent::NumberTextContent(const NumberTextContent &cloner)
: Abstract(cloner)
{}

NumberTextContent::~NumberTextContent() {}

Abstract*
NumberTextContent::Clone(Abstract *parent) const
{
	auto *p = new NumberTextContent(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	return p;
}

void
NumberTextContent::Init(ods::Tag *tag)
{
	ScanString(tag);
}

void
NumberTextContent::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
} // ods::
