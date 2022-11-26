#include "TextS.hpp"

#include "../Ns.hpp"

#include "../ndff/Container.hpp"
#include "../ndff/Property.hpp"

namespace ods::inst {

TextS::TextS(Abstract *parent, Tag *tag, ndff::Container *cntr)
: Abstract(parent, parent->ns(), id::TextS)
{
	if (cntr)
		ReadStrings(cntr);
	else if (tag)
		ReadStrings(tag);
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

void TextS::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name()}, list);
}

void TextS::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->text(), list);
}

void TextS::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
