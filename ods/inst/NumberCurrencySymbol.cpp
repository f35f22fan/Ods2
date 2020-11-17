#include "NumberCurrencySymbol.hpp"

#include "../currency.hh"
#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

NumberCurrencySymbol::NumberCurrencySymbol(Abstract *parent, Tag *tag) :
Abstract(parent, parent->ns(), id::NumberCurrencySymbol)
{
	if (tag != nullptr)
		Init(tag);
}

NumberCurrencySymbol::NumberCurrencySymbol(const NumberCurrencySymbol &cloner)
: Abstract(cloner)
{}

NumberCurrencySymbol::~NumberCurrencySymbol()
{}

Abstract*
NumberCurrencySymbol::Clone(Abstract *parent) const
{
	auto *p = new NumberCurrencySymbol(*this);

	if (parent != nullptr)
		p->parent(parent);

	p->number_language_ = number_language_;
	p->number_country_ = number_country_;

	return p;
}

QString
NumberCurrencySymbol::GetSymbol()
{
	for (auto *x: nodes_)
	{
		if (x->is_string())
			return *x->as_string();
	}

	return QLatin1String();
}

void
NumberCurrencySymbol::Init(ods::Tag *tag)
{
	tag->Copy(ns_->number(), ods::ns::kLanguage, number_language_);
	tag->Copy(ns_->number(), ods::ns::kCountry, number_country_);

	ScanString(tag);
}

void
NumberCurrencySymbol::SetSymbol(const ods::Currency &c)
{
	const CurrencyInfo info = ods::currency::info(c.id);

	for (auto *x: nodes_)
	{
		if (x->is_string())
		{
			x->SetString(info.symbol);
			return;
		}
	}

	Append(info.symbol);
}

void
NumberCurrencySymbol::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->number(), ods::ns::kLanguage, number_language_);
	Write(xml, ns_->number(), ods::ns::kCountry, number_country_);

	WriteNodes(xml);
}

} // ods::inst::
} // ods::
