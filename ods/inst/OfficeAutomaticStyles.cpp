#include "OfficeAutomaticStyles.hpp"

#include "NumberBooleanStyle.hpp"
#include "NumberCurrencyStyle.hpp"
#include "NumberDateStyle.hpp"
#include "NumberPercentageStyle.hpp"
#include "NumberTextStyle.hpp"
#include "NumberTimeStyle.hpp"
#include "StylePageLayout.hpp"
#include "StyleStyle.hpp"

#include "../Book.hpp"
#include "../Ns.hpp"
#include "../ns.hxx"
#include "../style.hxx"
#include "../Tag.hpp"

namespace ods::inst {

OfficeAutomaticStyles::OfficeAutomaticStyles(Abstract *parent, Tag *tag) :
Abstract(parent, parent->ns(), id::OfficeAutomaticStyles)
{
	if (tag)
		Init(tag);
}

OfficeAutomaticStyles::OfficeAutomaticStyles(const OfficeAutomaticStyles &cloner)
: Abstract(cloner)
{}

OfficeAutomaticStyles::~OfficeAutomaticStyles()
{}

Abstract*
OfficeAutomaticStyles::ByStyleName(const QString &name) const
{
	for (StringOrInst *node: nodes_)
	{
		if (!node->is_inst())
			continue;
		
		Abstract *p = node->as_inst();
		
		if (p->IsStyle())
		{
			auto *sn = p->style_name();
			
			if (sn != nullptr && name == *sn)
				return p;
		}
	}
	
	return nullptr;
}

Abstract*
OfficeAutomaticStyles::Clone(Abstract *parent) const
{
	auto *p = new OfficeAutomaticStyles(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->CloneChildrenOf(this);
	
	return p;
}

void OfficeAutomaticStyles::Init(ods::Tag *tag)
{
	Scan(tag);
}

void OfficeAutomaticStyles::ListKeywords(inst::Keywords &list, const inst::LimitTo lt)
{
	inst::AddKeywords({tag_name()}, list);
}

void OfficeAutomaticStyles::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->office(), list);
}

NumberBooleanStyle*
OfficeAutomaticStyles::NewNumberBooleanStyle()
{
	QString new_name = book_->GenUniqueStyleName(style::Family::None,
		id::NumberBooleanStyle);
	auto *p = new NumberBooleanStyle(this);
	p->style_name(new_name);
	Append(p, TakeOwnership::Yes);
	
	return p;
}

NumberCurrencyStyle*
OfficeAutomaticStyles::NewNumberCurrencyStyle()
{
	QString new_name = book_->GenUniqueStyleName(style::Family::None,
		id::NumberCurrencyStyle);
	auto *p = new NumberCurrencyStyle(this);
	p->style_name(new_name);
	Append(p, TakeOwnership::Yes);
	
	return p;
}

NumberDateStyle*
OfficeAutomaticStyles::NewNumberDateStyle()
{
	QString new_name = book_->GenUniqueStyleName(style::Family::None,
		id::NumberDateStyle);
	auto *p = new NumberDateStyle(this);
	p->style_name(new_name);
	Append(p, TakeOwnership::Yes);
	
	return p;
}

NumberPercentageStyle*
OfficeAutomaticStyles::NewNumberPercentageStyle()
{
	QString new_name = book_->GenUniqueStyleName(style::Family::None,
		id::NumberPercentageStyle);
	auto *p = new NumberPercentageStyle(this);
	p->style_name(new_name);
	Append(p, TakeOwnership::Yes);
	
	return p;
}

NumberTimeStyle*
OfficeAutomaticStyles::NewNumberTimeStyle()
{
	QString new_name = book_->GenUniqueStyleName(style::Family::None,
		id::NumberTimeStyle);
	auto *p = new NumberTimeStyle(this);
	p->style_name(new_name);
	Append(p, TakeOwnership::Yes);
	
	return p;
}

StyleStyle*
OfficeAutomaticStyles::NewStyleStyle(const style::Family f)
{
	QString new_name = book_->GenUniqueStyleName(f);
	auto *p = new StyleStyle(this);
	//mtl_info("%s", qPrintable(new_name));
	p->style_name(new_name);
	p->SetFamily(f);
	Append(p, TakeOwnership::Yes);
	
	return p;
}

void
OfficeAutomaticStyles::Scan(ods::Tag *tag)
{
	foreach (auto *x, tag->nodes())
	{
		if (AddText(x))
			continue;
		
		auto *next = x->as_tag();
		
		if (next->Is(ns_->number(), ns::kBooleanStyle)) {
			Append(new NumberBooleanStyle(this, next), TakeOwnership::Yes);
		} else if (next->Is(ns_->number(), ns::kCurrencyStyle)) {
			Append(new NumberCurrencyStyle(this, next), TakeOwnership::Yes);
		} else if (next->Is(ns_->number(), ns::kDateStyle)) {
			Append(new NumberDateStyle(this, next), TakeOwnership::Yes);
		} else if (next->Is(ns_->style(), ns::kPageLayout)) {
			Append(new StylePageLayout(this, next), TakeOwnership::Yes);
		} else if (next->Is(ns_->style(), ns::kStyle)) {
			Append(new StyleStyle(this, next), TakeOwnership::Yes);
		} else if (next->Is(ns_->number(), ns::kPercentageStyle)) {
			Append(new NumberPercentageStyle(this, next), TakeOwnership::Yes);
		} else if (next->Is(ns_->number(), ns::kTextStyle)) {
			Append(new NumberTextStyle(this, next), TakeOwnership::Yes);
		} else if (next->Is(ns_->number(), ns::kTimeStyle)) {
			Append(new NumberTimeStyle(this, next), TakeOwnership::Yes);
		} else {
			Scan(next);
		}
	}
}

void
OfficeAutomaticStyles::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
