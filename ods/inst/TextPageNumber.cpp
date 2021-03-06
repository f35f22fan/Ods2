#include "TextPageNumber.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

TextPageNumber::TextPageNumber(Abstract *parent, ods::Tag *tag)
: Abstract(parent, parent->ns(), id::TextPageNumber)
{
	if (tag != nullptr)
		Init(tag);
}

TextPageNumber::TextPageNumber(const TextPageNumber &cloner)
: Abstract(cloner)
{}

TextPageNumber::~TextPageNumber() {}

Abstract*
TextPageNumber::Clone(Abstract *parent) const
{
	auto *p = new TextPageNumber(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	return p;
}

void
TextPageNumber::Init(ods::Tag *tag)
{
	ScanString(tag);
}

void
TextPageNumber::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
} // ods::
