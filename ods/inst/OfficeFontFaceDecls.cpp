#include "OfficeFontFaceDecls.hpp"

#include "OfficeDocumentStyles.hpp"
#include "StyleFontFace.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

static const auto SwissStr = QStringLiteral("swiss");
static const auto VariableStr = QStringLiteral("variable");

OfficeFontFaceDecls::OfficeFontFaceDecls(ods::inst::Abstract *parent, Tag *tag)
: Abstract(parent, parent->ns(), id::OfficeFontFaceDecls)
{
	if (tag != nullptr)
		Init(tag);
	else
		InitDefault();
}

OfficeFontFaceDecls::OfficeFontFaceDecls(const OfficeFontFaceDecls &cloner)
: Abstract(cloner)
{}

OfficeFontFaceDecls::~OfficeFontFaceDecls()
{}

Abstract*
OfficeFontFaceDecls::Clone(Abstract *parent) const
{
	auto *p = new OfficeFontFaceDecls(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	return p;
}

StyleFontFace*
OfficeFontFaceDecls::GetFontFace(const QString &font_name, const AddIfNeeded ain)
{
	if (font_name.isEmpty())
		return nullptr;
	
	for (auto *x: nodes_)
	{
		if (x->Is(id::StyleFontFace))
		{
			auto *p = (StyleFontFace*)x->as_inst();

			if (p->IsFont(font_name))
				return p;
		}
	}
	
	if (ain == AddIfNeeded::Yes)
		return Register(font_name);
	
	return nullptr;
}

void
OfficeFontFaceDecls::Init(ods::Tag *tag)
{
	Scan(tag);
}

void
OfficeFontFaceDecls::InitDefault()
{
	auto *sff = new inst::StyleFontFace(this);
	sff->Set(QLatin1String("Liberation Sans"));
	sff->font_family_generic(SwissStr);
	sff->font_pitch(VariableStr);
	Append(sff);
	
	sff = new inst::StyleFontFace(this);
	sff->Set(QLatin1String("DejaVu Sans"));
	sff->font_family_generic(SwissStr);
	sff->font_pitch(VariableStr);
	Append(sff);
		
	sff = new inst::StyleFontFace(this);
	sff->Set(QLatin1String("Lohit Devanagari"));
	sff->font_family_generic(SwissStr);
	sff->font_pitch(VariableStr);
	Append(sff);
	
	sff = new inst::StyleFontFace(this);
	sff->Set(QLatin1String("Noto Sans CJK SC Regular"));
	sff->font_family_generic(SwissStr);
	sff->font_pitch(VariableStr);
	Append(sff);
}

StyleFontFace*
OfficeFontFaceDecls::Register(const QString &font_name)
{
	auto *sff = new StyleFontFace(this);
	sff->Set(font_name);
	Append(sff);
	return sff;
}

void
OfficeFontFaceDecls::Scan(ods::Tag *tag)
{
	foreach (auto *x, tag->nodes())
	{
		if (!x->is_tag())
			continue;
		
		auto *next = x->as_tag();
		
		if (next->Is(ns_->style(), ods::ns::kFontFace))
		{
			Append(new StyleFontFace(this, next));
		} else {
			Scan(next);
		}
	}
}

void
OfficeFontFaceDecls::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
} // ods::
