#include "NumberTextStyle.hpp"

#include "NumberTextContent.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods::inst {

NumberTextStyle::NumberTextStyle(Abstract *parent, Tag *tag) :
Abstract(parent, parent->ns(), id::NumberTextStyle)
{
	if (tag)
		Init(tag);
}

NumberTextStyle::NumberTextStyle(const NumberTextStyle &cloner)
: Abstract(cloner)
{}

NumberTextStyle::~NumberTextStyle()
{}

Abstract*
NumberTextStyle::Clone(Abstract *parent) const
{
	auto *p = new NumberTextStyle(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->style_name_ = style_name_;
	p->CloneChildrenOf(this);

	return p;
}

void NumberTextStyle::Init(ods::Tag *tag)
{
	tag->Copy(ns_->style(), ns::kName, style_name_);
	Scan(tag);
}

void NumberTextStyle::ListKeywords(inst::Keywords &list, const inst::LimitTo lt)
{
	inst::AddKeywords({tag_name(), ns::kName}, list);
}

void NumberTextStyle::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->number(), list);
	Add(ns_->style(), list);
}

void NumberTextStyle::Scan(ods::Tag *tag)
{
	foreach (auto *x, tag->nodes())
	{
		if (AddText(x))
			continue;
		
		auto *next = x->as_tag();
		
		if (next->Is(ns_->number(), ods::ns::kTextContent))
		{
			Append(new NumberTextContent(this, next), TakeOwnership::Yes);
		} else {
			Scan(next);
		}
	}
}

void NumberTextStyle::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->style(), ns::kName, style_name_);
	WriteNodes(xml);
}

} // ods::inst::
