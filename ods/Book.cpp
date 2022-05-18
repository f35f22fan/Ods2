#include "Book.hpp"

#include "filename.hxx"
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
#include "inst/TableNamedRange.hpp"

#include <QGuiApplication>
#include <QSaveFile>
#include <QScreen>
#include <QXmlStreamWriter>
#include <quazip/JlCompress.h>

namespace ods { // ods::

Book::Book()
{
	double dpi = QGuiApplication::primaryScreen()->logicalDotsPerInch();
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

Book*
Book::FromFile(const QString &full_path, QString *err)
{
	auto *book = new Book();
	book->Load(full_path, err);
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
			it_happened();
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
			it_happened();
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
	
	QVector<ods::Sheet*> &sheets = sp->tables();
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

void
Book::InitDefault()
{
	document_content_ = new inst::OfficeDocumentContent(this, new Ns());
	manifest_ = new inst::ManifestManifest(this, new Ns());
	document_meta_ = new inst::OfficeDocumentMeta(this, new Ns());
	document_styles_ = new inst::OfficeDocumentStyles(this, new Ns());
}

void
Book::InitTempDir()
{
	const bool do_remove = true;
	temp_dir_.setAutoRemove(do_remove);
	
	if (dev_mode_)
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

void
Book::Load(const QString &full_path, QString *err)
{
	InitTempDir();
	
	if (!dev_mode_ && !temp_dir_.isValid())
	{
		mtl_warn("temp dir invalid");
		return;
	}
	
	extracted_file_paths_ = JlCompress::extractDir(full_path, temp_dir_path_);
	
	if (extracted_file_paths_.isEmpty()) {
		if (err != nullptr)
			*err = QLatin1String("Couldn't extract files");
		return;
	}
	
	for (const auto &path : extracted_file_paths_) {
		if (path.endsWith(ods::filename::ContentXml)) {
			LoadContentXml(path, err);
			break;
		}
	}
	
	for (const auto &path : extracted_file_paths_) {
		if (path.endsWith(ods::filename::ManifestXml)) {
			LoadManifestXml(path, err);
			break;
		}
	}
	
	for (const auto &path : extracted_file_paths_) {
		if (path.endsWith(ods::filename::MetaXml)) {
			LoadMetaXml(path, err);
			break;
		}
	}
	
	for (const auto &path : extracted_file_paths_) {
		if (path.endsWith(ods::filename::StylesXml)) {
			LoadStylesXml(path, err);
			break;
		}
	}
}

void
Book::LoadContentXml(const QString &full_path, QString *err)
{
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
		
		if (xml.name() == QLatin1String(ods::ns::kDocumentContent))
		{
			auto *ns = new ods::Ns();
			ns->Read(xml);
			auto *tag = ods::Tag::From(xml, ns);
			document_content_ = new inst::OfficeDocumentContent(this, ns, tag);
			break;
		}
		
		if (err != nullptr && !err->isEmpty())
			break;
	}
}

void
Book::LoadManifestXml(const QString &full_path, QString *err)
{
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
		
		if (xml.name() == QLatin1String(ods::ns::kManifest))
		{
			auto *ns = new ods::Ns();
			ns->Read(xml);
			auto *tag = ods::Tag::From(xml, ns);
			manifest_ = new ods::inst::ManifestManifest(this, ns, tag);
			break;
		}
		
		if (err != nullptr && !err->isEmpty())
			break;
	}
}

void
Book::LoadMetaXml(const QString &full_path, QString *err)
{
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
		
		if (xml.name() == QLatin1String(ods::ns::kDocumentMeta))
		{
			auto *ns = new ods::Ns();
			ns->Read(xml);
			auto *tag = ods::Tag::From(xml, ns);
			document_meta_ = new inst::OfficeDocumentMeta(this, ns, tag);
			break;
		}
		
		if (err != nullptr && !err->isEmpty())
			break;
	}
}

void
Book::LoadStylesXml(const QString &full_path, QString *err)
{
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
		
		if (xml.name() == QLatin1String(ods::ns::kDocumentStyles))
		{
			auto *ns = new ods::Ns();
			ns->Read(xml);
			auto *tag = ods::Tag::From(xml, ns);
			document_styles_ = new inst::OfficeDocumentStyles(this, ns, tag);
			break;
		}
		
		if (err != nullptr && !err->isEmpty())
			break;
	}
}

