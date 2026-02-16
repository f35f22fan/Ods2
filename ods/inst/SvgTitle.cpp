#include "SvgTitle.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods::inst {

SvgTitle::SvgTitle(Abstract *parent, Tag *tag)
: Abstract (parent, parent->ns(), id::SvgTitle)
{
	if (tag)
		ReadStrings(tag);
}

SvgTitle::SvgTitle(const SvgTitle &cloner)
: Abstract (cloner)
{}

SvgTitle::~SvgTitle() {}

Abstract*
SvgTitle::Clone(Abstract *parent) const
{
	auto *p = new SvgTitle(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	return p;
}

const QString*
SvgTitle::GetFirstString() const
{
	for (StringOrInst *node: nodes_)
	{
		if (node->is_string())
			return node->as_str_ptr();
	}
	
	return nullptr;
}

void SvgTitle::ListKeywords(inst::Keywords &list, const inst::LimitTo lt)
{
	inst::AddKeywords({tag_name()}, list);
}

void SvgTitle::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->svg(), list);
}

void SvgTitle::SetFirstString(const QString &s)
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

void SvgTitle::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
