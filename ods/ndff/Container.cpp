#include "Container.hpp"

#include "../Book.hpp"
#include "CellNote.hpp"
#include "FileEntryInfo.hpp"
#include "../filename.hxx"
#include "../Ns.hpp"
#include "Property.hpp"
#include "../io.hh"

namespace ods::ndff {

Container::Container() {}
Container::~Container()
{
	delete helper_buf_;
	helper_buf_ = 0;
}

bool Container::GetString(ci32 id, QString &ret_val)
{
	ret_val = id_keyword_.value(id, QString());
	
	// An empty string could be valid? Not sure, so if empty check
	// if it was actually found:
	if (ret_val.isEmpty())
		return id_keyword_.contains(id);
	
	return true;
}

FileEntryInfo* Container::GetTopFile(QStringView filepath) const
{
	for (FileEntryInfo *p: top_files_)
	{
		if (filepath == p->path().toUtf8String())
			return p;
	}
	
	return nullptr;
}

bool Container::Init(Book *p, QStringView full_path)
{
	book_ = p;
	full_path_ = full_path.toString();
	mtl_info("==========>>>>> OPENING NDFF: %s", qPrintable(full_path_));
	io::ReadParams params = {};
	params.can_rely = io::CanRelyOnStatxSize::Yes;
	params.print_errors = io::PrintErrors::Yes;
	MTL_CHECK(io::ReadFile(full_path_, buf_, params));
	MTL_CHECK(buf_.size() > ndff::MainHeaderPlaces::MinSize);
	MTL_CHECK(ndff::CheckMagicNumber(buf_.data()));
	buf_.skip(4);
	cu8 info = buf_.next_u8();
	mtl_info("Document endianness: %s", (info & 1) ? "LE" : "BE");
	maj_version = buf_.next_u8();
	min_version = buf_.next_i16();
	mtl_info("Version: %d.%d", (i32)maj_version, (i32)min_version);
	
	namespaces_loc = buf_.next_i64();
	dictionary_loc = buf_.next_i64();
	top_files_loc = buf_.next_i64();
	// mtl_info("ns_loc: %lld, dict_loc: %lld, top_files_loc: %lld",
	//	namespaces_loc, dictionary_loc, top_files_loc);
	
	doc_type_len = buf_.next_u8();
	doc_type = buf_.NextStringUtf8(doc_type_len);
	mtl_info("Document type: \"%s\"", qPrintable(doc_type));
	
	free_space_trailing = buf_.next_u8();
	mtl_info("Free space trailing: %u bytes", free_space_trailing);
	
	MTL_CHECK(ReadNamespacesRegion());
	MTL_CHECK(ReadDictionaryRegion());
	MTL_CHECK(ReadFiles(top_files_loc, top_files_));

	ns_ = Ns::FromNDFF(this);
	PrepareForParsing();
	//PrintKeywords();
	
	return true;
}

ndff::Op Container::Next(Property &prop, const Op last_op,
	QHash<UriId, QVector<Property>> *h, CellNote *cn)
{
	prop.empty(true);
	ByteArray &buf = helper_buf_ ? *helper_buf_ : buf_;
	auto op = buf.next_op();
	
	if (op == Op::TS)
	{
		prop.uri_id = buf.next_unum();
		prop.name_id = buf.next_inum();
		if (!GetString(prop.name_id, prop.name))
			mtl_warn("Couldn't find name by id %d", prop.name_id);
	} else if (last_op == Op::TS) {
		while (op == Op::S32_PS)
		{
			prop.uri_id = buf.next_unum();
			prop.name_id = buf.next_inum();
			prop.value = buf.next_string(Pack::NDFF);
			if (h)
			{
				if (!GetString(prop.name_id, prop.name))
					mtl_warn("Couldn't find name by id %d", prop.name_id);
//				mtl_info("%s: %s", qPrintable(prop.name), qPrintable(prop.value));
//				((*h)[prop.uri_id]).append(prop);
			}
			op = buf_.next_op();
		}
	}
	
	return op;
}

QString Container::NextString()
{
	buf_.skip(-1);
	return buf_.next_string(Pack::NDFF);
}

bool Container::PrepareFor(FileEntryInfo *fei)
{
	MTL_CHECK(fei);
	buf_.to(fei->content_start());
	ci64 size = buf_.next_i64();
    mtl_info("File %s, size: %lld, compression: %d",
		fei->path().data(), size, i32(fei->compression()));
	if (fei->compression() != Compression::None)
	{
		helper_buf_ = buf_.CloneRegion(buf_.at(), size);
		MTL_CHECK(helper_buf_->Decompress(fei->compression()));
	} else {
		delete helper_buf_;
		helper_buf_ = 0;
	}
	
	return true;
}

void Container::PrepareForParsing()
{
	id_keyword_.clear();
	
	for (auto it = keywords_.constBegin(); it != keywords_.constEnd(); it++)
	{
		id_keyword_[it.value().id] = it.key();
	}
}

void Container::PrintKeywords()
{
	mtl_info("KEYWORDS START ====>>>");
	for (auto it = keywords_.constBegin(); it != keywords_.constEnd(); it++)
	{
		mtl_info("\"%s\", id: %d", qPrintable(it.key()), it.value().id);
	}
	mtl_info("KEYWORDS END   <<<====");
}

void Container::ReadDictionary(ByteArray &buf, ci64 how_much)
{
	ci64 read_till = buf.at() + how_much;
	while (buf.at() < read_till)
	{
		ci32 id = buf.next_inum();
		QString word = buf.next_string(Pack::NDFF);
		//mtl_info("Dictionary entry %d, \"%s\"", id, qPrintable(word));
		keywords_.insert(word, inst::IdAndCount::FromId(id));
	}
}

bool Container::ReadDictionaryRegion()
{/// using Keywords = QHash<QStringView, IdAndCount>;
	if (dictionary_loc == -1)
		return true; // having no dictionary is allowed
	
	buf_.to(dictionary_loc);
	i64 next_block_addr = -1;
	
	do {
		if (next_block_addr != -1)
			buf_.to(next_block_addr);
		
		/// Here starts every dictionary block
		next_block_addr = buf_.next_i64();
		cu32 info = buf_.next_u32();
		// the first 2 fields are not part of the block size
		cu32 block_size = info >> 3;
		cauto compression = Compression(info & 7u);
		if (compression == Compression::None)
		{
			ReadDictionary(buf_, block_size);
		} else if (compression == Compression::ZSTD) {
			auto *temp = buf_.CloneRegion(buf_.at(), block_size);
			cauto before = temp->size();
			temp->Decompress(compression);
            mtl_info("Size decompressed: %lld, compressed: %lld",
				temp->size(), before);
			ReadDictionary(*temp, temp->size());
			delete temp;
			buf_.skip(block_size);
		} else {
			mtl_warn("Unsupported compression");
		}
	} while (next_block_addr != -1);
	
	return true;
}

void Container::ReadNamespaces(ByteArray &buf, ci64 how_much)
{
	ci64 read_till = buf.at() + how_much;
	while (buf.at() < read_till)
	{
		UriId id = buf.next_u16();
		QString uri = buf.next_string(Pack::NDFF);
		ns_hash_.insert(id, uri);
	}
}

bool Container::ReadNamespacesRegion()
{ /// using NsHash = QHash<UriId, QString>;
	MTL_CHECK(namespaces_loc != -1);
	buf_.to(namespaces_loc);
	
	cu32 info = buf_.next_u32();
	cu32 region_size = info >> 3;
	MTL_CHECK(region_size > 0);
	
	const Compression compression = Compression(info & 7);
	if (compression == Compression::None)
	{
		//mtl_info("No compression");
		ReadNamespaces(buf_, region_size);
	} else if (compression == Compression::ZSTD) {
		auto *buf = buf_.CloneRegion(buf_.at(), region_size);
		buf->Decompress(compression);
		ReadNamespaces(*buf, buf->size());
		//mtl_info("ZSTD Compression");
		delete buf;
		buf_.skip(region_size);
	}
	else
        mtl_trace("Unsupported compression: %d", i32(compression));
	
	return true;
}

bool Container::ReadFiles(ci64 files_loc, QVector<FileEntryInfo*> &result_vec)
{
	ci64 HintEndOfList = -1;
	ci64 HintPlaceReserved = -2;
	MTL_CHECK(files_loc != HintEndOfList);
	
	if (files_loc < 0)
	{ // a single file
		ndff::FileEntryInfo *fei = ndff::FileEntryInfo::From(buf_, -files_loc, book_);
		MTL_CHECK(fei);
		result_vec.append(fei);
		return fei->is_folder() ? fei->ReadSubfiles(this) : true;
	}
	
	buf_.to(files_loc);
	while (true)
	{
		ci64 location = buf_.next_i64();
		if (location == HintEndOfList)
			break;
		if (location == HintPlaceReserved)
			continue;
		
		auto *fei = ndff::FileEntryInfo::From(buf_, location, book_);
		MTL_CHECK(fei);
		result_vec.append(fei);
		
		if (fei->is_folder())
			MTL_CHECK(fei->ReadSubfiles(this));
	}
	
	return true;
}

void Container::SwitchToMainBuf()
{
	delete helper_buf_;
	helper_buf_ = 0;
}

Compression Container::WhatCompressionShouldBeUsed(QStringView file_path,
	ci64 uncompressed_size) const
{
//	if (true)
//		return Compression::None;
	
	if (uncompressed_size <= 512)
		return Compression::None; // too small for (ZSTD) compression
	
	if (file_path.endsWith(filename::ContentXml) ||
		file_path.endsWith(filename::StylesXml))
	{
		return Compression::ZSTD;
	}
	
	return Compression::None;
}

} // ods::ndff::
