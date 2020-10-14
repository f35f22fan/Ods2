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

namespace ods { // ods::
namespace inst { // ods::inst::

OfficeAutomaticStyles::OfficeAutomaticStyles(Abstract *parent, Tag *tag) :
Abstract(parent, parent->ns(), id::OfficeAutomaticStyles)
{
	if (tag != nullptr)
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
	for (auto *x: nodes_)
	{
		if (!x->is_inst())
			continue;
		
		Abstract *p = x->as_inst();
		
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
	
	return p;
}

void
OfficeAutomaticStyles::Init(ods::Tag *tag)
{
	Scan(tag);
}

NumberBooleanStyle*
OfficeAutomaticStyles::NewNumberBooleanStyle()
{
	QString new_name = book_->GenUniqueStyleName(style::Family::None,
		id::NumberBooleanStyle);
	auto *p = new NumberBooleanStyle(this);
	p->style_name(new_name);
	Append(p);
	
	return p;
}

NumberCurrencyStyle*
OfficeAutomaticStyles::NewNumberCurrencyStyle()
{
	QString new_name = book_->GenUniqueStyleName(style::Family::None,
		id::NumberCurrencyStyle);
	auto *p = new NumberCurrencyStyle(this);
	p->style_name(new_name);
	Append(p);
	
	return p;
}

NumberDateStyle*
OfficeAutomaticStyles::NewNumberDateStyle()
{
	QString new_name = book_->GenUniqueStyleName(style::Family::None,
		id::NumberDateStyle);
	auto *p = new NumberDateStyle(this);
	p->style_name(new_name);
	Append(p);
	
	return p;
}

NumberPercentageStyle*
OfficeAutomaticStyles::NewNumberPercentageStyle()
{
	QString new_name = book_->GenUniqueStyleName(style::Family::None,
		id::NumberPercentageStyle);
	auto *p = new NumberPercentageStyle(this);
	p->style_name(new_name);
	Append(p);
	
	return p;
}

NumberTimeStyle*
OfficeAutomaticStyles::NewNumberTimeStyle()
{
	QString new_name = book_->GenUniqueStyleName(style::Family::None,
		id::NumberTimeStyle);
	auto *p = new NumberTimeStyle(this);
	p->style_name(new_name);
	Append(p);
	
	return p;
}

StyleStyle*
OfficeAutomaticStyles::NewStyleStyle(const style::Family f)
{
	QString new_name = book_->GenUniqueStyleName(f);
	auto *p = new StyleStyle(this);
	p->style_name(new_name);
	p->SetFamily(f);
	Append(p);
	
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
		
		if (next->Is(ns_->number(), ods::ns::kBooleanStyle)) {
			Append(new NumberBooleanStyle(this, next));
		} else if (next->Is(ns_->number(), ods::ns::kCurrencyStyle)) {
			Append(new NumberCurrencyStyle(this, next));
		} else if (next->Is(ns_->number(), ods::ns::kDateStyle)) {
			Append(new NumberDateStyle(this, next));
		} else if (next->Is(ns_->style(), ods::ns::kPageLayout)) {
			Append(new StylePageLayout(this, next));
		} else if (next->Is(ns_->style(), ods::ns::kStyle)) {
			Append(new StyleStyle(this, next));
		} else if (next->Is(ns_->number(), ods::ns::kPercentageStyle)) {
			Append(new NumberPercentageStyle(this, next));
		} else if (next->Is(ns_->number(), ods::ns::kTextStyle)) {
			Append(new NumberTextStyle(this, next));
		} else if (next->Is(ns_->number(), ods::ns::kTimeStyle)) {
			Append(new NumberTimeStyle(this, next));
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
} // ods::
