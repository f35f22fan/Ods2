#include "Container.hpp"

#include "FileEntryInfo.hpp"
#include "../io.hh"

namespace ods::ndff {

bool Container::GetKey(ci32 value, QString &ret_val)
{
	for (auto it = keywords_.constBegin(); it != keywords_.constEnd(); it++)
	{
		if (it.value().id == value)
		{
			ret_val = it.key();
			return true;
		}
	}
	
	return false;
}

FileEntryInfo* Container::GetTopFile(QString filepath) const
{
	for (FileEntryInfo *next: top_files_)
	{
		if (filepath == next->path()) {
			return next;
		}
	}
	
	return nullptr;
}



bool Container::Init(QStringView full_path)
{
	io::ReadParams params = {};
	params.can_rely = io::CanRelyOnStatxSize::Yes;
	params.print_errors = io::PrintErrors::Yes;
	CHECK_TRUE(io::ReadFile(full_path, buf_, params));
	CHECK_TRUE(buf_.size() > ndff::MainHeaderPlaces::MinSize);
	CHECK_TRUE(ndff::CheckMagicNumber(buf_.data()));
	buf_.skip_read(4);
	cu8 byte = buf_.next_u8();
	cu8 info = byte >> 4;
	mtl_info("Document endianness: %s", (info & 1) ? "BE" : "LE");
	maj_version = u16(byte & 0xF) << 8;
	maj_version |= buf_.next_u8();
	min_version = buf_.next_i16();
	mtl_info("Version: %d.%d", (i32)maj_version, (i32)min_version);
	
	namespaces_loc = buf_.next_i64();
	dictionary_loc = buf_.next_i64();
	top_files_loc = buf_.next_i64();
	mtl_info("ns_loc: %ld, dict_loc: %ld, top_files_loc: %ld",
		namespaces_loc, dictionary_loc, top_files_loc);
	
	doc_type_len = buf_.next_u8();
	doc_type = buf_.next_string_utf8(doc_type_len);
	mtl_info("Document type: \"%s\"", qPrintable(doc_type));
	
	CHECK_TRUE(ReadNamespaces());
	CHECK_TRUE(ReadDictionary());
	CHECK_TRUE(ReadTopFiles(top_files_loc, top_files_));

	return true;
}

bool Container::ReadDictionary()
{/// using Keywords = QHash<QStringView, IdAndCount>;
	if (dictionary_loc == -1)
		return true; // no dictionary allowed
	
	buf_.to(dictionary_loc);
	i64 next_block_addr = -1;
	
	do {
		if (next_block_addr != -1)
			buf_.to(next_block_addr);
		
		/// Here starts every dictionary block
		cauto block_starts_at = buf_.at();
		cu32 info = buf_.next_u32();
		next_block_addr = buf_.next_i64();
		
		/// The on disk format for compression info are the first 3 bits of
		/// an u32 in LE order, thus the 3 least significant bits:
		cu8 compression = info & 7u;
		if (compression)
			mtl_warn("Implement compression");
		
		ci64 block_size = info >> 3;
		//mtl_info("Dictionary compression: %d, size: %ld", compression, block_size);
		while (true)
		{
			ci64 num_read = buf_.at() - block_starts_at;
			if (num_read >= block_size)
				break;
			
			cu32 id = buf_.next_unum();
			QString word = buf_.next_string(Pack::NDFF);
			mtl_info("Dictionary entry %u, \"%s\"", id, qPrintable(word));
			keywords_.insert(word, inst::IdAndCount::FromId(id));
		}
	} while (next_block_addr != -1);
	
	return true;
}

bool Container::ReadNamespaces()
{ /// using NsHash = QHash<UriId, QString>;
	CHECK_TRUE(namespaces_loc != -1);
	buf_.to(namespaces_loc);
	
	ci64 was_at = buf_.at();
	ci32 size = buf_.next_i32();
	CHECK_TRUE(size > 0);
	//mtl_info("NS Size: %d", size);
	for (i32 i = 0; i < size;)
	{
		if (buf_.at() - was_at >= size)
			break;
		UriId id = buf_.next_unum();
		QString uri = buf_.next_string(Pack::NDFF);
		ns_hash_.insert(id, uri);
		//mtl_info("URI id %d, path: \"%s\"", id, qPrintable(uri));
	}
	
	
	return true;
}

bool Container::ReadTopFiles(ci64 files_table_loc, QVector<FileEntryInfo*> &vec)
{
	CHECK_TRUE(files_table_loc != -1);
	if (files_table_loc < 0)
	{ // a single file
		cauto save_pos = buf_.at();
		buf_.to(-files_table_loc);
		auto *fei = ndff::FileEntryInfo::From(buf_);
		buf_.to(save_pos);
		CHECK_PTR(fei);
		vec.append(fei);
		if (fei->is_folder())
		{
			ReadTopFiles(fei->subfiles_loc(), fei->subfiles());
		}
		return true;
	}
	
	buf_.to(files_table_loc);
	// -2=place reserved, -1=end of list
	while (true)
	{
		ci64 next_loc = buf_.next_i64();
		if (next_loc == -2)
			continue;
		if (next_loc == -1)
			break;
		
		cauto save_pos = buf_.at();
		buf_.to(next_loc);
		auto *fei = ndff::FileEntryInfo::From(buf_);
		buf_.to(save_pos);
		CHECK_PTR(fei);
		vec.append(fei);
		
		if (fei->is_folder()) {
			ReadTopFiles(fei->subfiles_loc(), fei->subfiles());
		}
	}
	
	return true;
}

} // ods::ndff::
