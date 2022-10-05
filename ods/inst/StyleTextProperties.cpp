#include "StyleTextProperties.hpp"

#include "OfficeFontFaceDecls.hpp"
#include "StyleFontFace.hpp"

#include "../Book.hpp"
#include "../Length.hpp"
#include "../LineStyle.hpp"
#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

#include "../attr/FoFontStyle.hpp"
#include "../attr/FoFontWeight.hpp"
#include "../attr/StyleTextUnderlineColor.hpp"
#include "../attr/StyleTextUnderlineWidth.hpp"

namespace ods::inst {

StyleTextProperties::StyleTextProperties(Abstract *parent, ods::Tag *tag)
: Abstract(parent, parent->ns(), id::StyleTextProperties)
{
	if (tag != nullptr)
		Init(tag);
}

StyleTextProperties::StyleTextProperties(const StyleTextProperties &cloner)
: Abstract(cloner)
{}

StyleTextProperties::~StyleTextProperties()
{
	delete fo_background_color_;
	delete fo_color_;
	delete fo_font_size_;
	delete fo_font_style_;
	delete fo_font_weight_;
	delete style_font_size_asian_;
	delete style_font_size_complex_;
	delete style_text_underline_color_;
	delete style_text_underline_style_;
	delete style_text_underline_width_;
}

Abstract*
StyleTextProperties::Clone(Abstract *parent) const
{
	auto *p = new StyleTextProperties(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	if (fo_background_color_ != nullptr)
		p->fo_background_color_ = new QColor(*fo_background_color_);
	
	if (fo_color_ != nullptr)
		p->fo_color_ = new QColor(*fo_color_);
	
	p->fo_country_ = fo_country_;
	
	if (fo_font_size_ != nullptr)
		p->fo_font_size_ = fo_font_size_->Clone();
	
	if (style_font_size_asian_ != nullptr)
		p->style_font_size_asian_ = style_font_size_asian_->Clone();
	
	if (style_font_size_complex_ != nullptr)
		p->style_font_size_complex_ = style_font_size_complex_->Clone();
	
	if (fo_font_style_ != nullptr)
		p->fo_font_style_ = fo_font_style_->Clone();
	
	if (fo_font_weight_ != nullptr)
		p->fo_font_weight_ = fo_font_weight_->Clone();
	
	p->fo_hyphenate_ = fo_hyphenate_;
	p->fo_language_ = fo_language_;
	p->style_country_asian_ = style_country_asian_;
	p->style_country_complex_ = style_country_complex_;
	
	//==> Don't create or delete! They're created & deleted in OfficeFontFaceDecls
	if (style_font_name_ != nullptr)
	{
		p->style_font_name_ = (StyleFontFace*)style_font_name_;
	}
	
	if (style_font_name_asian_ != nullptr)
	{
		p->style_font_name_asian_ = (StyleFontFace*) style_font_name_asian_;
	}
	
	if (style_font_name_complex_ != nullptr)
	{
		p->style_font_name_complex_ = (StyleFontFace*)style_font_name_complex_;
	}
	//<== Don't create or delete! They're created & deleted in OfficeFontFaceDecls
	
	if (style_text_underline_style_ != nullptr)
	{
		p->style_text_underline_style_ = (ods::LineStyle*)
			style_text_underline_style_->Clone();
	}
	
	if (style_text_underline_width_ != nullptr)
	{
		p->style_text_underline_width_ = (attr::StyleTextUnderlineWidth*)
			style_text_underline_width_->Clone();
	}
	
	if (style_text_underline_color_ != nullptr)
	{
		p->style_text_underline_color_ = (attr::StyleTextUnderlineColor*)
			style_text_underline_color_->Clone();
	}
	
	p->style_language_asian_ = style_language_asian_;
	p->style_language_complex_ = style_language_complex_;
	p->style_font_family_asian_ = style_font_family_asian_;
	p->style_font_family_complex_ = style_font_family_complex_;
	p->style_font_family_generic_asian_ = style_font_family_generic_asian_;
	p->style_font_family_generic_complex_ = style_font_family_generic_complex_;
	p->style_font_pitch_asian_ = style_font_pitch_asian_;
	p->style_font_pitch_complex_ = style_font_pitch_complex_;
	
	return p;
}

void StyleTextProperties::Init(ods::Tag *tag)
{
	QString str;
	tag->Copy(ns_->fo(), ns::kColor, str);
	
	if (QColor::isValidColor(str))
		fo_color_ = new QColor(str);
	
	tag->Copy(ns_->fo(), ns::kBackgroundColor, str);
	
	if (QColor::isValidColor(str))
		fo_background_color_ = new QColor(str);
	
	tag->Copy(ns_->fo(), ns::kCountry, fo_country_);
	
	tag->Copy(ns_->fo(), ns::kFontSize, str);
	fo_font_size_ = Length::FromString(str);
	
	tag->Copy(ns_->fo(), ns::kFontStyle, str);
	fo_font_style_ = attr::FoFontStyle::FromString(str);
	
	tag->Copy(ns_->fo(), ns::kFontWeight, str);
	fo_font_weight_ = attr::FoFontWeight::FromString(str);
	
	tag->Copy(ns_->style(), ns::kTextUnderlineColor, str);
	style_text_underline_color_ = attr::StyleTextUnderlineColor::FromString(str);
	
	tag->Copy(ns_->style(), ns::kTextUnderlineStyle, str);
	style_text_underline_style_ = ods::LineStyle::FromString(str);
	
	tag->Copy(ns_->style(), ns::kTextUnderlineWidth, str);
	style_text_underline_width_ = attr::StyleTextUnderlineWidth::FromString(str);
	
	tag->Copy(ns_->fo(), ns::kHyphenate, fo_hyphenate_);
	tag->Copy(ns_->fo(), ns::kLanguage, fo_language_);
	
	auto *font_factory = book_->GetFontFaceDecls();
	
	tag->Copy(ns_->style(), ns::kFontName, str);
	style_font_name_ = font_factory->GetFontFace(str, AddIfNeeded::No);
	
	tag->Copy(ns_->style(), ns::kFontNameAsian, str);
	style_font_name_asian_ = font_factory->GetFontFace(str, AddIfNeeded::No);
	
	tag->Copy(ns_->style(), ns::kFontNameComplex, str);
	style_font_name_complex_ = font_factory->GetFontFace(str, AddIfNeeded::No);
	
	tag->Copy(ns_->style(), ns::kLanguageAsian, style_language_asian_);
	tag->Copy(ns_->style(), ns::kCountryAsian, style_country_asian_);
	
	tag->Copy(ns_->style(), ns::kFontSizeAsian, str);
	style_font_size_asian_ = Length::FromString(str);
	
	tag->Copy(ns_->style(), ns::kFontSizeComplex, str);
	style_font_size_complex_ = Length::FromString(str);
	
	tag->Copy(ns_->style(), ns::kLanguageComplex, style_language_complex_);
	tag->Copy(ns_->style(), ns::kCountryComplex, style_country_complex_);
	
	tag->Copy(ns_->style(), ns::kFontFamilyAsian, style_font_family_asian_);
	tag->Copy(ns_->style(), ns::kFontFamilyComplex, style_font_family_complex_);
	tag->Copy(ns_->style(), ns::kFontFamilyGenericAsian, style_font_family_generic_asian_);
	tag->Copy(ns_->style(), ns::kFontFamilyGenericComplex, style_font_family_generic_complex_);
	tag->Copy(ns_->style(), ns::kFontPitchAsian, style_font_pitch_asian_);
	tag->Copy(ns_->style(), ns::kFontPitchComplex, style_font_pitch_complex_);
	
	ScanString(tag);
}

void StyleTextProperties::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name(),
		ns::kColor,
		ns::kBackgroundColor,
		ns::kCountry,
		ns::kFontSize,
		ns::kFontStyle,
		ns::kFontWeight,
		ns::kTextUnderlineColor,
		ns::kTextUnderlineStyle,
		ns::kTextUnderlineWidth,
		ns::kHyphenate,
		ns::kLanguage,
		ns::kFontName,
		ns::kFontNameAsian,
		ns::kFontNameComplex,
		ns::kLanguageAsian,
		ns::kCountryAsian,
		ns::kFontSizeAsian,
		ns::kFontSizeComplex,
		ns::kLanguageComplex,
		ns::kCountryComplex,
		ns::kFontFamilyAsian,
		ns::kFontFamilyComplex,
		ns::kFontFamilyGenericAsian,
		ns::kFontFamilyGenericComplex,
		ns::kFontPitchAsian,
		ns::kFontPitchComplex}, list);
}

