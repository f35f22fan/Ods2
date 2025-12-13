#include "Book.hpp"

#include "AutoDelete.hh"
#include "ByteArray.hpp"
#include "filename.hxx"
#include "io.hh"
#include "ndff/FileEntryInfo.hpp"
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
//#include <quazip/JlCompress.h>
//#include <QtCore/private/qzipwriter_p.h>
// #include <QtCore/private/qzipreader_p.h>
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

bool Book::CreateMimetypeFei(ByteArray &buffer, ndff::FileEntryInfo &fei,
	ci64 record_fei_loc)
{
	const char *s = "application/vnd.oasis.opendocument.spreadsheet";
	fei.file_data().add(s, strlen(s) + 1, ExactSize::Yes);
	
	fei.self_loc(buffer.at());
	fei.SetRegularFile(filename::MimeType);
	fei.SetTimesToNow();
	fei.WriteTo(buffer);
	
	if (record_fei_loc >= 0)
		buffer.set_i64(record_fei_loc, fei.self_loc());
	
	return true;
}

i64 Book::CreateFeiTable(ByteArray &output, cu32 how_many,
	ci64 record_result_loc)
{
	ci64 table_addr = output.at();
	for (u32 i = 0; i < how_many; i++)
	{
		output.add_i64(ndff::ReservedFeiTableValue);
	}
	
	output.add_i64(ndff::EndOfFeiTable);
	if (record_result_loc != -1)
		output.set_i64(record_result_loc, table_addr);
	
	return table_addr;
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

void Book::CreateDictionaryRegion(ByteArray &buffer, inst::Keywords &list)
{
	QueryKeywords(list);
	QVector<StringCount> vec;
	
	for (auto it = list.constBegin(); it != list.constEnd(); it++)
	{
		QStringView word = it.key();
		const inst::IdAndCount idc = it.value();
		StringCount sc;
		sc.keyword = word.toString();
		sc.count = idc.count;
		sc.id = idc.id;
		vec.append(sc);
	}
	
	std::sort(vec.begin(), vec.end(), SortDictionaryEntries);
	ByteArray buf;
	for (StringCount &next: vec)
	{
		//mtl_info("Dictionary entry id: %d \"%s\"", next.id, qPrintable(next.keyword));
		buf.add_inum(next.id);
		buf.add_string(next.keyword, Pack::NDFF);
	}
	
	const auto compression = Compression::ZSTD;
	
	switch (compression) {
	case Compression::None: {
		break;
	};
	case Compression::ZSTD: {
		ci64 uncompressed_size = buf.size();
		MTL_CHECK_VOID(buf.Compress(compression));
        mtl_info("Compressed buf size: %lld, uncompressed: %lld",
				 buf.size(), uncompressed_size);
		//buf.DumpToTerminal();
		break;
	};
	default: {
		mtl_tbd();
	};
	}
	
	cu32 block_info = (u32(buf.size()) << 3) | compression;
	buffer.add_i64(-1); // next block
	buffer.add_u32(block_info);
	buffer.add(buf);
}

void Book::CreateMainHeader(ByteArray &ba)
{
	ba.add((const char*)ndff::MagicNumber, 4, ExactSize::Yes);
	cu8 info = (1 << 7) | 1; // LE
	ba.add_u8(info);
//==> Say version 0.1 is the minimum:
	ba.add_u8(0); // smallest major version
	ba.add_i16(1); // smallest minor version or -1 if doesn't matter
//<==
	ba.add_i64(-1); // namespaces_loc will be set later
	ba.add_i64(-1); // dictionary_loc will be set later
	ba.add_i64(-1); // top_files_loc to be set later
	const char *doc_type = "ods";
	cu8 doc_type_strlen = strlen(doc_type);
	ba.add_u8(doc_type_strlen); // string length
	ba.add(doc_type, doc_type_strlen, ExactSize::Yes); // the string itself
	
	{ // free space
		cu8 free_space = 32;
		ba.add_u8(free_space);
		
		if (free_space > 0)
			ba.add_zeroes(free_space);
	}

}

void Book::CreateNamespacesRegion(ByteArray &result, inst::NsHash &h)
{
	QueryUsedNamespaces(h, CreateIfNeeded::Yes);
	auto it = h.constBegin();
	ByteArray buf;
	while (it != h.constEnd())
	{
		const UriId uri_id = it.key();
		const QString uri = it.value();
		it++;
		buf.add_u16(uri_id);
		buf.add_string(uri, Pack::NDFF);
	}
	
	ci64 uncompressed_size = buf.size();
	const auto compression = Compression::ZSTD;
	switch (compression) {
	case Compression::None: {
		break;
	};
	case Compression::ZSTD: {
		MTL_CHECK_VOID(buf.Compress(compression));
        mtl_info("Compressed buf size: %lld, uncompressed: %lld",
				 buf.size(), uncompressed_size);
		//buf.DumpToTerminal();
		break;
	};
	default: {
		mtl_tbd();
	};
	}
	
	cu32 info = (u32(buf.size()) << 3) | compression;
	result.add_u32(info);
	result.add(buf);
}

Book* Book::FromNDFF(QStringView full_path)
{
	auto *book = Book::New(DevMode::Yes);
	if (!book->InitNDFF(full_path))
	{
		delete book;
		return 0;
	}
	
	return book;
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
	const bool do_remove = true;
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

bool Book::InitNDFF(QStringView full_path)
{
	MTL_CHECK(ndff_.Init(this, full_path));
	auto &ndff_buf = ndff_.buf();
	{
		ndff::FileEntryInfo *fei = ndff_.GetTopFile(filename::ContentXml);
		MTL_CHECK(ndff_.PrepareFor(fei));
		document_content_ = new inst::OfficeDocumentContent(this, ndff_.ns(), 0, &ndff_);
	}
	{
		ndff::FileEntryInfo *fei = ndff_.GetTopFile(filename::StylesXml);
		MTL_CHECK(ndff_.PrepareFor(fei));
		document_styles_ = new inst::OfficeDocumentStyles(this, ndff_.ns(), 0, &ndff_);
	}
	{
		ndff::FileEntryInfo *fei = ndff_.GetTopFile(filename::MetaXml);
		MTL_CHECK(ndff_.PrepareFor(fei));
		document_meta_ = new inst::OfficeDocumentMeta(this, ndff_.ns(), 0, &ndff_);
	}
	{
		auto *folder = ndff_.GetTopFile(filename::MetaInf);
		MTL_CHECK(folder);
		ndff::FileEntryInfo *fei = folder->GetFile(filename::ManifestXml);
		MTL_CHECK(ndff_.PrepareFor(fei));
		manifest_ = new inst::ManifestManifest(this, ndff_.ns(), 0, &ndff_);
	}
	ndff_.SwitchToMainBuf();
	
	return true;
}

bool Book::Load(QString zip_filepath, QString *err)
{
	InitTempDir();
	
	if (!dev_mode() && !temp_dir_.isValid())
	{
		mtl_warn("temp dir invalid");
		return false;
	}
	
	// QZipReader zr(zip_filepath);
	// mtl_info("Loading from %s", qPrintable(zip_filepath));
	// if (!zr.extractAll(temp_dir_path_)) {
	// 	mtl_info("QZipReader error status: %d\n", zr.status());
	// 	return;
	// }
	// for (auto next: zr.fileInfoList()) {
	// 	auto ba = next.filePath.toLocal8Bit();
	// 	mtl_info("===FILE: %s", ba.data());
	// 	extracted_file_paths_.append(next.filePath);
	// }
	// mtl_info("QZipReader status=%d, FileOpenError=%d, temp_dir_path_=%s\n",
	// 	zr.status(), QZipReader::Status::FileOpenError, qPrintable(temp_dir_path_));
	
	// mtl_info("zip_filepath=\"%s\", temp_dir_path=\"%s\"", qPrintable(zip_filepath),
	// 	qPrintable(temp_dir_path_));
    // QFile input(zip_filepath);
    // QuaZip qz(&input);
    // qz.setZip64Enabled(true);
    // extracted_file_paths_ = JlCompress::extractDir(qz, temp_dir_path_);
	
	QString error_str;
	extracted_file_paths_ = ods::zip::ExtractFiles(zip_filepath, temp_dir_path_, &error_str);
	if (!error_str.isEmpty()) {
        mtl_warn("%s", qPrintable(error_str));
		if (err)
			*err = error_str;
		return false;
	}
	//mtl_info("Count: %d", extracted_file_paths_.size());
	
	// for (int i = 0; i < extracted_file_paths_.size(); i++) {
	// 	const auto path = extracted_file_paths_[i];
	// 	mtl_info("path: %s", qPrintable(path));
	// }
	
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
	
	// if (!JlCompress::compressDir(zip_filepath, temp_dir_path_, true)) {
	// 	mtl_warn("Failed to compress .ods file to %s", qPrintable(zip_filepath));
	// 	return false;
	// }
	
	if (ndff_enabled())
	{
		auto ms = timer.elapsed();
		mtl_info("SaveToODS() took %lld ms", ms);
		timer.restart();
		ndff_path_ = zip_filepath + QLatin1String(".ndff");
		SaveNDFF(err);
		mtl_info("SaveToNDFF() took %lld ms", timer.elapsed());
	}
	
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

bool Book::SaveFromScratch(QString *err)
{
	QSaveFile ndff_file(ndff_path_);
	MTL_CHECK(ndff_file.open(QIODevice::WriteOnly | QIODevice::Truncate));
	
	ByteArray main_buffer;
	CreateMainHeader(main_buffer);
	inst::NsHash ns_hash;
	{ //==> Namespaces region
		ci64 ns_address = main_buffer.at();
		CreateNamespacesRegion(main_buffer, ns_hash);
		main_buffer.set_i64(ndff::MainHeaderPlaces::namespaces_loc, ns_address);
	} //<== Namespaces region
	
	inst::Keywords keywords;
	{ //==> Dictionary region
		ci64 dict_location = main_buffer.at();
		CreateDictionaryRegion(main_buffer, keywords);
		main_buffer.set_i64(ndff::MainHeaderPlaces::dictionary_loc, dict_location);
	} //<== Dictionary region
	
	QByteArray type_ods = QString::fromLatin1("ods").toUtf8();
	main_buffer.add_u8(type_ods.size()); // must be u8 according to spec
	main_buffer.add(type_ods.data(), type_ods.size(), ExactSize::Yes);
	
	/* The files are:
		1) content.xml (file)
		2) styles.xml (file)
		3) mimetype (file)
		4) meta.xml (file)
		5) META-INF (folder)
		6) manifest.xml (file)
 */
	ci64 fei_table_loc = CreateFeiTable(main_buffer, 5,
		ndff::MainHeaderPlaces::top_files_loc);
	MTL_CHECK(fei_table_loc > 0);
	i64 record_result_at = fei_table_loc;
	
	ndff::FileEntryInfo contents_fei(this);
	contents_fei.SetFei(document_content_, filename::ContentXml,
		ns_hash, keywords, main_buffer, record_result_at);
	
	ndff::FileEntryInfo styles_fei(this);
	styles_fei.SetFei(document_styles_, filename::StylesXml,
		ns_hash, keywords, main_buffer, record_result_at += 8);
	
	ndff::FileEntryInfo meta_fei(this);
	meta_fei.SetFei(document_meta_, filename::MetaXml,
		ns_hash, keywords, main_buffer, record_result_at += 8);
	
	ndff::FileEntryInfo manifest_fei(this);
	manifest_fei.SetFei(manifest_, filename::ManifestXml,
		ns_hash, keywords, main_buffer, -1);
	
	/// "META-INF" folder
	ndff::FileEntryInfo folder_fei(this);
	{
		/// Tying "META-INFO/manifest.xml" to its parent folder
		folder_fei.subfiles_loc(-manifest_fei.self_loc());
		folder_fei.SetFolderFei(main_buffer, filename::MetaInf,
			record_result_at += 8);
	}
	
	ndff::FileEntryInfo mimetype_fei(this);
	MTL_CHECK(CreateMimetypeFei(main_buffer, mimetype_fei, record_result_at += 8));
	
	mimetype_fei.SetFileData(main_buffer, filename::MimeType);
	meta_fei.SetFileData(main_buffer, filename::MetaXml);
	styles_fei.SetFileData(main_buffer, filename::StylesXml);
	contents_fei.SetFileData(main_buffer, filename::ContentXml);
	manifest_fei.SetFileData(main_buffer, filename::ManifestXml);
	
	mimetype_fei.WriteEfa(main_buffer);
	meta_fei.WriteEfa(main_buffer);
	styles_fei.WriteEfa(main_buffer);
	contents_fei.WriteEfa(main_buffer);
	manifest_fei.WriteEfa(main_buffer);
	folder_fei.WriteEfa(main_buffer);
	
	//contents_fei.PrintFileName(main_buffer);
	
	MTL_CHECK(WriteAll(ndff_file, main_buffer.data(), main_buffer.size(), err));
	
	cbool ok = ndff_file.commit();
	if (!ok && err)
		*err = QString("Failed to save as ") + ndff_path_;
	
	return ok;
}

bool Book::SaveNDFF(QString *err)
{
	if (created_but_not_saved())
	{
		return SaveFromScratch(err);
	}
	mtl_tbd();
	//ByteArray ba;
	if (document_content_->CheckChanged(Recursively::Yes))
	{
		
	}
	
	if (document_styles_->CheckChanged(Recursively::Yes))
	{
	}
	
	if (manifest_->CheckChanged(Recursively::Yes))
	{
		
	}
	
	if (document_meta_->CheckChanged(Recursively::Yes))
	{
		
	}
	
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
	const bool ok = out.commit();
	
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
	//return ndff_.WhatCompressionShouldBeUsed(file_path, uncompressed_size);
}

void Book::WriteStartDocument(QXmlStreamWriter &xml)
{
	xml.setAutoFormatting(true);
	xml.setAutoFormattingIndent(-1);
	xml.writeStartDocument(QLatin1String("1.0"));
}

} // ods::
