#include "TextPageCount.hpp"

#include "../Ns.hpp"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

TextPageCount::TextPageCount(Abstract *parent, Tag *tag)
: Abstract(parent, parent->ns(), id::TextPageCount)
{
	if (tag != nullptr)
		Init(tag);
}

TextPageCount::TextPageCount(const TextPageCount &cloner)
: Abstract(cloner)
{}

TextPageCount::~TextPageCount() {}

Abstract*
TextPageCount::Clone(Abstract *parent) const
{
	auto *p = new TextPageCount(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	return p;
}

void
TextPageCount::Init(ods::Tag *tag)
{
	ScanString(tag);
}

void
TextPageCount::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
} // ods::
