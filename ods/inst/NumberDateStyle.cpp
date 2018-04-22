#include "NumberDateStyle.hpp"

#include "NumberDay.hpp"
#include "NumberHours.hpp"
#include "NumberMinutes.hpp"
#include "NumberMonth.hpp"
#include "NumberSeconds.hpp"
#include "NumberText.hpp"
#include "NumberYear.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

NumberDateStyle::NumberDateStyle(Abstract *parent, Tag *tag) :
Abstract(parent, parent->ns(), id::NumberDateStyle)
{
	if (tag != nullptr)
		Init(tag);
}

NumberDateStyle::NumberDateStyle(const NumberDateStyle &cloner)
: Abstract(cloner)
{}

NumberDateStyle::~NumberDateStyle()
{}

Abstract*
NumberDateStyle::Clone(Abstract *parent) const
{
	auto *p = new NumberDateStyle(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->number_automatic_order_ = number_automatic_order_;
	p->number_country_ = number_country_;
	p->number_language_ = number_language_;
	p->style_name_ = style_name_;
	
	return p;
}

void
NumberDateStyle::Init(ods::Tag *tag)
{
	tag->Copy(ns_->number(), ods::ns::kAutomaticOrder, number_automatic_order_);
	tag->Copy(ns_->number(), ods::ns::kCountry, number_country_);
	tag->Copy(ns_->number(), ods::ns::kLanguage, number_language_);
	tag->Copy(ns_->style(), ods::ns::kName, style_name_);
	Scan(tag);
}

NumberDay*
NumberDateStyle::NewDay()
{
	auto *p = new NumberDay(this);
	Append(p);
	return p;
}

NumberHours*
NumberDateStyle::NewHours()
{
	auto *p = new NumberHours(this);
	Append(p);
	return p;
}

NumberMinutes*
NumberDateStyle::NewMinutes()
{
	auto *p = new NumberMinutes(this);
	Append(p);
	return p;
}

NumberMonth*
NumberDateStyle::NewMonth()
{
	auto *p = new NumberMonth(this);
	Append(p);
	return p;
}

NumberSeconds*
NumberDateStyle::NewSeconds()
{
	auto *p = new NumberSeconds(this);
	Append(p);
	return p;
}

NumberText*
NumberDateStyle::NewText(const QString &s)
{
	auto *p = new NumberText(this);
	p->SetFirstString(s);
	Append(p);
	return p;
}

NumberYear*
NumberDateStyle::NewYear()
{
	auto *p = new NumberYear(this);
	Append(p);
	return p;
}

void
NumberDateStyle::Scan(ods::Tag *scan_tag)
{
	foreach (auto *x, scan_tag->nodes())
	{
		if (AddText(x))
			continue;
		
		auto *tag = x->as_tag();
		
		if (tag->Has(ns_->number()))
		{
			if (tag->Has(ods::ns::kDay))
				Append(new NumberDay(this, tag));
			else if (tag->Has(ods::ns::kHours))
				Append(new NumberHours(this, tag));
			else if (tag->Has(ods::ns::kMinutes))
				Append(new NumberMinutes(this, tag));
			else if (tag->Has(ods::ns::kMonth))
				Append(new NumberMonth(this, tag));
			else if (tag->Has(ods::ns::kSeconds))
				Append(new NumberSeconds(this, tag));
			else if (tag->Has(ods::ns::kText))
				Append(new NumberText(this, tag));
			else if (tag->Has(ods::ns::kYear))
				Append(new NumberYear(this, tag));
		} else {
			Scan(tag);
		}
	}
}

void
NumberDateStyle::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->number(), ods::ns::kAutomaticOrder, number_automatic_order_);
	Write(xml, ns_->number(), ods::ns::kCountry, number_country_);
	Write(xml, ns_->number(), ods::ns::kLanguage, number_language_);
	Write(xml, ns_->style(), ods::ns::kName, style_name_);
	
	WriteNodes(xml);
}

} // ods::inst::
} // ods::
