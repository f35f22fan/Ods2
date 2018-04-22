#pragma once

#include "decl.hxx"
#include "err.hpp"
#include "global.hxx"

#include "inst/Abstract.hpp"
#include "inst/decl.hxx"

#include <QMimeDatabase>
#include <QTemporaryDir>
#include <QVector>

class QXmlStreamWriter;

namespace ods { // ods::

class ODS_API Book
{
private:
	Book();
public:
	virtual ~Book();
	
	inst::OfficeDocumentContent*
	document_content() const { return document_content_; }
	
	const QString&
	error_msg() const { return error_msg_; }
	
	const QStringList&
	extracted_file_paths() const { return extracted_file_paths_; }
	
	static Book*
	FromFile(const QString &full_path);
	
	inst::Abstract*
	GetAnyStyle(const QString &name);
	
	inst::OfficeFontFaceDecls*
	GetFontFaceDecls() const;
	
	QString*
	GetMediaDirPath();
	
	QString
	GetMimeType(const QString &kFilePath);
	
	QString
	GenUniqueStyleName(const ods::style::Family f, const id::func fu = nullptr);
	
	inst::ManifestManifest*
	manifest() const { return manifest_; }
	
	static Book*
	New();
	
	inst::StyleStyle*
	NewCellStyle(const Place place = Place::ContentFile);
	
	inst::StyleStyle*
	NewColumnStyle(const Place place = Place::ContentFile);
	
	inst::StyleStyle*
	NewStyle(const Place place, const style::Family f);
	
	bool // returns true on success
	Save(const QFile &target);
	
	inst::OfficeSpreadsheet*
	spreadsheet() const;
	
	const QString&
	temp_dir_path() const { return temp_dir_path_; }
	
	void
	WriteStartDocument(QXmlStreamWriter&);
	
private:
	NO_ASSIGN_COPY_MOVE(Book);
	
	void InitTempDir();
	void InitDefault();
	void Load(const QString &full_path);
	void LoadContentXml(const QString &full_path);
	void LoadManifestXml(const QString &full_path);
	void LoadMetaXml(const QString &full_path);
	void LoadStylesXml(const QString &full_path);
	bool Save(inst::Abstract *top, const QString &full_path);
	void Scan(ods::Tag *parent);
	
	QMimeDatabase db_;
	const bool dev_mode_ = true;
	QString error_msg_;
	QStringList extracted_file_paths_;
	QString media_dir_path_;
	
	QTemporaryDir temp_dir_;
	QString temp_dir_path_;
	inst::OfficeDocumentContent *document_content_ = nullptr;
	inst::OfficeDocumentMeta *document_meta_ = nullptr;
	inst::OfficeDocumentStyles *document_styles_ = nullptr;
	inst::ManifestManifest *manifest_ = nullptr;
	
	friend class inst::OfficeDocumentContent;
	friend class inst::OfficeDocumentMeta;
	friend class inst::OfficeDocumentStyles;
	friend class inst::ManifestManifest;
};

} // ods::
