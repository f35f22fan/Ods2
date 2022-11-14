#include "StyleFontFace.hpp"

#include "../ndff/Container.hpp"
#include "../ndff/Property.hpp"
#include "OfficeFontFaceDecls.hpp"
#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods::inst {

StyleFontFace::StyleFontFace(Abstract *parent, Tag *tag, ndff::Container *cntr)
: Abstract(parent, parent->ns(), id::StyleFontFace)
{
	if (cntr)
		Init(cntr);
	else if (tag)
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

void StyleFontFace::font_family_generic(const QString &s)
{
	style_font_family_generic_ = s;
}

void StyleFontFace::font_pitch(const QString &s)
{
	style_font_pitch_ = s;
}

void StyleFontFace::Init(ndff::Container *cntr)
{
	using Op = ndff::Op;
	ndff::Property prop;
	QHash<UriId, QVector<ndff::Property>> attrs;
	Op op = cntr->Next(prop, Op::TS, &attrs);
	CopyAttr(attrs, ns_->style(), ns::kName, style_name_);
	CopyAttr(attrs, ns_->svg(), ns::kFontFamily, svg_font_family_);
	CopyAttr(attrs, ns_->style(), ns::kFontFamilyGeneric, style_font_family_generic_);
	CopyAttr(attrs, ns_->style(), ns::kFontPitch, style_font_pitch_);
	ReadStrings(cntr, op);
}

void StyleFontFace::Init(ods::Tag *tag)
{
	tag->Copy(ns_->style(), ns::kName, style_name_);
	tag->Copy(ns_->svg(), ns::kFontFamily, svg_font_family_);
	tag->Copy(ns_->style(), ns::kFontFamilyGeneric, style_font_family_generic_);
	tag->Copy(ns_->style(), ns::kFontPitch, style_font_pitch_);
}

bool StyleFontFace::IsFont(const QString &font_name) const
{
	/*
	@20.269
	The style:font-name attribute specifies a font that is declared by a
	<style:font-face> element with a style:name attribute whose name is
	the same as that of the style:font-name attribute value.
	*/
	return style_name_ == font_name;
}

void StyleFontFace::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name(), ns::kName, ns::kFontFamily,
		ns::kFontFamilyGeneric, ns::kFontPitch}, list);
}

void StyleFontFace::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->style(), list);
}

void StyleFontFace::Set(const QString &s)
{
	if (s.isEmpty())
		return;
	
	svg_font_family_ = s;
	style_name_ = s;
}

void StyleFontFace::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->style(), ns::kName, style_name_);
	Write(xml, ns_->svg(), ns::kFontFamily, svg_font_family_);
	Write(xml, ns_->style(), ns::kFontFamilyGeneric, style_font_family_generic_);
	Write(xml, ns_->style(), ns::kFontPitch, style_font_pitch_);
}

void StyleFontFace::WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba)
{
	CHECK_TRUE_VOID(ba != nullptr);
	WriteTag(kw, *ba);
	WriteNdffProp(kw, *ba, ns_->style(), ns::kName, style_name_);
	WriteNdffProp(kw, *ba, ns_->svg(), ns::kFontFamily, svg_font_family_);
	WriteNdffProp(kw, *ba, ns_->style(), ns::kFontFamilyGeneric, style_font_family_generic_);
	WriteNdffProp(kw, *ba, ns_->style(), ns::kFontPitch, style_font_pitch_);
	CloseBasedOnChildren(h, kw, file, ba);
}

} // ods::inst::
