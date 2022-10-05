#pragma once

#include "Abstract.hpp"
#include "decl.hxx"

#include "../decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API OfficeDocumentContent : public Abstract
{
public:
	OfficeDocumentContent(Book *book, Ns *ns, Tag *tag = nullptr);
	OfficeDocumentContent(const OfficeDocumentContent &cloner);
	virtual ~OfficeDocumentContent();
	
	static OfficeDocumentContent* From(Ns *ns, ndff::Container *cntr);
	
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
	
	bool has_children(const IncludingText itx) const override {
		return office_scripts_ || office_font_face_decls_ ||
		office_automatic_styles_ || office_body_;
	}
	void ListChildren(QVector<StringOrInst *> &vec, const Recursively r) override;
	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	bool Save(const QString &dir_path);
	void WriteData(QXmlStreamWriter &xml) override;
	void WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba) override;
private:
	
	void Init(ods::Tag *tag);
	void InitDefault();
	void Scan(ods::Tag *tag);
	
	OfficeScripts *office_scripts_ = nullptr;
	OfficeFontFaceDecls *office_font_face_decls_ = nullptr;
	OfficeAutomaticStyles *office_automatic_styles_ = nullptr;
	OfficeBody *office_body_ = nullptr;
	QString office_version_;
};

} // ods::inst::
