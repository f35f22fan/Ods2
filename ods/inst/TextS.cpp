#include "TextS.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

TextS::TextS(Abstract *parent, Tag *tag)
: Abstract(parent, parent->ns(), id::TextS)
{
	if (tag != nullptr)
		Init(tag);
}

TextS::TextS(const TextS &cloner)
: Abstract(cloner)
{}

TextS::~TextS() {}

Abstract*
TextS::Clone(Abstract *parent) const
{
	auto *p = new TextS(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	return p;
}

void
TextS::Init(ods::Tag *tag)
{
	ScanString(tag);
}

void
TextS::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
} // ods::
