#include "Book.hpp"

#include "AutoDelete.hh"
#include "ByteArray.hpp"
#include "filename.hxx"
#include "io.hh"
#include "ndff/FileEntryInfo.hpp"
#include "ndff/ndff.hh"
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
#include <quazip/JlCompress.h>

namespace ods { // ods::

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

void Book::CreateDictionaryRegion(ByteArray &buffer, inst::Keywords &list)
{
	QueryKeywords(list);
	QVector<StringCount> vec;
	
	for (auto it = list.constBegin(); it != list.constEnd(); it++)
	{
		QStringView word = it.key();
		const inst::IdAndCount idc = it.value();
		vec.append({ .keyword = word.toString(), .count = idc.count,
			.id = idc.id});
	}
	
	std::sort(vec.begin(), vec.end(), SortDictionaryEntries);
	
	cauto block_start = buffer.at();
	buffer.add_u32(0); // block info will be here
	buffer.add_i64(-1); // next block address, -1 = no more
	for (StringCount &next: vec)
	{
		//mtl_info("Dictionary entry id: %d \"%s\"", next.id, qPrintable(next.keyword));
		buffer.add_inum(next.id);
		buffer.add_string(next.keyword, Pack::NDFF);
	}
	cu32 block_size = buffer.at() - block_start;
	cu32 no_compression = 0; // 0=None, 1=Zstd
	cu32 block_info = (block_size << 3) | no_compression;
	buffer.set_u32(block_start, block_info);
	mtl_info("Dictionary occupies %u bytes", block_size);
}

void Book::CreateMainHeader(ByteArray &ba)
{
	ba.add((const char*)ndff::MagicNumber, 4, ExactSize::Yes);
	cu8 info = 0; // first bit not set, thus document uses LE order.
	ba.add_u8(info);
//==> Say version 0.1 is the minimum:
	ba.add_u8(0); // smallest major version
	ba.add_i16(1); // smallest minor version or -1 if doesn't matter
//<==
	ba.add_i64(-1); // namespaces_loc will be set later
	ba.add_i64(-1); // dictionary_loc will be set later
	ba.add_i64(-1); // top_files_loc to be set later
	const char *doc_type = "ods";
	cu8 doc_type_len = strlen(doc_type);
	ba.add_u8(doc_type_len); // string length
	cu8 free_space = 64;
	ba.add_u8(free_space);
	ba.add(doc_type, doc_type_len, ExactSize::Yes); // the string itself
	
	if (free_space > 0)
		ba.add_zeroes(free_space);

}

void Book::CreateNamespacesRegion(ByteArray &result, inst::NsHash &h)
{
	auto was_at = result.at();
	result.add_i32(0); // size in bytes, will be set at method end
	QueryUsedNamespaces(h, CreateIfNeeded::Yes);
	auto it = h.constBegin();
	while (it != h.constEnd())
	{
		const UriId uri_id = it.key();
		const QString uri = it.value();
		it++;
		result.add_unum(uri_id);
		mtl_info("URI ID: %d, path \"%s\"", uri_id, qPrintable(uri));
		result.add_string(uri, Pack::NDFF);
	}
	
	ci32 total_size = result.size() - was_at;
	result.set_i32(was_at, total_size);
	mtl_info("%sNamespaces occupy %d bytes, start_at: %ld%s", MTL_BOLD,
		total_size, was_at, MTL_BOLD_END);
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
	book->Load(full_path, err);
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
	CHECK_TRUE(ndff_.Init(this, full_path));
	auto &ba = ndff_.buf();
	{
		ndff::FileEntryInfo *fei = ndff_.GetTopFile(filename::ContentXml);
		ba.to(fei->content_start());
		document_content_ = new inst::OfficeDocumentContent(this, ndff_.ns(), 0, &ndff_);
	}
	{
		ndff::FileEntryInfo *fei = ndff_.GetTopFile(filename::StylesXml);
		ba.to(fei->content_start());
		document_styles_ = new inst::OfficeDocumentStyles(this, ndff_.ns(), 0, &ndff_);
	}
	{
		ndff::FileEntryInfo *fei = ndff_.GetTopFile(filename::MetaXml);
		ba.to(fei->content_start());
		document_meta_ = new inst::OfficeDocumentMeta(this, ndff_.ns(), 0, &ndff_);
	}
	{
		auto *folder = ndff_.GetTopFile(filename::MetaInf);
		CHECK_PTR(folder);
		ndff::FileEntryInfo *fei = folder->GetFile(filename::ManifestXml);
		if (!fei)
		{
			mtl_trace("No manifest fei");
			return false;
		}
		ba.to(fei->content_start());
		manifest_ = new inst::ManifestManifest(this, ndff_.ns(), 0, &ndff_);
	}
	return true;
}

void Book::Load(QStringView full_path, QString *err)
{
	InitTempDir();
	
	if (!dev_mode() && !temp_dir_.isValid())
	{
		mtl_warn("temp dir invalid");
		return;
	}
	
	extracted_file_paths_ = JlCompress::extractDir(full_path.toString(),
		temp_dir_path_);
	
	if (extracted_file_paths_.isEmpty()) {
		if (err != nullptr)
			*err = QLatin1String("Couldn't extract files");
		return;
	}
	
	{
		cauto file_index = ods::FindIndexThatEndsWith(extracted_file_paths_,
			QString(ods::filename::ManifestXml));
		CHECK_TRUE_VOID(file_index != -1);
		LoadManifestXml(file_index, err);
	}
	
	{
		cauto file_index = ods::FindIndexThatEndsWith(extracted_file_paths_,
			QString(ods::filename::ContentXml));
		CHECK_TRUE_VOID(file_index != -1);
		LoadContentXml(file_index, err);
	}
	
	{
		cauto file_index = ods::FindIndexThatEndsWith(extracted_file_paths_,
			QString(ods::filename::MetaXml));
		CHECK_TRUE_VOID(file_index != -1);
		LoadMetaXml(file_index, err);
	}
	
	{
		cauto file_index = ods::FindIndexThatEndsWith(extracted_file_paths_,
			QString(ods::filename::StylesXml));
		CHECK_TRUE_VOID(file_index != -1);
		LoadStylesXml(file_index, err);
	}
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

void Book::QueryKeywords(inst::Keywords &list)
{
	QVector<inst::Abstract*> tops = GetNamespaceClasses();
	
	for (inst::Abstract *top: tops)
	{
		top->ListKeywords(list, inst::LimitTo::All);
		
		QVector<StringOrInst*> children;
		top->ListChildren(children, Recursively::Yes);
		
		for (StringOrInst *next: children)
		{
			if (next->is_inst())
				next->as_inst()->ListKeywords(list, inst::LimitTo::All);
		}
	}
}

void Book::QueryUsedNamespaces(inst::NsHash &list, const CreateIfNeeded create_default)
{
	QVector<inst::Abstract*> tops = GetNamespaceClasses();
	
	if (create_default == CreateIfNeeded::Yes && tops.isEmpty())
	{
		Ns *ns = Ns::Default();
		auto prefixes = ns->prefixes();
		
		for (Prefix *pref: prefixes)
		{
			list[pref->id()] = pref->uri();
		}
		
		return;
	}
	
	for (inst::Abstract *top: tops)
	{
		top->ListUsedNamespaces(list);
		
		QVector<StringOrInst*> children;
		top->ListChildren(children, Recursively::Yes);
		
		for (StringOrInst *next: children)
		{
			if (next->is_inst())
				next->as_inst()->ListUsedNamespaces(list);
		}
	}
}

bool Book::Save(const QFile &target, QString *err)
{
	if (document_content_ == nullptr || document_styles_ == nullptr)
	{
		mtl_warn();
		return false;
	}
	
	QElapsedTimer timer;
	timer.start();
	QDir temp_dir(temp_dir_path_);
	
	QString full_path;
	
	full_path = temp_dir.filePath(filename::ContentXml);
	SaveXmlFile(document_content_, full_path, err);
	
	full_path = temp_dir.filePath(filename::StylesXml);
	SaveXmlFile(document_styles_, full_path, err);
	
	QDir meta_dir(temp_dir.filePath(filename::MetaInf));
	
	if (!meta_dir.exists())
	{
		if (!temp_dir.mkdir(filename::MetaInf)) {
			if (err != nullptr)
				*err = QString("Failed to create meta-inf dir");
			return false;
		}
	}
	
	full_path = meta_dir.filePath(filename::ManifestXml);
	SaveXmlFile(manifest_, full_path, err);
	
	full_path = temp_dir.filePath(filename::MetaXml);
	SaveXmlFile(document_meta_, full_path, err);
	
	{ // mimetype
		full_path = temp_dir.filePath(ods::filename::MimeType);
		QFile file(full_path);
		
		if (file.open(QIODevice::WriteOnly))
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
	
	if (ndff()) {
		auto ms = timer.elapsed();
		mtl_info("Save() took %lld ms", ms);
		timer.restart();
		SaveNDFF(err);
		mtl_info("SaveNDFF() took %lld ms", timer.elapsed());
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

void Book::AddFolderFei(ByteArray &buffer, ndff::FileEntryInfo &fei,
	QString filename, ci64 record_result_loc)
{
	fei.self_loc(buffer.at());
	fei.SetFolder(filename);
	fei.SetTimesToNow();
	fei.WriteTo(buffer);
	if (record_result_loc != -1)
		buffer.set_i64(record_result_loc, fei.self_loc());
}

void Book::AddFei(inst::Abstract *top, QStringView fn,
	inst::NsHash &ns_hash,
	inst::Keywords &keywords,
	ndff::FileEntryInfo &fei,
	ByteArray &main_buffer,
	ci64 record_result_loc)
{
	top->WriteNDFF(ns_hash, keywords, nullptr, &fei.file_data());
	
	fei.self_loc(main_buffer.at());
	fei.SetRegularFile(fn);
	fei.SetTimesToNow();
	fei.WriteTo(main_buffer);
	
	if (record_result_loc != -1) {
		main_buffer.set_i64(record_result_loc, fei.self_loc());
	} else {
		/* if -1 then this FEI will be an entry in a folder FEI
		which will be done separately by the method caller. */
		auto ba = fn.toLocal8Bit();
		mtl_info("Not automatically recording address of %s", ba.data());
	}
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

i64 Book::CreateFeiTable(ByteArray &buffer, cu32 reserve_count,
	ci64 record_table_loc)
{
	ci64 table_addr = buffer.at();
	for (u32 i = 0; i < reserve_count; i++)
	{
		buffer.add_i64(ndff::ReservedFeiTableValue);
	}
	
	buffer.add_i64(ndff::EndOfFeiTable);
	if (record_table_loc != -1) {
		buffer.set_i64(record_table_loc, table_addr);
	}
	
	return table_addr;
}

bool Book::SaveFromScratch(QString *err)
{
	QString full_path = QDir::home().filePath("out.ndff");
	QSaveFile ndff_file(full_path);
	CHECK_TRUE(ndff_file.open(QIODevice::WriteOnly | QIODevice::Truncate));
	
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
	CHECK_TRUE(fei_table_loc > 0);
	i64 next_fei_loc = fei_table_loc;
	
	ndff::FileEntryInfo contents_fei;
	AddFei(document_content_, filename::ContentXml,
		ns_hash, keywords,
		contents_fei,
		main_buffer, next_fei_loc);
	
	ndff::FileEntryInfo styles_fei;
	AddFei(document_styles_, filename::StylesXml,
		ns_hash, keywords, styles_fei,
		main_buffer, next_fei_loc += 8);
	
	ndff::FileEntryInfo meta_fei;
	AddFei(document_meta_, filename::MetaXml,
		ns_hash, keywords, meta_fei,
		main_buffer, next_fei_loc += 8);
	
	ndff::FileEntryInfo manifest_fei;
	AddFei(manifest_, filename::ManifestXml,
		ns_hash, keywords, manifest_fei,
		main_buffer, -1);
	
	/// "META-INF" folder
	ndff::FileEntryInfo folder_fei;
	{
		/// Tying "META-INFO/manifest.xml" to its parent folder
		folder_fei.subfiles_loc(-manifest_fei.self_loc());
		AddFolderFei(main_buffer, folder_fei, filename::MetaInf,
			next_fei_loc += 8);
	}
	
	ndff::FileEntryInfo mimetype_fei;
	CHECK_TRUE(CreateMimetypeFei(main_buffer, mimetype_fei, next_fei_loc += 8));
	
	mimetype_fei.AddFileData(main_buffer, mimetype_fei.file_data(), filename::MimeType);
	meta_fei.AddFileData(main_buffer, meta_fei.file_data(), filename::MetaXml);
	styles_fei.AddFileData(main_buffer, styles_fei.file_data(), filename::StylesXml);
	contents_fei.AddFileData(main_buffer, contents_fei.file_data(), filename::ContentXml);
	manifest_fei.AddFileData(main_buffer, manifest_fei.file_data(), filename::ManifestXml);
	
	mimetype_fei.WriteEfa(main_buffer);
	meta_fei.WriteEfa(main_buffer);
	styles_fei.WriteEfa(main_buffer);
	contents_fei.WriteEfa(main_buffer);
	manifest_fei.WriteEfa(main_buffer);
	folder_fei.WriteEfa(main_buffer);
	
	CHECK_TRUE(WriteAll(ndff_file, main_buffer.data(), main_buffer.size(), err));
	
	cbool ok = ndff_file.commit();
	if (!ok && err)
		*err = QString("Failed to save as ") + full_path;
	
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

void Book::WriteStartDocument(QXmlStreamWriter &xml)
{
	xml.setAutoFormatting(true);
	xml.setAutoFormattingIndent(-1);
	xml.writeStartDocument(QLatin1String("1.0"), false);
}

} // ods::
