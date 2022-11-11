#include "NumberBooleanStyle.hpp"

#include "NumberBoolean.hpp"
#include "NumberText.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

#include "../ndff/Property.hpp"
#include "../ndff/Container.hpp"

namespace ods::inst {

NumberBooleanStyle::NumberBooleanStyle(Abstract *parent, ods::Tag *tag, ndff::Container *cntr)
 : Abstract(parent, parent->ns(), id::NumberBooleanStyle)
{
	if (cntr)
		Init(cntr);
	else if (tag)
		Init(tag);
}

NumberBooleanStyle::NumberBooleanStyle(const NumberBooleanStyle &cloner)
: Abstract(cloner)
{}

NumberBooleanStyle::~NumberBooleanStyle()
{}

Abstract*
NumberBooleanStyle::Clone(Abstract *parent) const
{
	auto *p = new NumberBooleanStyle(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	/*
	number_country_;
	number_language_;
	number_rfc_language_tag_;
	number_script_;
	number_title_;
	number_transliteration_country_;
	number_transliteration_format_;
	number_translitaration_language_;
	number_transliteration_style_;
	style_display_name_;
	style_name_;
	style_volatile_;
	*/
	
	p->number_country_ = number_country_;
	p->number_language_ = number_language_;
	p->number_rfc_language_tag_ = number_rfc_language_tag_;
	p->number_script_ = number_script_;
	p->number_title_ = number_title_;
	p->number_transliteration_country_ = number_transliteration_country_;
	p->number_transliteration_format_ = number_transliteration_format_;
	p->number_translitaration_language_ = number_translitaration_language_;
	p->number_transliteration_style_ = number_transliteration_style_;
	p->style_display_name_ = style_display_name_;
	p->style_name_ = style_name_;
	p->style_volatile_ = style_volatile_;
	
	return p;
}

void NumberBooleanStyle::Init(ndff::Container *cntr)
{
	ndff(true);
	using Op = ndff::Op;
	ndff::Property prop;
	QHash<UriId, QVector<ndff::Property>> attrs;
	Op op = cntr->Next(prop, Op::TS, &attrs);
	
	if (attrs.contains(ns_->number()->id()))
	{
		CopyAttr(attrs, ns_->number(), ods::ns::kCountry, number_country_);
		CopyAttr(attrs, ns_->number(), ods::ns::kLanguage, number_language_);
		CopyAttr(attrs, ns_->number(), ods::ns::kRfcLanguageTag, number_rfc_language_tag_);
		CopyAttr(attrs, ns_->number(), ods::ns::kScript, number_script_);
		CopyAttr(attrs, ns_->number(), ods::ns::kTitle, number_title_);
		CopyAttr(attrs, ns_->number(), ods::ns::kTransliterationCountry, number_transliteration_country_);
		CopyAttr(attrs, ns_->number(), ods::ns::kTransliterationFormat, number_transliteration_format_);
		CopyAttr(attrs, ns_->number(), ods::ns::kTransliterationLanguage, number_translitaration_language_);
		CopyAttr(attrs, ns_->number(), ods::ns::kTransliterationStyle, number_transliteration_style_);
	}
	
	CopyAttr(attrs, ns_->style(), ods::ns::kDisplayName, style_display_name_);
	CopyAttr(attrs, ns_->style(), ods::ns::kName, style_name_);
	CopyAttr(attrs, ns_->style(), ods::ns::kVolatile, style_volatile_);
	
	if (op == Op::N32_TE)
	{
		mtl_info("Op::TE");
		return;
	}
	
	if (op == Op::TCF_CMS)
	{
		mtl_info("Op::TCF");
		op = cntr->Next(prop, op);
	}
	
	while (op == Op::TS)
	{
		if (prop.is(ns_->number()))
		{
			if (prop.name == ns::kBoolean)
				Append(new NumberBoolean(this, 0, cntr), TakeOwnership::Yes);
			else if (prop.name == ns::kText)
				Append(new NumberText(this, 0, cntr), TakeOwnership::Yes);
		}
		
		op = cntr->Next(prop, op);
	}
	
	if (op != Op::SCT)
		mtl_trace("op: %d", op);
}

void NumberBooleanStyle::Init(ods::Tag *tag)
{
	tag->Copy(ns_->number(), ods::ns::kCountry, number_country_);
	tag->Copy(ns_->number(), ods::ns::kLanguage, number_language_);
	tag->Copy(ns_->number(), ods::ns::kRfcLanguageTag, number_rfc_language_tag_);
	tag->Copy(ns_->number(), ods::ns::kScript, number_script_);
	tag->Copy(ns_->number(), ods::ns::kTitle, number_title_);
	tag->Copy(ns_->number(), ods::ns::kTransliterationCountry, number_transliteration_country_);
	tag->Copy(ns_->number(), ods::ns::kTransliterationFormat, number_transliteration_format_);
	tag->Copy(ns_->number(), ods::ns::kTransliterationLanguage, number_translitaration_language_);
	tag->Copy(ns_->number(), ods::ns::kTransliterationStyle, number_transliteration_style_);
	tag->Copy(ns_->style(), ods::ns::kDisplayName, style_display_name_);
	tag->Copy(ns_->style(), ods::ns::kName, style_name_);
	tag->Copy(ns_->style(), ods::ns::kVolatile, style_volatile_);
	
	Scan(tag);
}

void NumberBooleanStyle::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name(), ns::kCountry,
		ns::kLanguage, ns::kRfcLanguageTag,
		ns::kScript, ns::kTitle,
		ns::kTransliterationCountry,
		ns::kTransliterationFormat,
		ns::kTransliterationLanguage,
		ns::kTransliterationStyle,
		ns::kDisplayName, ns::kName, ns::kVolatile}, list);
}

