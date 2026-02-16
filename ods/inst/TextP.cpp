#include "TextP.hpp"

#include "TextDate.hpp"
#include "TextPageCount.hpp"
#include "TextPageNumber.hpp"
#include "TextS.hpp"
#include "TextSheetName.hpp"
#include "TextTime.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods::inst {

TextP::TextP(Abstract *parent, ods::Tag *tag)
: Abstract(parent, parent->ns(), id::TextP)
{
	if (tag)
		Init(tag);
}

TextP::TextP(const TextP &cloner) : Abstract(cloner)
{}

TextP::~TextP()
{}

void TextP::AppendString(const QString &s)
{
	for (StringOrInst *node: nodes_)
	{
		if (node->is_string())
		{
			node->AppendString(s);
			return;
		}
	}
	
	Append(s);
}

Abstract*
TextP::Clone(Abstract *parent) const
{
	auto *p = new TextP(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	return p;
}

const QString*
TextP::GetFirstString() const
{
	for (StringOrInst *node: nodes_)
	{
		if (node->is_string())
			return node->as_str_ptr();
	}
	
	return nullptr;
}

void TextP::Init(ods::Tag *tag)
{
	Scan(tag);
}

void TextP::ListKeywords(Keywords &list, const LimitTo lt)
{
	AddKeywords({tag_name()}, list);
}

void TextP::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->text(), list);
}

void TextP::Scan(ods::Tag *tag)
{
	for (StringOrTag *node: tag->nodes())
	{
		if (AddText(node))
			continue;
		
		Tag *next = node->as_tag();
		
		if (next->Is(ns_->text(), ns::kPageNumber)) {
			Append(new TextPageNumber(this, next), TakeOwnership::Yes);
		} else if (next->Is(ns_->text(), ns::kS)) {
			Append(new TextS(this, next), TakeOwnership::Yes);
		} else if (next->Is(ns_->text(), ns::kSheetName)) {
			Append(new TextSheetName(this, next), TakeOwnership::Yes);
		} else if (next->Is(ns_->text(), ns::kPageCount)) {
			Append(new TextPageCount(this, next), TakeOwnership::Yes);
		} else if (next->Is(ns_->text(), ns::kTime)) {
			Append(new TextTime(this, next), TakeOwnership::Yes);
		} else if (next->Is(ns_->text(), ns::kDate)) {
			Append(new TextDate(this, next), TakeOwnership::Yes);
		} else {
			Scan(next);
		}
	}
}

void TextP::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::

