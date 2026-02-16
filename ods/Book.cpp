#include "Book.hpp"

#include "AutoDelete.hh"
#include "ByteArray.hpp"
#include "filename.hxx"
#include "io.hh"
#include "Ns.hpp"
#include "ns.hxx"
#include "ods.hh"
#include "Sheet.hpp"
#include "Tag.hpp"

#include "inst/ManifestManifest.hpp"
#include "inst/OfficeAutomaticStyles.hpp"
#include "inst/OfficeBody.hpp"
#include "inst/OfficeDocumentContent.hpp"
#include "inst/OfficeDocumentMeta.hpp"
#include "inst/OfficeDocumentStyles.hpp"
#include "inst/OfficeSpreadsheet.hpp"
#include "inst/OfficeStyles.hpp"
#include "inst/TableNamedExpressions.hpp"

#include <QElapsedTimer>
#include <QGuiApplication>
#include <QSaveFile>
#include <QScreen>
#include <QXmlStreamWriter>
#include "zip.hh"

namespace ods {

class StringCount {
public:
	QString keyword;
	i32 count = 0;
	i32 id = 0;
};

bool SortDictionaryEntries(const StringCount &lhs, const StringCount &rhs)
{
	return lhs.count > rhs.count;
}

Book::Book(const DevMode dm)
{
	dev_mode(dm == DevMode::Yes);
	cf64 dpi = QGuiApplication::primaryScreen()->logicalDotsPerInch();
	ods::DPI(dpi);
	InitTempDir();
}

Book::~Book()
{
	delete document_content_;
	delete document_styles_;
	delete document_meta_;
	delete manifest_;
	
	delete named_ranges_; // its items don't belong to it.
	named_ranges_ = nullptr;
}

i32 Book::sheet_count() const
{
	auto *sp = spreadsheet();
	return sp ? sp->sheet_count() : -1;
}

QString Book::GetMimeTypeForFile(QString full_path) const
{
	QMimeType mime = db_.mimeTypeForFile(full_path);
	return mime.name();
}

ods::Sheet* Book::GetSheet(ci32 at)
{
	auto *sp = spreadsheet();
	return sp ? sp->GetSheet(at) : nullptr;
}

Book*
Book::FromFile(const QString &full_path, QString *err, const DevMode dm)
{
	auto *book = new Book(dm);
	book->loading(true);
	if (!book->Load(full_path, err)) {
		delete book;
		return nullptr;
	}
	book->loading(false);
	return book;
}

QString
Book::GenUniqueStyleName(const style::Family f, const id::func fu)
{
	QString base;
	
	if (fu != nullptr)
	{
		if (fu == id::StyleStyle)
			base = QLatin1String("ss");
		else if (fu == id::NumberCurrencyStyle)
			base = QLatin1String("ncs");
		else if (fu == id::NumberCurrencySymbol)
			base = QLatin1String("ncsy");
		else if (fu == id::NumberDateStyle)
			base = QLatin1String("nds");
		else if (fu == id::NumberPercentageStyle)
			base = QLatin1String("nps");
		else if (fu == id::NumberTimeStyle)
			base = QLatin1String("nts");
		else if (fu == id::NumberBooleanStyle)
			base = QLatin1String("bool");
		else
			mtl_it_happened();
	}
	
	if (f != style::Family::None)
	{
		if (f == style::Family::Cell)
			base.append(QLatin1String("ce"));
		else if (f == style::Family::Column)
			base.append(QLatin1String("co"));
		else if (f == style::Family::Row)
			base.append(QLatin1String("ro"));
		else if (f == style::Family::Table)
			base.append(QLatin1String("ta"));
		else
			mtl_it_happened();
	}
	
	int i = 0;
	
	while (true)
	{
		QString next = base + QString::number(i++);
		
		if (GetAnyStyle(next) == nullptr)
			return next;
	}
}

inst::Abstract*
Book::GetAnyStyle(const QString &name)
{
	auto *a = document_content_->GetAnyStyle(name);
	
	if (a != nullptr)
		return a;
	
	return document_styles_->GetAnyStyle(name);
}

const QVector<inst::TableNamedRange*>&
Book::GetAllNamedRanges()
{
	if (named_ranges_ != nullptr)
		return *named_ranges_;
	
	named_ranges_ = new QVector<inst::TableNamedRange*>();
	auto *sp = spreadsheet();
	
	if (sp == nullptr) {
		mtl_trace();
		return *named_ranges_;
	}
	
	inst::TableNamedExpressions *tne = sp->named_expressions();
	
	if (tne != nullptr)
		tne->CopyNamedRangesTo(named_ranges_);
	
	QVector<ods::Sheet*> &sheets = sp->sheets();
	for (ods::Sheet *sheet: sheets) {
		auto *tne = sheet->named_expressions();
		if (tne != nullptr)
			tne->CopyNamedRangesTo(named_ranges_);
	}
	
	return *named_ranges_;
}

inst::OfficeFontFaceDecls*
Book::GetFontFaceDecls() const
{
	return document_content_->font_face_decls();
}

QString*
Book::GetMediaDirPath()
{
	if (!media_dir_path_.isEmpty())
		return &media_dir_path_;
	
	QDir dir(QDir(temp_dir_path_).filePath(ods::filename::MediaDirName));
	
	if (!dir.exists())
	{
		if (!dir.mkpath("."))
			return nullptr;
	}
	
	media_dir_path_ = dir.absolutePath();
	return &media_dir_path_;
}

QString
Book::GetMimeType(const QString &file_path)
{
	return db_.mimeTypeForFile(file_path).name();
}

QVector<inst::Abstract*>
Book::GetNamespaceClasses()
{
	QVector<inst::Abstract*> vec;
	if (document_content_)
		vec.append(document_content_);
	
	if (document_meta_)
		vec.append(document_meta_);
	
	if (document_styles_)
		vec.append(document_styles_);
	
	if (manifest_)
		vec.append(manifest_);
	
	return vec;
}

void Book::InitDefault()
{
	document_content_ = new inst::OfficeDocumentContent(this, Ns::Default());
	manifest_ = new inst::ManifestManifest(this, Ns::Default());
	document_meta_ = new inst::OfficeDocumentMeta(this, Ns::Default());
	document_styles_ = new inst::OfficeDocumentStyles(this, Ns::Default());
}

void Book::InitTempDir()
{
	cbool do_remove = true;
	temp_dir_.setAutoRemove(do_remove);
	
	if (dev_mode())
	{
		temp_dir_path_ = QDir::home().filePath("ods_dev_mode");
		QDir dir(temp_dir_path_);
		
		if (dir.exists() && !dir.removeRecursively())
			mtl_warn("Failed to delete recursively");
		
		if (!dir.mkpath("."))
			mtl_warn("Can't create temp dir");
	} else {
		temp_dir_path_ = temp_dir_.path();
	}
}

bool Book::Load(QString zip_filepath, QString *err)
{
	InitTempDir();
	
	if (!dev_mode() && !temp_dir_.isValid())
	{
		mtl_warn("temp dir invalid");
		return false;
	}
	
	QString error_str;
	extracted_file_paths_ = ods::zip::ExtractFiles(zip_filepath, temp_dir_path_, &error_str);
	if (!error_str.isEmpty()) {
		mtl_warn("%s", qPrintable(error_str));
		if (err)
			*err = error_str;
		return false;
	}
	
	if (extracted_file_paths_.isEmpty()) {
		if (err != nullptr)
			*err = QLatin1String("extracted_file_paths_ is empty.");
		return false;
	}
	
	{
		cauto file_index = ods::FindIndexThatEndsWith(extracted_file_paths_,
			QString(ods::filename::ManifestXml));
		MTL_CHECK(file_index != -1);
		LoadManifestXml(file_index, err);
	}
	
	{
		cauto file_index = ods::FindIndexThatEndsWith(extracted_file_paths_,
			QString(ods::filename::ContentXml));
		MTL_CHECK(file_index != -1);
		LoadContentXml(file_index, err);
	}
	
	{
		cauto file_index = ods::FindIndexThatEndsWith(extracted_file_paths_,
			QString(ods::filename::MetaXml));
		MTL_CHECK(file_index != -1);
		LoadMetaXml(file_index, err);
	}
	
	{
		cauto file_index = ods::FindIndexThatEndsWith(extracted_file_paths_,
			QString(ods::filename::StylesXml));
		MTL_CHECK(file_index != -1);
		LoadStylesXml(file_index, err);
	}
	
	return true;
}

void Book::LoadContentXml(ci32 file_index, QString *err)
{
	cauto &full_path = extracted_file_paths_[file_index];
	QFile file(full_path);
	
	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
		if (err != nullptr)
			*err = file.errorString();
		return;
	}
	
