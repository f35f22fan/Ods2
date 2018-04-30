#pragma once

#include "Abstract.hpp"
#include "decl.hxx"

#include "../decl.hxx"
#include "../err.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

class ODS_API OfficeDocumentContent : public Abstract
{
public:
	OfficeDocumentContent(Book *book, Ns *ns, Tag *tag = nullptr);
	OfficeDocumentContent(const OfficeDocumentContent &cloner);
	virtual ~OfficeDocumentContent();
	
	OfficeAutomaticStyles*
	automatic_styles() const { return office_automatic_styles_; }
	
	OfficeBody*
	body() const { return office_body_; }
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	OfficeFontFaceDecls*
	font_face_decls() const { return office_font_face_decls_; }
	
	Abstract*
	GetAnyStyle(const QString &name);
	
	bool
	Save(const QString &dir_path);
	
	void
	WriteData(QXmlStreamWriter &xml) override;
	
private:
	
	void Init(ods::Tag *tag);
	void InitDefault();
	void Scan(ods::Tag *tag);
	
	OfficeAutomaticStyles *office_automatic_styles_ = nullptr;
	OfficeBody *office_body_ = nullptr;
	OfficeFontFaceDecls *office_font_face_decls_ = nullptr;
	OfficeScripts *office_scripts_ = nullptr;
	
	QString office_version_;
};

} // ods::inst::
} // ods::
