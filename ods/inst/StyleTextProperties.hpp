#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../decl.hxx"
#include "../err.hpp"
#include "../attr/decl.hxx"
#include "../Color.hpp"

namespace ods::inst {

class ODS_API StyleTextProperties : public ods::inst::Abstract
{
public:
	StyleTextProperties(Abstract *parent, ods::Tag *tag = 0,
		ndff::Container *cntr = 0);
	StyleTextProperties(const StyleTextProperties &cloner);
	virtual ~StyleTextProperties();
	
	const Color&
	background_color() const { return fo_background_color_; }
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	const Color&
	color() const { return fo_color_; }
	
	StyleFontFace*
	font_face() const { return style_font_name_; }
	
	Length*
	font_size() const { return fo_font_size_; }
	
	Length*
	font_size_asian() const { return style_font_size_asian_; }

	Length*
	font_size_complex() const { return style_font_size_complex_; }
	
	attr::FoFontStyle*
	font_style() const { return fo_font_style_; }
	
	attr::FoFontWeight*
	font_weight() const { return fo_font_weight_; }
	
	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	
	void SetBackgroundColor(const Color &c);
	void SetColor(const Color &c);
	void SetFontFace(StyleFontFace *sff);
	void SetFontSize(Length *fs);
	void SetFontSizeAsian(Length *fs);
	void SetFontSizeComplex(Length *fs);
	void SetFontStyle(attr::FoFontStyle *fs);
	void SetFontWeight(attr::FoFontWeight *fw);
	void SetTextUnderlineColor(attr::StyleTextUnderlineColor *stuc);
	void SetTextUnderlineStyle(LineStyle *stus);
	void SetTextUnderlineWidth(attr::StyleTextUnderlineWidth *stuw);
	
	attr::StyleTextUnderlineColor*
	text_underline_color() const { return style_text_underline_color_; }
	
	ods::LineStyle*
	text_underline_style() const { return style_text_underline_style_; }
	
	attr::StyleTextUnderlineWidth*
	text_underline_width() const { return style_text_underline_width_; }
	
	void WriteData(QXmlStreamWriter &xml) override;
	void WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba) override;
private:

	void Init(ndff::Container *cntr);
	void Init(ods::Tag *tag);
	
	Color fo_background_color_ = {};
	Color fo_color_ = {};
	
	QString fo_country_;
	
	ods::Length *fo_font_size_ = nullptr;
	ods::Length *style_font_size_asian_ = nullptr;
	ods::Length *style_font_size_complex_ = nullptr;
	
	attr::FoFontStyle *fo_font_style_ = nullptr;
	attr::FoFontWeight *fo_font_weight_ = nullptr;
	
	//fo:hyphenate="true"
	QString fo_hyphenate_;
	QString fo_language_;
	QString style_country_asian_;
	QString style_country_complex_;
	
	//==> Don't create or delete! They're managed by OfficeFontFaceDecls
	StyleFontFace *style_font_name_ = nullptr;
	StyleFontFace *style_font_name_asian_ = nullptr;
	StyleFontFace *style_font_name_complex_ = nullptr;
	//<== Don't create or delete! They're managed by OfficeFontFaceDecls
	
	// style:text-underline-style="solid"
	ods::LineStyle *style_text_underline_style_ = nullptr;
	// style:text-underline-width="auto"
	attr::StyleTextUnderlineWidth *style_text_underline_width_ = nullptr;
	// style:text-underline-color="font-color"
	attr::StyleTextUnderlineColor *style_text_underline_color_ = nullptr;
	
	QString style_language_asian_;
	QString style_language_complex_;
	
	QString style_font_family_asian_;
	QString style_font_family_complex_;
	QString style_font_family_generic_asian_;
	QString style_font_family_generic_complex_;
	QString style_font_pitch_asian_;
	QString style_font_pitch_complex_;
};

} // ods::inst::
