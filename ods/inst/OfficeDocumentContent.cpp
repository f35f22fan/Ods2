#include "OfficeDocumentContent.hpp"

#include "OfficeAutomaticStyles.hpp"
#include "OfficeBody.hpp"
#include "OfficeFontFaceDecls.hpp"
#include "OfficeScripts.hpp"

#include "../Book.hpp"
#include "../filename.hxx"
#include "../Ns.hpp"
#include "../ns.hxx"
#include "../ods.hh"
#include "../Sheet.hpp"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

OfficeDocumentContent::OfficeDocumentContent
(ods::Book *book, ods::Ns *ns, ods::Tag *tag)
: Abstract(nullptr, ns, id::OfficeDocumentContent)
{
	book_ = book;
	book_->document_content_ = this;
	
	if (tag != nullptr)
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
	
	if (office_scripts_ != nullptr)
	{
		p->office_scripts_ = (OfficeScripts*) office_scripts_->Clone();
	}
	
	return p;
}

Abstract*
OfficeDocumentContent::GetAnyStyle(const QString &name)
{
	auto *a = office_automatic_styles_->GetStyleRecursive(name);
	
	if (a != nullptr)
		return a;
	
	return office_font_face_decls_->GetStyleRecursive(name);
}

void
OfficeDocumentContent::Init(ods::Tag *tag)
{
	tag->Copy(ns_->office(), ods::ns::kVersion, office_version_);
	Scan(tag);
}

void
OfficeDocumentContent::InitDefault()
{
	office_scripts_ = new OfficeScripts(this);
	office_body_ = new OfficeBody(this);
	office_automatic_styles_ = new OfficeAutomaticStyles(this);
	office_font_face_decls_ = new OfficeFontFaceDecls(this);
	
	office_version_ = QLatin1String("1.2");
}

void
OfficeDocumentContent::Scan(ods::Tag *tag)
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

void
OfficeDocumentContent::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->office(), ods::ns::kVersion, office_version_);
//	auto &ids = ns_->ids();
//	UriId tag_id = ids.Office;
//mtl_info("office id: %d, style_id: %d", tag_id, ids.Style);
	office_scripts_->Write(xml);
	office_font_face_decls_->Write(xml);
	office_automatic_styles_->Write(xml);
	office_body_->Write(xml);
}

} // ods::inst::
} // ods::