void StyleTextProperties::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->fo(), list);
	Add(ns_->style(), list);
}

void StyleTextProperties::SetBackgroundColor(QColor *p)
{
	delete fo_background_color_;
	fo_background_color_ = p;
}

void StyleTextProperties::SetColor(QColor *p)
{
	delete fo_color_;
	fo_color_ = p;
}

void StyleTextProperties::SetFontFace(StyleFontFace *p)
{
	delete style_font_name_;
	style_font_name_ = p;
}

void StyleTextProperties::SetFontSize(Length *p)
{
	delete fo_font_size_;
	fo_font_size_ = p;
}

void StyleTextProperties::SetFontSizeAsian(Length *p)
{
	delete style_font_size_asian_;
	style_font_size_asian_ = p;
}

void StyleTextProperties::SetFontSizeComplex(Length *p)
{
	delete style_font_size_complex_;
	style_font_size_complex_ = p;
}

void StyleTextProperties::SetFontStyle(attr::FoFontStyle *p)
{
	delete fo_font_style_;
	fo_font_style_ = p;
}

void StyleTextProperties::SetFontWeight(attr::FoFontWeight *p)
{
	delete fo_font_weight_;
	fo_font_weight_ = p;
}

void StyleTextProperties::SetTextUnderlineColor(attr::StyleTextUnderlineColor *p)
{
	delete style_text_underline_color_;
	style_text_underline_color_ = p;
}