	QXmlStreamReader xml(&file);
	QXmlStreamReader::TokenType token;
	
	while(!xml.atEnd() && !xml.hasError())
	{
		token = xml.readNext();
		//if (token == QXmlStreamReader::StartDocument)
		
		if (token != QXmlStreamReader::StartElement)
			continue;
		
		if (xml.name() == ns::kDocumentContent)
		{
			Ns *ns = Ns::FromXml(xml, file_index);
			auto *tag = ods::Tag::From(xml, ns);
			document_content_ = new inst::OfficeDocumentContent(this, ns, tag);
			delete tag;
			break;
		}
		
		if (err != nullptr && !err->isEmpty())
			break;
	}
}

void Book::LoadManifestXml(ci32 file_index, QString *err)
{
	cauto &full_path = extracted_file_paths_[file_index];
	QFile file(full_path);
	
	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
		if (err)
			*err = file.errorString();
		return;
	}
	
	QXmlStreamReader xml(&file);
	QXmlStreamReader::TokenType token;
	
	while(!xml.atEnd() && !xml.hasError())
	{
		token = xml.readNext();
		if (token != QXmlStreamReader::StartElement)
			continue;
		
		if (xml.name() == ns::kManifest)
		{
			auto *ns = Ns::FromXml(xml, file_index);
			auto *tag = ods::Tag::From(xml, ns);
			manifest_ = new ods::inst::ManifestManifest(this, ns, tag);
			delete tag;
			break;
		}
		
		if (err && !err->isEmpty())
			break;
	}
}