Book*
Book::New()
{
	auto *book = new Book();
	book->InitDefault();
	return book;
}

inst::StyleStyle*
Book::NewStyle(const Place place, const style::Family f)
{
	inst::OfficeAutomaticStyles *au = nullptr;
	
	if (place == Place::ContentFile)
	{
		return document_content_->automatic_styles()->NewStyleStyle(f);
	} else if (place == Place::StylesFile) {
		return document_styles_->office_styles()->NewStyleStyle(f);
	}
	
	it_happened();
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

bool
Book::Save(const QFile &target, QString *err)
{
	if (document_content_ == nullptr || document_styles_ == nullptr)
	{
		mtl_warn();
		return false;
	}
	
	QDir base_dir(temp_dir_path_);
	
	QString full_path;
	const QString MetaInf = QStringLiteral("META-INF");
	
	full_path = base_dir.filePath(ods::filename::ContentXml);
	Save(document_content_, full_path, err);
	
	full_path = base_dir.filePath(ods::filename::StylesXml);
	Save(document_styles_, full_path, err);
	
	QDir meta_dir(base_dir.filePath(MetaInf));
	
	if (!meta_dir.exists()) {
		if (!base_dir.mkdir(MetaInf)) {
			if (err != nullptr)
				*err = "Failed to create meta-inf dir";
			return false;
		}
	}
	
	full_path = meta_dir.filePath(ods::filename::ManifestXml);
	Save(manifest_, full_path, err);
	
	full_path = base_dir.filePath(ods::filename::MetaXml);
	Save(document_meta_, full_path, err);
	
	{ // mimetype
		full_path = base_dir.filePath(ods::filename::MimeType);
		QFile file(full_path);
		
		if (file.open(QIODevice::ReadWrite))
		{
			QTextStream stream(&file);
			stream << "application/vnd.oasis.opendocument.spreadsheet";
		} else {
			if (err != nullptr)
				*err = QLatin1String("Failed to save ") + full_path;
			return false;
		}
	}
	
	QString ods_path = target.fileName();
	if (!JlCompress::compressDir(ods_path, temp_dir_path_))
	{
		if (err != nullptr)
			*err = QLatin1String("Failed to compress dir");
		return false;
	}
	
//	auto ba = ods_path.toLocal8Bit();
//	mtl_line("Saved as: %s", ba.data());
	return true;
}

bool
Book::Save(inst::Abstract *top, const QString &full_path, QString *err)
{
	QSaveFile out(full_path);
	out.open(QIODevice::WriteOnly | QIODevice::Truncate);
	
	QXmlStreamWriter xml(&out);
	WriteStartDocument(xml);
	top->Write(xml);
	xml.writeEndDocument();
	const bool ok = out.commit();
	
	if (!ok && err != nullptr)
		*err = QString("Failed to save as ") + full_path;
	
	return ok;
}

inst::OfficeSpreadsheet*
Book::spreadsheet() const
{
	CHECK_TRUE_NULL((document_content_ != nullptr));
	auto *body = document_content_->body();
	CHECK_TRUE_NULL((body != nullptr));
	
	return body->spreadsheet();
}

void
Book::WriteStartDocument(QXmlStreamWriter &xml)
{
	xml.setAutoFormatting(true);
	xml.setAutoFormattingIndent(-1);
	xml.writeStartDocument(QLatin1String("1.0"), false);
}

} // ods::
