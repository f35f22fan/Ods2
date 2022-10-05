#include "TextSheetName.hpp"

#include "../Ns.hpp"
#include "../Tag.hpp"

namespace ods::inst {

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
	
	p->CloneChildrenOf(this);
	
	return p;
}

void TextSheetName::Init(Tag *tag)
{
	ScanString(tag);
}

void TextSheetName::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name()}, list);
}

void TextSheetName::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->text(), list);
}

void TextSheetName::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