void Book::LoadMetaXml(ci32 file_index, QString *err)
{
	cauto &full_path = extracted_file_paths_[file_index];
	QFile file(full_path);
	
	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
		if (err != nullptr)
			*err = file.errorString();
		return;
	}
	
	QXmlStreamReader xml(&file);
	QXmlStreamReader::TokenType token;
	
	while(!xml.atEnd() && !xml.hasError())
	{
		token = xml.readNext();
		if (token != QXmlStreamReader::StartElement)
			continue;
		
		if (xml.name() == ns::kDocumentMeta)
		{
			auto *ns = Ns::FromXml(xml, file_index);
			auto *tag = ods::Tag::From(xml, ns);
			document_meta_ = new inst::OfficeDocumentMeta(this, ns, tag);
			delete tag;
			break;
		}
		
		if (err != nullptr && !err->isEmpty())
			break;
	}
}

void Book::LoadStylesXml(ci32 file_index, QString *err)
{
	cauto &full_path = extracted_file_paths_[file_index];
	QFile file(full_path);
	
	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
		if (err != nullptr)
			*err = file.errorString();
		return;
	}
	
	QXmlStreamReader xml(&file);
	QXmlStreamReader::TokenType token;

	while(!xml.atEnd() && !xml.hasError())
	{
		token = xml.readNext();
		
		if (token != QXmlStreamReader::StartElement)
			continue;
		
		if (xml.name() == ns::kDocumentStyles)
		{
			auto *ns = Ns::FromXml(xml, file_index);
			auto *tag = ods::Tag::From(xml, ns);
			document_styles_ = new inst::OfficeDocumentStyles(this, ns, tag);
			delete tag;
			break;
		}
		
		if (err != nullptr && !err->isEmpty())
			break;
	}
}

Book* Book::New(const DevMode dm)
{
	auto *book = new Book(dm);
	book->InitDefault();
	book->created_but_not_saved(true);
	
	return book;
}

inst::StyleStyle*
Book::NewStyle(const Place place, const style::Family f)
{
	if (place == Place::ContentFile)
	{
		return document_content_->automatic_styles()->NewStyleStyle(f);
	} else if (place == Place::StylesFile) {
		return document_styles_->office_styles()->NewStyleStyle(f);
	}
	
	mtl_it_happened();
	return nullptr;
}

inst::StyleStyle*
Book::NewCellStyle(const Place place)
{
	return NewStyle(place, style::Family::Cell);
}

inst::StyleStyle*
Book::NewColumnStyle(const Place place)
{
	return NewStyle(place, style::Family::Column);
}

inst::StyleStyle*
Book::NewRowStyle(const Place place)
{
	return NewStyle(place, style::Family::Row);
}

void Book::QueryKeywords(inst::Keywords &words_hash)
{
	QVector<inst::Abstract*> tops = GetNamespaceClasses();
	
	for (inst::Abstract *top: tops)
	{
		top->ListKeywords(words_hash, inst::LimitTo::All);
		
		QVector<StringOrInst*> children;
		top->ListChildren(children, Recursively::Yes);
		
		for (StringOrInst *next: children)
		{
			if (next->is_inst())
				next->as_inst()->ListKeywords(words_hash, inst::LimitTo::All);
		}
	}
	
	if (false)
	{
		for (auto it = words_hash.constBegin(); it != words_hash.constEnd(); it++)
		{
			const QString &name = it.key();
			const inst::IdAndCount &ic = it.value();
			mtl_info("%s: %d", qPrintable(name), ic.count);
		}
	}
}

