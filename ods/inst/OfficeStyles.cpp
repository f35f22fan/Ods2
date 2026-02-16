#include "OfficeStyles.hpp"

#include "StyleDefaultStyle.hpp"
#include "NumberCurrencyStyle.hpp"
#include "NumberNumberStyle.hpp"
#include "StyleStyle.hpp"

#include "../Book.hpp"
#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods::inst {

OfficeStyles::OfficeStyles(ods::inst::Abstract *parent, ods::Tag *tag)
: Abstract(parent, parent->ns(), id::OfficeStyles)
{
	if (tag)
		Init(tag);
}

OfficeStyles::OfficeStyles(const OfficeStyles &cloner)
: Abstract(cloner)
{}

OfficeStyles::~OfficeStyles()
{}

Abstract*
OfficeStyles::Clone(Abstract *parent) const
{
	auto *p = new OfficeStyles(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->CloneChildrenOf(this);
	
	return p;
}

void OfficeStyles::Init(ods::Tag *tag)
{
	Scan(tag);
}

void OfficeStyles::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name()}, list);
}

void OfficeStyles::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->office(), list);
}

NumberCurrencyStyle*
OfficeStyles::NewNumberCurrencyStyle()
{
	QString new_name = book_->GenUniqueStyleName(style::Family::None,
		id::NumberCurrencyStyle);
	auto *p = new NumberCurrencyStyle(this);
	p->style_name(new_name);
	Append(p, TakeOwnership::Yes);
	
	return p;
}

StyleStyle*
OfficeStyles::NewStyleStyle(const style::Family f)
{
	QString new_name = book_->GenUniqueStyleName(f);
	auto *p = new StyleStyle(this);
	p->style_name(new_name);
	p->SetFamily(f);
	Append(p, TakeOwnership::Yes);
	
	return p;
}

void
OfficeStyles::Scan(ods::Tag *tag)
{
	for (auto *x: tag->nodes())
	{
		if (!x->is_tag())
			continue;
		
		auto *next = x->as_tag();
		
		if (next->Is(ns_->style(), ns::kDefaultStyle))
		{
			Append(new StyleDefaultStyle(this, next), TakeOwnership::Yes);
		} else if (next->Is(ns_->style(), ns::kStyle)) {
			Append(new StyleStyle(this, next), TakeOwnership::Yes);
		} else if (next->Is(ns_->number(), ns::kNumberStyle)) {
			Append(new NumberNumberStyle(this, next), TakeOwnership::Yes);
		} else if (next->Is(ns_->number(), ns::kCurrencyStyle)) {
			Append(new NumberCurrencyStyle(this, next), TakeOwnership::Yes);
		} else {
			Scan(next);
		}
	}
}

void
OfficeStyles::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