void NumberBooleanStyle::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->number(), list);
	Add(ns_->style(), list);
}

NumberBoolean*
NumberBooleanStyle::NewBoolean()
{
	auto *p = new NumberBoolean(this);
	Append(p, TakeOwnership::Yes);
	return p;
}

NumberText*
NumberBooleanStyle::NewText(const QString &s)
{
	auto *p = new NumberText(this);
	p->SetFirstString(s);
	Append(p, TakeOwnership::Yes);
	return p;
}

void NumberBooleanStyle::Scan(ods::Tag *tag)
{
	for (auto *x: tag->nodes())
	{
		if (AddText(x))
			continue;
		
		auto *next = x->as_tag();
		
		if (next->Is(ns_->number(), ns::kBoolean))
		{
			Append(new NumberBoolean(this, next), TakeOwnership::Yes);
		} else if (next->Is(ns_->number(), ns::kText)) {
			Append(new NumberText(this, next), TakeOwnership::Yes);
		} else {
			Scan(next);
		}
	}
}

void NumberBooleanStyle::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->style(), ods::ns::kName, style_name_);
	Write(xml, ns_->style(), ods::ns::kVolatile, style_volatile_);
	Write(xml, ns_->number(), ods::ns::kCountry, number_country_);
	Write(xml, ns_->number(), ods::ns::kLanguage, number_language_);
	Write(xml, ns_->number(), ods::ns::kRfcLanguageTag, number_rfc_language_tag_);
	Write(xml, ns_->number(), ods::ns::kScript, number_script_);
	Write(xml, ns_->number(), ods::ns::kTitle, number_title_);
	Write(xml, ns_->number(), ods::ns::kTransliterationCountry, number_transliteration_country_);
	Write(xml, ns_->number(), ods::ns::kTransliterationFormat, number_transliteration_format_);
	Write(xml, ns_->number(), ods::ns::kTransliterationLanguage, number_translitaration_language_);
	Write(xml, ns_->number(), ods::ns::kTransliterationStyle, number_transliteration_style_);
	Write(xml, ns_->style(), ods::ns::kDisplayName, style_display_name_);
	
	WriteNodes(xml);
}

void NumberBooleanStyle::WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba)
{
	CHECK_TRUE_VOID(ba != nullptr);
	WriteTag(kw, *ba);
	WriteNdffProp(kw, *ba, ns_->style(), ods::ns::kName, style_name_);
	WriteNdffProp(kw, *ba, ns_->style(), ods::ns::kVolatile, style_volatile_);
	WriteNdffProp(kw, *ba, ns_->number(), ods::ns::kCountry, number_country_);
	WriteNdffProp(kw, *ba, ns_->number(), ods::ns::kLanguage, number_language_);
	WriteNdffProp(kw, *ba, ns_->number(), ods::ns::kRfcLanguageTag, number_rfc_language_tag_);
	WriteNdffProp(kw, *ba, ns_->number(), ods::ns::kScript, number_script_);
	WriteNdffProp(kw, *ba, ns_->number(), ods::ns::kTitle, number_title_);
	WriteNdffProp(kw, *ba, ns_->number(), ods::ns::kTransliterationCountry, number_transliteration_country_);
	WriteNdffProp(kw, *ba, ns_->number(), ods::ns::kTransliterationFormat, number_transliteration_format_);
	WriteNdffProp(kw, *ba, ns_->number(), ods::ns::kTransliterationLanguage, number_translitaration_language_);
	WriteNdffProp(kw, *ba, ns_->number(), ods::ns::kTransliterationStyle, number_transliteration_style_);
	WriteNdffProp(kw, *ba, ns_->style(), ods::ns::kDisplayName, style_display_name_);
	CloseBasedOnChildren(h, kw, file, ba);
}

} // ods::inst::
