#include "TextPageNumber.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods::inst {

TextPageNumber::TextPageNumber(Abstract *parent, ods::Tag *tag)
: Abstract(parent, parent->ns(), id::TextPageNumber)
{
	if (tag)
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
	
	p->CloneChildrenOf(this);
	
	return p;
}

void TextPageNumber::Init(ods::Tag *tag)
{
	ReadStrings(tag);
}

void TextPageNumber::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name()}, list);
}

void TextPageNumber::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->text(), list);
}

void TextPageNumber::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
