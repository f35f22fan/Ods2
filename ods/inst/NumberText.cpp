#include "NumberText.hpp"

namespace ods::inst {

NumberText::NumberText(ods::inst::Abstract *parent, Tag *tag, ndff::Container *cntr)
: Abstract(parent, parent->ns(), id::NumberText)
{
	if (cntr)
		ReadStrings(cntr);
	else if (tag)
		ReadStrings(tag);
}

NumberText::NumberText(const NumberText &cloner)
: Abstract(cloner)
{}

NumberText::~NumberText() {}

Abstract*
NumberText::Clone(Abstract *parent) const
{
	auto *p = new NumberText(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->CloneChildrenOf(this);
	
	return p;
}

const QString*
NumberText::GetFirstString() const
{
	for (StringOrInst *node: nodes_)
	{
		if (node->is_string())
			return node->as_str_ptr(); //&node->as_string();
	}
	
	return nullptr;
}

void NumberText::ListKeywords(Keywords &list, const LimitTo lt)
{
	AddKeywords({tag_name()}, list);
}

void NumberText::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->number(), list);
}

void NumberText::SetFirstString(const QString &s)
{
	for (StringOrInst *node: nodes_)
	{
		if (node->is_string())
		{
			node->SetString(s);
			return;
		}
	}
	
	Append(s);
}

void NumberText::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
