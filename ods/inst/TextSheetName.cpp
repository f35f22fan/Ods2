#include "TextSheetName.hpp"

#include "../Ns.hpp"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

TextSheetName::TextSheetName(Abstract *parent, Tag *tag)
: Abstract(parent, parent->ns(), id::TextSheetName)
{
	if (tag != nullptr)
		Init(tag);
}

TextSheetName::TextSheetName(const TextSheetName &cloner)
: Abstract(cloner)
{}

TextSheetName::~TextSheetName() {}

Abstract*
TextSheetName::Clone(Abstract *parent) const
{
	auto *p = new TextSheetName(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	return p;
}

void
TextSheetName::Init(Tag *tag)
{
	ScanString(tag);
}

void
TextSheetName::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
} // ods::
