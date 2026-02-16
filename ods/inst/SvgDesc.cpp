#include "SvgDesc.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods::inst {

SvgDesc::SvgDesc(Abstract *parent, Tag * tag)
: Abstract (parent, parent->ns(), id::SvgDesc)
{
	if (tag)
		Init(tag);
}

SvgDesc::SvgDesc(const SvgDesc &cloner)
: Abstract (cloner)
{}

SvgDesc::~SvgDesc() {}

Abstract*
SvgDesc::Clone(Abstract *parent) const
{
	auto *p = new SvgDesc(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	return p;
}

const QString*
SvgDesc::GetFirstString() const
{
	for (StringOrInst *node: nodes_)
	{
		if (node->is_string())
			return node->as_str_ptr();
	}
	
	return nullptr;
}

void SvgDesc::Init(ods::Tag *tag)
{
	ReadStrings(tag);
}

void SvgDesc::ListKeywords(inst::Keywords &list, const inst::LimitTo lt)
{
	inst::AddKeywords({tag_name()}, list);
}

void SvgDesc::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->svg(), list);
}

void SvgDesc::SetFirstString(const QString &s)
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

void SvgDesc::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
