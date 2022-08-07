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
	
	inst::OfficeDocumentStyles*
	document_styles() const { return document_styles_; }
	
	const QStringList&
	extracted_file_paths() const { return extracted_file_paths_; }
	
	static Book*
	FromFile(const QString &full_path, QString *err);
	
	const QVector<inst::TableNamedRange *> &GetAllNamedRanges();
	
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
	NewRowStyle(const Place place = Place::ContentFile);
	
	inst::StyleStyle*
	NewStyle(const Place place, const style::Family f);
	
	bool quick_save() const {
		return loading_ ? false : (quick_save_mode_ == QuickSave::Yes);
	}
	void quick_save(const QuickSave s) { quick_save_mode_ = s; }
	
	Records* records() const { return records_; }
	
	bool // returns true on success
	Save(const QFile &target, QString *err);
	
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
	void Load(const QString &full_path, QString *err);
	void LoadContentXml(ci32 file_index, QString *err);
	void LoadManifestXml(ci32 file_index, QString *err);
	void LoadMetaXml(ci32 file_index, QString *err);
	void LoadStylesXml(ci32 file_index, QString *err);
	bool Save(inst::Abstract *top, const QString &full_path, QString *err);
	void Scan(ods::Tag *parent);
	
	QMimeDatabase db_;
	const bool dev_mode_ = true;
	bool loading_ = false;
	QStringList extracted_file_paths_;
	QString media_dir_path_;
	
	QTemporaryDir temp_dir_;
	QString temp_dir_path_;
	inst::OfficeDocumentContent *document_content_ = nullptr;
	inst::OfficeDocumentMeta *document_meta_ = nullptr;
	inst::OfficeDocumentStyles *document_styles_ = nullptr;
	inst::ManifestManifest *manifest_ = nullptr;
	QVector<inst::TableNamedRange*> *named_ranges_ = nullptr;
	
	QuickSave quick_save_mode_ = QuickSave::No;
	Records *records_ = nullptr;
	
	friend class inst::OfficeDocumentContent;
	friend class inst::OfficeDocumentMeta;
	friend class inst::OfficeDocumentStyles;
	friend class inst::ManifestManifest;
};

} // ods::
