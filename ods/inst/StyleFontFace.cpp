#include "StyleFontFace.hpp"

#include "OfficeFontFaceDecls.hpp"
#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

StyleFontFace::StyleFontFace(Abstract *parent, Tag *tag)
: Abstract(parent, parent->ns(), id::StyleFontFace)
{
	if (tag != nullptr)
		Init(tag);
}

StyleFontFace::StyleFontFace(const StyleFontFace &cloner)
: Abstract(cloner)
{}

StyleFontFace::~StyleFontFace() {}

Abstract*
StyleFontFace::Clone(Abstract *parent) const
{
	auto *p = new StyleFontFace(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->style_name_ = style_name_;
	p->svg_font_family_ = svg_font_family_;
	p->style_font_family_generic_ = style_font_family_generic_;
	p->style_font_pitch_ = style_font_pitch_;
	return p;
}

void
StyleFontFace::font_family_generic(const QString &s)
{
	style_font_family_generic_ = s;
}

void
StyleFontFace::font_pitch(const QString &s)
{
	style_font_pitch_ = s;
}

void
StyleFontFace::Init(ods::Tag *tag)
{
	tag->Copy(ns_->style(), ods::ns::kName, style_name_);
	tag->Copy(ns_->svg(), ods::ns::kFontFamily, svg_font_family_);
	tag->Copy(ns_->style(), ods::ns::kFontFamilyGeneric, style_font_family_generic_);
	tag->Copy(ns_->style(), ods::ns::kFontPitch, style_font_pitch_);
}

bool
StyleFontFace::IsFont(const QString &font_name) const
{
	/*
	@20.269
	The style:font-name attribute specifies a font that is declared by a
	<style:font-face> element with a style:name attribute whose name is
	the same as that of the style:font-name attribute value.
	*/
	return style_name_ == font_name;
}

void
StyleFontFace::Set(const QString &s)
{
	if (s.isEmpty())
		return;
	
	svg_font_family_ = s;
	style_name_ = s;
}

void
StyleFontFace::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->style(), ods::ns::kName, style_name_);
	Write(xml, ns_->svg(), ods::ns::kFontFamily, svg_font_family_);
	Write(xml, ns_->style(), ods::ns::kFontFamilyGeneric, style_font_family_generic_);
	Write(xml, ns_->style(), ods::ns::kFontPitch, style_font_pitch_);
}

} // ods::inst::
} // ods::