void StyleTextProperties::SetTextUnderlineStyle(ods::LineStyle *p)
{
	delete style_text_underline_style_;
	style_text_underline_style_ = p;
}

void StyleTextProperties::SetTextUnderlineWidth(attr::StyleTextUnderlineWidth *p)
{
	delete style_text_underline_width_;
	style_text_underline_width_ = p;
}

void StyleTextProperties::WriteData(QXmlStreamWriter &xml)
{
	if (fo_color_ != nullptr)
		Write(xml, ns_->fo(), ns::kColor, fo_color_->name());
	
	if (fo_background_color_ != nullptr)
		Write(xml, ns_->fo(), ns::kBackgroundColor, fo_background_color_->name());
	
	Write(xml, ns_->fo(), ns::kCountry, fo_country_);
	
	if (fo_font_size_ != nullptr && fo_font_size_->is_valid())
		Write(xml, ns_->fo(), ns::kFontSize, fo_font_size_->toString());
	
	if (fo_font_style_ != nullptr && fo_font_style_->is_valid())
		Write(xml, ns_->fo(), ns::kFontStyle, fo_font_style_->toString());
	
	if (fo_font_weight_ != nullptr && fo_font_weight_->is_valid())
		Write(xml, ns_->fo(), ns::kFontWeight, fo_font_weight_->toString());
	
	Write(xml, ns_->fo(), ns::kHyphenate, fo_hyphenate_);
	Write(xml, ns_->fo(), ns::kLanguage, fo_language_);
	
	if (style_font_name_ != nullptr)
		Write(xml, ns_->style(), ns::kFontName, style_font_name_->font_family());
	
	if (style_font_name_asian_ != nullptr)
		Write(xml, ns_->style(), ns::kFontNameAsian, style_font_name_asian_->font_family());
	
	if (style_font_name_complex_ != nullptr)
		Write(xml, ns_->style(), ns::kFontNameComplex, style_font_name_complex_->font_family());
	
	Write(xml, ns_->style(), ns::kLanguageAsian, style_language_asian_);
	Write(xml, ns_->style(), ns::kCountryAsian, style_country_asian_);
	
	if (style_font_size_asian_ != nullptr && style_font_size_asian_->is_valid())
		Write(xml, ns_->style(), ns::kFontSizeAsian, style_font_size_asian_->toString());
	
	if (style_font_size_complex_ != nullptr && style_font_size_complex_->is_valid())
		Write(xml, ns_->style(), ns::kFontSizeComplex, style_font_size_complex_->toString());
	
	if (style_text_underline_color_ != nullptr && style_text_underline_color_->is_valid())
		Write(xml, ns_->style(), ns::kTextUnderlineColor, style_text_underline_color_->toString());
	
	if (style_text_underline_style_ != nullptr && style_text_underline_style_->is_valid())
		Write(xml, ns_->style(), ns::kTextUnderlineStyle, style_text_underline_style_->toString());
	
	if (style_text_underline_width_ != nullptr && style_text_underline_width_->is_valid())
		Write(xml, ns_->style(), ns::kTextUnderlineWidth, style_text_underline_width_->toString());
	
	Write(xml, ns_->style(), ns::kLanguageComplex, style_language_complex_);
	Write(xml, ns_->style(), ns::kCountryComplex, style_country_complex_);
	
	Write(xml, ns_->style(), ns::kFontFamilyAsian, style_font_family_asian_);
	Write(xml, ns_->style(), ns::kFontFamilyComplex, style_font_family_complex_);
	Write(xml, ns_->style(), ns::kFontFamilyGenericAsian, style_font_family_generic_asian_);
	Write(xml, ns_->style(), ns::kFontFamilyGenericComplex, style_font_family_generic_complex_);
	Write(xml, ns_->style(), ns::kFontPitchAsian, style_font_pitch_asian_);
	Write(xml, ns_->style(), ns::kFontPitchComplex, style_font_pitch_complex_);
	
	WriteNodes(xml);
}

