#include "TextPageCount.hpp"

#include "../Ns.hpp"
#include "../Tag.hpp"

namespace ods::inst {

TextPageCount::TextPageCount(Abstract *parent, Tag *tag,
	ndff::Container *cntr)
: Abstract(parent, parent->ns(), id::TextPageCount)
{
	if (cntr)
		ReadStrings(cntr);
	else if (tag)
		ReadStrings(tag);
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
	
	p->CloneChildrenOf(this);
	
	return p;
}

void TextPageCount::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name()}, list);
}

void TextPageCount::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->text(), list);
}

void TextPageCount::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
