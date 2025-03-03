#include "OfficeDocumentContent.hpp"

#include "OfficeAutomaticStyles.hpp"
#include "OfficeBody.hpp"
#include "OfficeFontFaceDecls.hpp"
#include "OfficeScripts.hpp"

#include "../Book.hpp"
#include "../ByteArray.hpp"
#include "../filename.hxx"
#include "../ndff/Container.hpp"
#include "../ndff/Property.hpp"
#include "../Ns.hpp"
#include "../ns.hxx"
#include "../ods.hh"
#include "../Sheet.hpp"
#include "../Tag.hpp"

namespace ods::inst {

OfficeDocumentContent::OfficeDocumentContent
(ods::Book *book, ods::Ns *ns, ods::Tag *tag, ndff::Container *cntr)
: Abstract(nullptr, ns, id::OfficeDocumentContent)
{
	book_ = book;
	book_->document_content_ = this;
	
	if (cntr)
		Init(cntr);
	else if (tag)
		Init(tag);
	else
		InitDefault();
}

OfficeDocumentContent::OfficeDocumentContent(const OfficeDocumentContent &cloner)
: Abstract(cloner)
{}

OfficeDocumentContent::~OfficeDocumentContent()
{
	delete office_automatic_styles_;
	delete office_body_;
	delete office_font_face_decls_;
	delete office_scripts_;
}

Abstract*
OfficeDocumentContent::Clone(Abstract *parent) const
{
	auto *p = new OfficeDocumentContent(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	if (office_automatic_styles_ != nullptr)
	{
		p->office_automatic_styles_ =
			(OfficeAutomaticStyles*)office_automatic_styles_->Clone();
	}
	
	if (office_body_ != nullptr)
	{
		p->office_body_ = (OfficeBody*)office_body_->Clone();
	}
	
	if (office_font_face_decls_ != nullptr)
	{
		p->office_font_face_decls_ = (OfficeFontFaceDecls*)
			office_font_face_decls_->Clone();
	}
	
	if (office_scripts_)
	{
		p->office_scripts_ = (OfficeScripts*) office_scripts_->Clone();
	}
	
	return p;
}

Abstract*
OfficeDocumentContent::GetAnyStyle(const QString &name)
{
	auto *a = office_automatic_styles_->GetStyleRecursive(name);
	return a ? a : office_font_face_decls_->GetStyleRecursive(name);
}

void OfficeDocumentContent::Init(ods::Tag *tag)
{
	tag->Copy(ns_->office(), ns::kVersion, office_version_);
	Scan(tag);
}

void OfficeDocumentContent::Init(ndff::Container *cntr)
{
	using Op = ndff::Op;
	ndff::Property prop;
	Op op = cntr->Next(prop, Op::None);
	mtl_info("First OP: %s", ndff::ops(op));
	NdffAttrs attrs;
	op = cntr->Next(prop, op, &attrs);
	mtl_info("OP after attrs: %s", ndff::ops(op));
	CopyAttr(attrs, ns_->office(), ns::kVersion, office_version_);
	mtl_info("office_version_: %s", qPrintable(office_version_));
	if (op == Op::N32_TE)
	{
		mtl_trace("op %s", ndff::ops(op));
		return;
	}
	
	if (op == Op::TCF_CMS)
	{
		op = cntr->Next(prop, op);
		mtl_trace("op %s (After TCF)", ndff::ops(op));
	}
	
	while (true)
	{
		mtl_trace("op %s", ndff::ops(op));
		if (op == Op::TS)
		{
			mtl_trace();
			if (prop.is(ns_->office()))
			{
				//mtl_trace("prop.name: %s", qPrintable(prop.name));
				if (prop.name == ns::kScripts) {
					office_scripts_ = new OfficeScripts(this, 0, cntr);
				} else if (prop.name == ns::kFontFaceDecls) {
					office_font_face_decls_ = new OfficeFontFaceDecls(this, 0, cntr);
				} else if (prop.name == ns::kAutomaticStyles) {
					office_automatic_styles_ = new OfficeAutomaticStyles(this, 0, cntr);
				} else if (prop.name == ns::kBody) {
					office_body_ = new OfficeBody(this, 0, cntr);
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
		mtl_trace("Unexpected op: %s", ndff::ops(op));
}

void OfficeDocumentContent::InitDefault()
{
	office_scripts_ = new OfficeScripts(this);
	office_body_ = new OfficeBody(this);
	office_automatic_styles_ = new OfficeAutomaticStyles(this);
	office_font_face_decls_ = new OfficeFontFaceDecls(this);
	
	office_version_ = QLatin1String("1.2");
}

void OfficeDocumentContent::ListChildren(QVector<StringOrInst*> &vec,
	const Recursively r)
{
	if (office_scripts_)
	{
		vec.append(new StringOrInst(office_scripts_, TakeOwnership::No));
		if (r == Recursively::Yes)
			office_scripts_->ListChildren(vec, r);
	}
	
	if (office_font_face_decls_)
	{
		vec.append(new StringOrInst(office_font_face_decls_, TakeOwnership::No));
		if (r == Recursively::Yes)
			office_font_face_decls_->ListChildren(vec, r);
	}
	
	if (office_automatic_styles_)
	{
		vec.append(new StringOrInst(office_automatic_styles_, TakeOwnership::No));
		if (r == Recursively::Yes)
			office_automatic_styles_->ListChildren(vec, r);
	}
	
	if (office_body_)
	{
		vec.append(new StringOrInst(office_body_, TakeOwnership::No));
		if (r == Recursively::Yes)
			office_body_->ListChildren(vec, r);
	}
}

void OfficeDocumentContent::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->office(), list);
}

void OfficeDocumentContent::ListKeywords(Keywords &list, const LimitTo lt)
{
	AddKeywords({tag_name(), ns::kVersion}, list);
}

void OfficeDocumentContent::Scan(ods::Tag *tag)
{
	for (auto *node: tag->nodes())
	{
		if (!node->is_tag())
			continue;
		
		bool scan = false;
		ods::Tag *subtag = node->as_tag();
		if (subtag->Has(ns_->office()))
		{
			if (subtag->Has(ods::ns::kAutomaticStyles)) {
				office_automatic_styles_ = new OfficeAutomaticStyles(this, subtag);
			} else if (subtag->Has(ods::ns::kBody)) {
				office_body_ = new OfficeBody(this, subtag);
			} else if (subtag->Has(ods::ns::kFontFaceDecls)) {
				office_font_face_decls_ = new OfficeFontFaceDecls(this, subtag);
			} else if (subtag->Has(ods::ns::kScripts)) {
				office_scripts_ = new OfficeScripts(this, subtag);
			} else {
				scan = true;
			}
		} else {
			scan = true;
		}
		
		if (scan)
			Scan(subtag);
	}
}

void OfficeDocumentContent::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->office(), ods::ns::kVersion, office_version_);
	
	if (office_scripts_)
		office_scripts_->Write(xml);
	
	if (office_font_face_decls_)
		office_font_face_decls_->Write(xml);
	
	if (office_automatic_styles_)
		office_automatic_styles_->Write(xml);
	
	if (office_body_)
		office_body_->Write(xml);
}

void OfficeDocumentContent::WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba)
{
	MTL_CHECK_VOID(ba != nullptr);
	WriteTag(kw, *ba);
	WriteNdffProp(kw, *ba, ns_->office(), ns::kVersion, office_version_);
	CloseBasedOnChildren(h, kw, file, ba);
}

} // ods::inst::
