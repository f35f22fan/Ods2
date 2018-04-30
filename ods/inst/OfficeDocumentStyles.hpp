#pragma once

#include "Abstract.hpp"
#include "decl.hxx"

#include "../decl.hxx"
#include "../err.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

class ODS_API OfficeDocumentStyles : public Abstract
{
public:
	OfficeDocumentStyles(ods::Book *book, ods::Ns *ns, ods::Tag *tag = nullptr);
	OfficeDocumentStyles(const OfficeDocumentStyles &cloner);
	virtual ~OfficeDocumentStyles();
	
	OfficeAutomaticStyles*
	automatic_styles() const { return office_automatic_styles_; }
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	Abstract*
	GetAnyStyle(const QString &name);
	
	ods::inst::OfficeFontFaceDecls*
	office_font_face_decls() const { return office_font_face_decls_; }
	
	ods::inst::OfficeStyles*
	office_styles() const { return office_styles_; }
	
	void
	WriteData(QXmlStreamWriter &xml) override;
	
private:
	
	void Init(ods::Tag *tag);
	void InitDefault();
	void LoadStylesXml(const QString &full_path);
	void Scan(ods::Tag *tag);
	
	OfficeAutomaticStyles *office_automatic_styles_ = nullptr;
	OfficeFontFaceDecls *office_font_face_decls_ = nullptr;
	OfficeStyles *office_styles_ = nullptr;
	OfficeMasterStyles *office_master_styles_ = nullptr;
	
	QString office_version_;
};

} // ods::inst::
} // ods::
