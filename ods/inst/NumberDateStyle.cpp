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

namespace ods::inst {

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
	p->CloneChildrenOf(this, ClonePart::Text);
	
	return p;
}

void NumberDateStyle::Init(ods::Tag *tag)
{
	tag->Copy(ns_->number(), ns::kAutomaticOrder, number_automatic_order_);
	tag->Copy(ns_->number(), ns::kCountry, number_country_);
	tag->Copy(ns_->number(), ns::kLanguage, number_language_);
	tag->Copy(ns_->style(), ns::kName, style_name_);
	Scan(tag);
}

void NumberDateStyle::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name(), ns::kAutomaticOrder,
		ns::kCountry, ns::kLanguage, ns::kName}, list);
}

void NumberDateStyle::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->number(), list);
	
	if (!style_name_.isEmpty())
		Add(ns_->style(), list);
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

void NumberDateStyle::Scan(ods::Tag *scan_tag)
{
	foreach (auto *x, scan_tag->nodes())
	{
		if (AddText(x))
			continue;
		
		auto *tag = x->as_tag();
		
		if (tag->Has(ns_->number()))
		{
			if (tag->Has(ns::kDay))
				Append(new NumberDay(this, tag));
			else if (tag->Has(ns::kHours))
				Append(new NumberHours(this, tag));
			else if (tag->Has(ns::kMinutes))
				Append(new NumberMinutes(this, tag));
			else if (tag->Has(ns::kMonth))
				Append(new NumberMonth(this, tag));
			else if (tag->Has(ns::kSeconds))
				Append(new NumberSeconds(this, tag));
			else if (tag->Has(ns::kText))
				Append(new NumberText(this, tag));
			else if (tag->Has(ns::kYear))
				Append(new NumberYear(this, tag));
		} else {
			Scan(tag);
		}
	}
}

void NumberDateStyle::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->number(), ns::kAutomaticOrder, number_automatic_order_);
	Write(xml, ns_->number(), ns::kCountry, number_country_);
	Write(xml, ns_->number(), ns::kLanguage, number_language_);
	Write(xml, ns_->style(), ns::kName, style_name_);
	
	WriteNodes(xml);
}

void NumberDateStyle::WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba)
{
	CHECK_TRUE_VOID(ba != nullptr);
	WriteTag(kw, *ba);
	WriteNdffProp(kw, *ba, ns_->number(), ns::kAutomaticOrder, number_automatic_order_);
	WriteNdffProp(kw, *ba, ns_->number(), ns::kCountry, number_country_);
	WriteNdffProp(kw, *ba, ns_->number(), ns::kLanguage, number_language_);
	WriteNdffProp(kw, *ba, ns_->style(), ns::kName, style_name_);
	CloseBasedOnChildren(h, kw, file, ba);
}

} // ods::inst::
