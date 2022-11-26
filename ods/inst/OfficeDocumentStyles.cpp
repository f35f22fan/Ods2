#include "OfficeDocumentStyles.hpp"

#include "NumberCurrencyStyle.hpp"
#include "OfficeAutomaticStyles.hpp"
#include "OfficeFontFaceDecls.hpp"
#include "OfficeMasterStyles.hpp"
#include "OfficeStyles.hpp"
#include "StyleFontFace.hpp"

#include "../Book.hpp"
#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

#include <QFile>
#include <QXmlStreamReader>

namespace ods::inst {

OfficeDocumentStyles::OfficeDocumentStyles(ods::Book *book, ods::Ns *ns, Tag *tag, ndff::Container *cntr) :
Abstract(nullptr, ns, id::OfficeDocumentStyles)
{
	book_ = book;
	book_->document_styles_ = this;
	
	if (cntr)
		Init(cntr);
	else if (tag)
		Init(tag);
	else
		InitDefault();
}

OfficeDocumentStyles::OfficeDocumentStyles(const OfficeDocumentStyles &cloner)
: Abstract(cloner)
{}

OfficeDocumentStyles::~OfficeDocumentStyles()
{
	delete office_font_face_decls_;
	delete office_styles_;
	delete office_automatic_styles_;
	delete office_master_styles_;
}

Abstract*
OfficeDocumentStyles::Clone(Abstract *parent) const
{
	auto *p = new OfficeDocumentStyles(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->office_version_ = office_version_;
	
	if (office_automatic_styles_ != nullptr)
	{
		p->office_automatic_styles_ = (OfficeAutomaticStyles*)
			office_automatic_styles_->Clone();
	}
	
	if (office_font_face_decls_ != nullptr)
	{
		p->office_font_face_decls_ = (OfficeFontFaceDecls*)
			office_font_face_decls_->Clone();
	}
	
	if (office_styles_ != nullptr)
	{
		p->office_styles_ = (OfficeStyles*) office_styles_->Clone();
	}
	
	if (office_master_styles_ != nullptr)
	{
		p->office_master_styles_ = (OfficeMasterStyles*)
			office_master_styles_->Clone();
	}
	
	return p;
}

Abstract*
OfficeDocumentStyles::GetAnyStyle(const QString &name)
{
	auto *a = office_automatic_styles_->GetStyleRecursive(name);
	
	if (a != nullptr)
		return a;
	
	a = office_styles_->GetStyleRecursive(name);
	
	if (a != nullptr)
		return a;
	
	a = office_master_styles_->GetStyleRecursive(name);
	
	if (a != nullptr)
		return a;
	
	return office_font_face_decls_->GetStyleRecursive(name);
}

void OfficeDocumentStyles::Init(ndff::Container *cntr)
{
	using Op = ndff::Op;
	ndff::Property prop;
	NdffAttrs attrs;
	Op op = cntr->Next(prop, Op::None);
	op = cntr->Next(prop, op, &attrs);
	CopyAttr(attrs, ns_->office(), ns::kVersion, office_version_);
//	mtl_info("office_version_: %s", qPrintable(office_version_));
	if (op == Op::N32_TE)
		return;
	
	if (op == Op::TCF_CMS)
		op = cntr->Next(prop, op);
	
	while (true)
	{
		if (op == Op::TS)
		{
			if (prop.is(ns_->office()))
			{
				if (prop.name == ns::kFontFaceDecls) {
					office_font_face_decls_ = new OfficeFontFaceDecls(this, 0, cntr);
				} else if (prop.name == ns::kStyles) {
					office_styles_ = new OfficeStyles(this, 0, cntr);
				} else if (prop.name == ns::kAutomaticStyles) {
					office_automatic_styles_ = new OfficeAutomaticStyles(this, 0, cntr);
				} else if (prop.name == ns::kMasterStyles) {
					office_master_styles_ = new OfficeMasterStyles(this, 0, cntr);
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

void OfficeDocumentStyles::Init(ods::Tag *tag)
{
	tag->Copy(ns_->office(), ns::kVersion, office_version_);
	Scan(tag);
}

void OfficeDocumentStyles::InitDefault()
{
	office_version_ = QLatin1String("1.2");
	
	office_automatic_styles_ = new OfficeAutomaticStyles(this);
	office_font_face_decls_ = new OfficeFontFaceDecls(this);
	office_styles_ = new OfficeStyles(this);
	office_master_styles_ = new OfficeMasterStyles(this);
}

void OfficeDocumentStyles::ListChildren(QVector<StringOrInst*> &vec,
	const Recursively r)
{
	if (office_font_face_decls_)
	{
		vec.append(new StringOrInst(office_font_face_decls_, TakeOwnership::No));
		if (r == Recursively::Yes)
			office_font_face_decls_->ListChildren(vec, r);
	}
	
	if (office_styles_)
	{
		vec.append(new StringOrInst(office_styles_, TakeOwnership::No));
		if (r == Recursively::Yes)
			office_styles_->ListChildren(vec, r);
	}
	
	if (office_automatic_styles_)
	{
		vec.append(new StringOrInst(office_automatic_styles_, TakeOwnership::No));
		if (r == Recursively::Yes)
			office_automatic_styles_->ListChildren(vec, r);
	}
	
	if (office_master_styles_)
	{
		vec.append(new StringOrInst(office_master_styles_, TakeOwnership::No));
		if (r == Recursively::Yes)
			office_master_styles_->ListChildren(vec, r);
	}
}

void OfficeDocumentStyles::ListKeywords(Keywords &list, const LimitTo lt)
{
	AddKeywords({tag_name(), ns::kVersion}, list);
}

void OfficeDocumentStyles::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->office(), list);
}

void OfficeDocumentStyles::Scan(ods::Tag *tag)
{
	foreach (auto *x, tag->nodes())
	{
		if (!x->is_tag())
			continue;
		
		auto *next = x->as_tag();
		
		if (next->Is(ns_->office(), ns::kFontFaceDecls))
		{
			office_font_face_decls_ = new OfficeFontFaceDecls(this, next);
		} else if (next->Is(ns_->office(), ns::kStyles)) {
			office_styles_ = new OfficeStyles(this, next);
		} else if (next->Is(ns_->office(), ns::kAutomaticStyles)) {
			office_automatic_styles_ = new OfficeAutomaticStyles(this, next);
		} else if (next->Is(ns_->office(), ns::kMasterStyles)) {
			office_master_styles_ = new OfficeMasterStyles(this, next);
		} else {
			Scan(next);
		}
	}
}

void OfficeDocumentStyles::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->office(), ns::kVersion, office_version_);
	
	office_font_face_decls_->Write(xml);
	office_styles_->Write(xml);
	office_automatic_styles_->Write(xml);
	office_master_styles_->Write(xml);
}

void OfficeDocumentStyles::WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba)
{
	CHECK_TRUE_VOID(ba != nullptr);
	WriteTag(kw, *ba);
	WriteNdffProp(kw, *ba, ns_->office(), ns::kVersion, office_version_);
	CloseBasedOnChildren(h, kw, file, ba);
}

} // ods::inst::