void Book::QueryUsedNamespaces(inst::NsHash &ns_hash, const CreateIfNeeded cr)
{
	QVector<inst::Abstract*> tops = GetNamespaceClasses();
	
	if (cr == CreateIfNeeded::Yes && tops.isEmpty())
	{
		Ns *ns = Ns::Default();
		auto prefixes = ns->prefixes();
		
		for (Prefix *pref: prefixes)
		{
			ns_hash[pref->id()] = pref->uri();
		}
		
		return;
	}
	
	for (inst::Abstract *top: tops)
	{
		top->ListUsedNamespaces(ns_hash);
		
		QVector<StringOrInst*> children;
		top->ListChildren(children, Recursively::Yes);
		
		for (StringOrInst *next: children)
		{
			if (next->is_inst())
				next->as_inst()->ListUsedNamespaces(ns_hash);
		}
	}
}

bool Book::Save(const QFile &target, QString *err)
{
	MTL_CHECK(document_content_ && document_styles_);
	QElapsedTimer timer;
	timer.start();
	
	QDir temp_dir(temp_dir_path_);
	QString full_path;
	
	full_path = temp_dir.filePath(filename::ContentXml);
	MTL_CHECK(SaveXmlFile(document_content_, full_path, err));
	
	full_path = temp_dir.filePath(filename::StylesXml);
	MTL_CHECK(SaveXmlFile(document_styles_, full_path, err));
	
	QDir meta_dir(temp_dir.filePath(filename::MetaInf));
	
	if (!meta_dir.exists())
	{
		if (!temp_dir.mkdir(filename::MetaInf)) {
			if (err)
				*err = QString("Failed to create meta-inf dir");
			return false;
		}
	}
	
	full_path = meta_dir.filePath(filename::ManifestXml);
	MTL_CHECK(SaveXmlFile(manifest_, full_path, err));
	
	full_path = temp_dir.filePath(filename::MetaXml);
	MTL_CHECK(SaveXmlFile(document_meta_, full_path, err));
	
	{ // mimetype
		full_path = temp_dir.filePath(ods::filename::MimeType);
		QFile file(full_path);
		
		if (file.open(QIODevice::WriteOnly))
		{
			QTextStream stream(&file);
			stream << ns::kMimeType;
		} else {
			if (err != nullptr)
				*err = QLatin1String("Failed to save ") + full_path;
			return false;
		}
	}
	
	QString zip_filepath = QFileInfo(target).absoluteFilePath();
	MTL_CHECK(ods::zip::CompressDir(temp_dir_path_, zip_filepath, err));
	
	return true;
}

bool WriteAll(QIODevice &file, const char *data, ci64 max, QString *err = 0)
{
//	mtl_info("ba size: %ld", max);
	i64 so_far = 0;
	while (so_far < max)
	{
		cauto chunk = file.write(data + so_far, max - so_far);
		if (chunk == -1)
		{
			if (err)
				*err = strerror(errno);
			return false;
		}
		
		if (chunk == 0) // EOF
			break;
		
		so_far += chunk;
	}
//	mtl_info("Written: %ld", so_far);
	
	return true;
}

bool Book::SaveXmlFile(inst::Abstract *top, const QString &full_path, QString *err)
{
	QSaveFile out(full_path);
	MTL_CHECK(out.open(QIODevice::WriteOnly | QIODevice::Truncate));
	
	QXmlStreamWriter xml(&out);
	WriteStartDocument(xml);
	top->Write(xml);
	xml.writeEndDocument();
	cbool ok = out.commit();
	
	if (!ok && err != nullptr)
		*err = QString("Failed to save as ") + full_path;
	
	return ok;
}

inst::OfficeSpreadsheet*
Book::spreadsheet() const
{
	MTL_CHECK_ARG(document_content_, nullptr);
	auto *body = document_content_->body();
	MTL_CHECK_ARG(body, nullptr);
	
	return body->spreadsheet();
}

Compression Book::WhatCompressionShouldBeUsed(QStringView file_path,
	ci64 uncompressed_size) const
{
	return Compression::None;
}

void Book::WriteStartDocument(QXmlStreamWriter &xml)
{
	xml.setAutoFormatting(true);
	xml.setAutoFormattingIndent(-1);
	xml.writeStartDocument(QLatin1String("1.0"));
}

} // ods::
