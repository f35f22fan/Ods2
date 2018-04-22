#include "TextTitle.hpp"

#include "../Ns.hpp"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

TextTitle::TextTitle(Abstract *parent, Tag *tag)
: Abstract(parent, parent->ns(), id::TextTitle)
{
	if (tag != nullptr)
		Init(tag);
}

TextTitle::TextTitle(const TextTitle &cloner)
: Abstract(cloner)
{}

TextTitle::~TextTitle() {}

Abstract*
TextTitle::Clone(Abstract *parent) const
{
	auto *p = new TextTitle(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	return p;
}

void
TextTitle::Init(Tag *tag)
{
	ScanString(tag);
}

void
TextTitle::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
} // ods::
