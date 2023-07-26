#include "NumberCurrencySymbol.hpp"

#include "../currency.hh"
#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

#include "../ndff/Container.hpp"
#include "../ndff/Property.hpp"

namespace ods::inst {

NumberCurrencySymbol::NumberCurrencySymbol(Abstract *parent, Tag *tag,
ndff::Container *cntr) :
Abstract(parent, parent->ns(), id::NumberCurrencySymbol)
{
	if (cntr)
		Init(cntr);
	else if (tag)
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
	p->CloneChildrenOf(this, ClonePart::Text);
	
	return p;
}

QString
NumberCurrencySymbol::GetSymbol()
{
	for (StringOrInst *node: nodes_)
	{
		if (node->is_string())
			return node->as_string();
	}

	return QString();
}

void NumberCurrencySymbol::Init(ndff::Container *cntr)
{
	// number:currency-symbol(1.2) => has attrs, no children, has char data
	using Op = ndff::Op;
	ndff::Property prop;
	QHash<UriId, QVector<ndff::Property>> attrs;
	Op op = cntr->Next(prop, Op::TS, &attrs);
	CopyAttr(attrs, ns_->number(), ns::kLanguage, number_language_);
	CopyAttr(attrs, ns_->number(), ns::kCountry, number_country_);
	CopyAttr(attrs, ns_->number(), ns::kRfcLanguageTag, number_rfc_language_tag_);
	CopyAttr(attrs, ns_->number(), ns::kScript, number_script_);
	ReadStrings(cntr, op);
}

void NumberCurrencySymbol::Init(ods::Tag *tag)
{
	tag->Copy(ns_->number(), ns::kLanguage, number_language_);
	tag->Copy(ns_->number(), ns::kCountry, number_country_);
	tag->Copy(ns_->number(), ns::kRfcLanguageTag, number_rfc_language_tag_);
	tag->Copy(ns_->number(), ns::kScript, number_script_);

	ReadStrings(tag);
}

void NumberCurrencySymbol::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name(), ns::kLanguage, ns::kCountry}, list);
}

void NumberCurrencySymbol::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->number(), list);
}

void NumberCurrencySymbol::SetSymbol(const ods::Currency &c)
{
	const CurrencyInfo info = ods::currency::info(c.id);

	for (StringOrInst *node: nodes_)
	{
		if (node->is_string())
		{
			node->SetString(info.symbol);
			return;
		}
	}

	Append(info.symbol);
}

void NumberCurrencySymbol::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->number(), ns::kLanguage, number_language_);
	Write(xml, ns_->number(), ns::kCountry, number_country_);
	Write(xml, ns_->number(), ns::kRfcLanguageTag, number_rfc_language_tag_);
	Write(xml, ns_->number(), ns::kScript, number_script_);

	WriteNodes(xml);
}

void NumberCurrencySymbol::WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba)
{
	MTL_CHECK_VOID(ba != nullptr);
	WriteTag(kw, *ba);
	WriteNdffProp(kw, *ba, ns_->number(), ns::kLanguage, number_language_);
	WriteNdffProp(kw, *ba, ns_->number(), ns::kCountry, number_country_);
	WriteNdffProp(kw, *ba, ns_->number(), ns::kRfcLanguageTag, number_rfc_language_tag_);
	WriteNdffProp(kw, *ba, ns_->number(), ns::kScript, number_script_);
	CloseBasedOnChildren(h, kw, file, ba);
}

} // ods::inst::
