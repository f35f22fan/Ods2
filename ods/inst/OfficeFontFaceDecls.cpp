#include "OfficeFontFaceDecls.hpp"

#include "OfficeDocumentStyles.hpp"
#include "StyleFontFace.hpp"

#include "../ndff/Container.hpp"
#include "../ndff/Property.hpp"
#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods::inst {

static const auto SwissStr = QStringLiteral("swiss");
static const auto VariableStr = QStringLiteral("variable");

OfficeFontFaceDecls::OfficeFontFaceDecls(ods::inst::Abstract *parent, Tag *tag,
	ndff::Container *cntr)
: Abstract(parent, parent->ns(), id::OfficeFontFaceDecls)
{
	if (cntr)
		Init(cntr);
	else if (tag)
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
	
	for (StringOrInst *node: nodes_)
	{
		if (node->Is(Id::StyleFontFace))
		{
			auto *p = (StyleFontFace*)node->as_inst();
			if (p->IsFont(font_name))
				return p;
		}
	}
	
	if (ain == AddIfNeeded::Yes)
		return Register(font_name);
	
	return nullptr;
}

void OfficeFontFaceDecls::Init(ndff::Container *cntr)
{
	mtl_trace();
	using Op = ndff::Op;
	ndff::Property prop;
	Op op = cntr->Next(prop, Op::TS);
	if (op == Op::N32_TE)
	{
mtl_trace();
		return;
	}
	
	if (op == Op::TCF_CMS)
		op = cntr->Next(prop, op);
	
	while (true)
	{
		if (op == Op::TS)
		{
			if (prop.is(ns_->style()))
			{
				if (prop.name == ns::kFontFace) {
					Append(new StyleFontFace(this, 0, cntr), TakeOwnership::Yes);
				} else {
					mtl_trace();
				}
			}
		} else if (ndff::is_text(op)) {
			Append(cntr->NextString());
		} else {
			break;
		}
		
		op = cntr->Next(prop, op);
	}
	
	if (op != Op::SCT)
		mtl_trace("Unexpected op: %d", op);
}

void OfficeFontFaceDecls::Init(ods::Tag *tag)
{
	Scan(tag);
}

void OfficeFontFaceDecls::InitDefault()
{
	auto *sff = new inst::StyleFontFace(this);
	sff->Set(QLatin1String("Liberation Sans"));
	sff->font_family_generic(SwissStr);
	sff->font_pitch(VariableStr);
	Append(sff, TakeOwnership::Yes);
	
	sff = new inst::StyleFontFace(this);
	sff->Set(QLatin1String("DejaVu Sans"));
	sff->font_family_generic(SwissStr);
	sff->font_pitch(VariableStr);
	Append(sff, TakeOwnership::Yes);
	
	sff = new inst::StyleFontFace(this);
	sff->Set(QLatin1String("Lohit Devanagari"));
	sff->font_family_generic(SwissStr);
	sff->font_pitch(VariableStr);
	Append(sff, TakeOwnership::Yes);
	
	sff = new inst::StyleFontFace(this);
	sff->Set(QLatin1String("Noto Sans CJK SC Regular"));
	sff->font_family_generic(SwissStr);
	sff->font_pitch(VariableStr);
	Append(sff, TakeOwnership::Yes);
}

void OfficeFontFaceDecls::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name()}, list);
}

void OfficeFontFaceDecls::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->office(), list);
}

StyleFontFace*
OfficeFontFaceDecls::Register(const QString &font_name)
{
	auto *p = new StyleFontFace(this);
	p->Set(font_name);
	Append(p, TakeOwnership::Yes);
	
	return p;
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
			Append(new StyleFontFace(this, next), TakeOwnership::Yes);
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
