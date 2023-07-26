#pragma once

#include "decl.hxx"
#include "err.hpp"
#include "global.hxx"
#include "ods.hxx"

#include "inst/Abstract.hpp"
#include "inst/decl.hxx"
#include "ndff/decl.hxx"
#include "ndff/Container.hpp"

#include <QMimeDatabase>
#include <QHash>
#include <QSet>
#include <QTemporaryDir>
#include <QVector>

class QXmlStreamWriter;

namespace ods {

class ODS_API Book
{
	using Bits = u8;
	const Bits LoadingBit = 1u << 0;
	const Bits DevModeBit = 1u << 1;
	const Bits CreatedButNotSavedBit = 1u << 2;
	const Bits NdffBit = 1u << 3;
	
private:
	Book(const DevMode dm);
public:
	static Book* FromFile(const QString &full_path, QString *err,
		const DevMode dm = DevMode::No);
	static Book* FromNDFF(QStringView full_path);
	static Book* New(const DevMode dm = DevMode::No);
	virtual ~Book();
	
	bool dev_mode() const { return bits_ & DevModeBit; }
	void dev_mode(cbool b) {
		if (b)
			bits_ |= DevModeBit;
		else
			bits_ &= ~DevModeBit;
	}
	
	inst::OfficeDocumentContent*
	document_content() const { return document_content_; }
	
	inst::OfficeDocumentStyles*
	document_styles() const { return document_styles_; }
	
	const QStringList&
	extracted_file_paths() const { return extracted_file_paths_; }
	
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
	
	inst::StyleStyle*
	NewCellStyle(const Place place = Place::ContentFile);
	
	inst::StyleStyle*
	NewColumnStyle(const Place place = Place::ContentFile);
	
	inst::StyleStyle*
	NewRowStyle(const Place place = Place::ContentFile);
	
	inst::StyleStyle*
	NewStyle(const Place place, const style::Family f);
	
	bool loading() const { return bits_ & LoadingBit; }
	void loading(const bool b) {
		if (b)
			bits_ |= LoadingBit;
		else
			bits_ &= ~LoadingBit;
	}
	
	bool ndff_enabled() const { return bits_ & NdffBit; }
	void ndff_enabled(cbool b) {
		if (b)
			bits_ |= NdffBit;
		else
			bits_ &= ~NdffBit;
	}
	
	QString ndff_path() const { return ndff_path_; }
	
	ndff::Container& ndff_container() { return ndff_; }
	
	void QueryUsedNamespaces(inst::NsHash &list, const CreateIfNeeded create_default);
	void QueryKeywords(inst::Keywords &list);
	// returns true on success
	bool Save(const QFile &target, QString *err);
	bool SaveNDFF(QString *err);
	
	//==> quick getters for convenience
	i32 sheet_count() const;
	ods::Sheet* GetSheet(ci32 at);
	//<== quick getters for convenience
	
	inst::OfficeSpreadsheet*
	spreadsheet() const;
	
	const QString&
	temp_dir_path() const { return temp_dir_path_; }
	Compression WhatCompressionShouldBeUsed(QStringView file_path,
		ci64 uncompressed_size) const
	{
		return ndff_.WhatCompressionShouldBeUsed(file_path, uncompressed_size);
	}
	void WriteStartDocument(QXmlStreamWriter &xml);
	
private:
	NO_ASSIGN_COPY_MOVE(Book);
	
	void AddFei(inst::Abstract *top, QStringView fn, inst::NsHash &ns_hash,
		inst::Keywords &keywords,
		ndff::FileEntryInfo &fei,
		ByteArray &output_buffer,
		ci64 record_result_loc);
	
	void AddFolderFei(ByteArray &buffer, ndff::FileEntryInfo &fei,
		QString filename, ci64 record_result_loc = -1);
	// success if ret val is positive
	i64 CreateFeiTable(ByteArray &buffer, cu32 reserved,
		ci64 record_table_loc = -1);
	bool CreateMimetypeFei(ByteArray &buffer, ndff::FileEntryInfo &fei, ci64 record_fei_loc);
	
	bool created_but_not_saved() const { return bits_ & CreatedButNotSavedBit; }
	void created_but_not_saved(const bool b) {
		if (b)
			bits_ |= CreatedButNotSavedBit;
		else
			bits_ &= ~CreatedButNotSavedBit;
	}
	
	void CreateDictionaryRegion(ByteArray &buffer, inst::Keywords &list);
	void CreateMainHeader(ByteArray &ba);
	void CreateNamespacesRegion(ByteArray &result, inst::NsHash &h);
	
	QVector<inst::Abstract*> GetNamespaceClasses();
	void InitDefault();
	void InitTempDir();
	bool InitNDFF(QStringView full_path);
	void Load(QStringView full_path, QString *err);
	void LoadContentXml(ci32 file_index, QString *err);
	void LoadManifestXml(ci32 file_index, QString *err);
	void LoadMetaXml(ci32 file_index, QString *err);
	void LoadStylesXml(ci32 file_index, QString *err);
	bool SaveFromScratch(QString *err);
	bool SaveXmlFile(inst::Abstract *top, const QString &full_path, QString *err);
	void Scan(ods::Tag *parent);
	
	QMimeDatabase db_;
	QStringList extracted_file_paths_;
	QString media_dir_path_;
	QString ndff_path_;
	QTemporaryDir temp_dir_;
	QString temp_dir_path_;
	inst::OfficeDocumentContent *document_content_ = nullptr;
	inst::OfficeDocumentMeta *document_meta_ = nullptr;
	inst::OfficeDocumentStyles *document_styles_ = nullptr;
	inst::ManifestManifest *manifest_ = nullptr;
	QVector<inst::TableNamedRange*> *named_ranges_ = nullptr;
	
	Bits bits_ = 0;
	ndff::Container ndff_;
	
	friend class inst::OfficeDocumentContent;
	friend class inst::OfficeDocumentMeta;
	friend class inst::OfficeDocumentStyles;
	friend class inst::ManifestManifest;
};

} // ods::