void StyleTextProperties::WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba)
{
	CHECK_TRUE_VOID(ba != nullptr);
	WriteTag(kw, *ba);
	if (fo_color_ != nullptr)
		WriteNdffProp(kw, *ba, ns_->fo(), ns::kColor, fo_color_->name());
	
	if (fo_background_color_ != nullptr)
		WriteNdffProp(kw, *ba, ns_->fo(), ns::kBackgroundColor, fo_background_color_->name());
	
	WriteNdffProp(kw, *ba, ns_->fo(), ns::kCountry, fo_country_);
	
	if (fo_font_size_ != nullptr && fo_font_size_->is_valid())
		WriteNdffProp(kw, *ba, ns_->fo(), ns::kFontSize, fo_font_size_->toString());
	
	if (fo_font_style_ != nullptr && fo_font_style_->is_valid())
		WriteNdffProp(kw, *ba, ns_->fo(), ns::kFontStyle, fo_font_style_->toString());
	
	if (fo_font_weight_ != nullptr && fo_font_weight_->is_valid())
		WriteNdffProp(kw, *ba, ns_->fo(), ns::kFontWeight, fo_font_weight_->toString());
	
	WriteNdffProp(kw, *ba, ns_->fo(), ns::kHyphenate, fo_hyphenate_);
	WriteNdffProp(kw, *ba, ns_->fo(), ns::kLanguage, fo_language_);
	
	if (style_font_name_ != nullptr)
		WriteNdffProp(kw, *ba, ns_->style(), ns::kFontName, style_font_name_->font_family());
	
	if (style_font_name_asian_ != nullptr)
		WriteNdffProp(kw, *ba, ns_->style(), ns::kFontNameAsian, style_font_name_asian_->font_family());
	
	if (style_font_name_complex_ != nullptr)
		WriteNdffProp(kw, *ba, ns_->style(), ns::kFontNameComplex, style_font_name_complex_->font_family());
	
	WriteNdffProp(kw, *ba, ns_->style(), ns::kLanguageAsian, style_language_asian_);
	WriteNdffProp(kw, *ba, ns_->style(), ns::kCountryAsian, style_country_asian_);
	
	if (style_font_size_asian_ != nullptr && style_font_size_asian_->is_valid())
		WriteNdffProp(kw, *ba, ns_->style(), ns::kFontSizeAsian, style_font_size_asian_->toString());
	
	if (style_font_size_complex_ != nullptr && style_font_size_complex_->is_valid())
		WriteNdffProp(kw, *ba, ns_->style(), ns::kFontSizeComplex, style_font_size_complex_->toString());
	
	if (style_text_underline_color_ != nullptr && style_text_underline_color_->is_valid())
		WriteNdffProp(kw, *ba, ns_->style(), ns::kTextUnderlineColor, style_text_underline_color_->toString());
	
	if (style_text_underline_style_ != nullptr && style_text_underline_style_->is_valid())
		WriteNdffProp(kw, *ba, ns_->style(), ns::kTextUnderlineStyle, style_text_underline_style_->toString());
	
	if (style_text_underline_width_ != nullptr && style_text_underline_width_->is_valid())
		WriteNdffProp(kw, *ba, ns_->style(), ns::kTextUnderlineWidth, style_text_underline_width_->toString());
	
	WriteNdffProp(kw, *ba, ns_->style(), ns::kLanguageComplex, style_language_complex_);
	WriteNdffProp(kw, *ba, ns_->style(), ns::kCountryComplex, style_country_complex_);
	WriteNdffProp(kw, *ba, ns_->style(), ns::kFontFamilyAsian, style_font_family_asian_);
	WriteNdffProp(kw, *ba, ns_->style(), ns::kFontFamilyComplex, style_font_family_complex_);
	WriteNdffProp(kw, *ba, ns_->style(), ns::kFontFamilyGenericAsian, style_font_family_generic_asian_);
	WriteNdffProp(kw, *ba, ns_->style(), ns::kFontFamilyGenericComplex, style_font_family_generic_complex_);
	WriteNdffProp(kw, *ba, ns_->style(), ns::kFontPitchAsian, style_font_pitch_asian_);
	WriteNdffProp(kw, *ba, ns_->style(), ns::kFontPitchComplex, style_font_pitch_complex_);
	CloseBasedOnChildren(h, kw, file, ba);
}

} // ods::inst::
