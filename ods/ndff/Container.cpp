#include "Container.hpp"

#include "../Book.hpp"
#include "FileEntryInfo.hpp"
#include "../Ns.hpp"
#include "Property.hpp"
#include "../io.hh"

namespace ods::ndff {

Container::Container() {}
Container::~Container() {}

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
		if (filepath == QString(p->path()))
			return p;
	}
	
	return nullptr;
}

bool Container::Init(Book *p, QStringView full_path)
{
	book_ = p;
	full_path_ = full_path.toString();
	mtl_info("OPENING NDFF: %s", qPrintable(full_path_));
	io::ReadParams params = {};
	params.can_rely = io::CanRelyOnStatxSize::Yes;
	params.print_errors = io::PrintErrors::Yes;
	CHECK_TRUE(io::ReadFile(full_path_, buf_, params));
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
	free_space_trailing = buf_.next_u8();
	mtl_info("Free space trailing: %u bytes", free_space_trailing);
	doc_type = buf_.next_string_utf8(doc_type_len);
	mtl_info("Document type: \"%s\"", qPrintable(doc_type));
	
	CHECK_TRUE(ReadNamespaces());
	CHECK_TRUE(ReadDictionary());
	CHECK_TRUE(ReadFiles(top_files_loc, top_files_));

	ns_ = Ns::FromNDFF(this);
	PrepareForParsing();
	//PrintKeywords();
	
	book_->ndff(true);
	
	return true;
}

ndff::Op Container::Next(Property &prop, const Op last_op,
	QHash<UriId, QVector<Property>> *h)
{
	prop.empty(true);
	auto op = buf_.next_op();
	
	if (op == Op::TS)
	{
		prop.uri_id = buf_.next_unum();
		prop.name_id = buf_.next_inum();
		if (!GetString(prop.name_id, prop.name))
			mtl_warn("Couldn't find name by id %d", prop.name_id);
	} else if (last_op == Op::TS) {
		while (op == Op::S32_PS)
		{
			prop.uri_id = buf_.next_unum();
			prop.name_id = buf_.next_inum();
			prop.value = buf_.next_string(Pack::NDFF);
			if (h)
			{
				if (!GetString(prop.name_id, prop.name))
					mtl_warn("Couldn't find name by id %d", prop.name_id);
				//mtl_printq2("Property value: ", prop.value);
				((*h)[prop.uri_id]).append(prop);
			}
			op = buf_.next_op();
		}
	}
	
	return op;
}

QString Container::NextString()
{
	buf_.skip_read(-1);
	return buf_.next_string(Pack::NDFF);
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

bool Container::ReadDictionary()
{/// using Keywords = QHash<QStringView, IdAndCount>;
	if (dictionary_loc == -1)
		return true; // having no dictionary is allowed
	
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
		mtl_info("Dictionary compression: %d, size: %ld", compression, block_size);
		while (true)
		{
			ci64 num_read = buf_.at() - block_starts_at;
			if (num_read >= block_size)
				break;
			
			ci32 id = buf_.next_inum();
			QString word = buf_.next_string(Pack::NDFF);
			//mtl_info("Dictionary entry %d, \"%s\"", id, qPrintable(word));
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

bool Container::ReadFiles(ci64 files_loc, QVector<FileEntryInfo*> &result_vec)
{
	ci64 HintEndOfList = -1;
	ci64 HintPlaceReserved = -2;
	CHECK_TRUE(files_loc != HintEndOfList);
	
	if (files_loc < 0)
	{ // a single file
		ndff::FileEntryInfo *fei = ndff::FileEntryInfo::From(buf_, -files_loc);
		CHECK_PTR(fei);
		result_vec.append(fei);
		return fei->is_folder() ? fei->ReadSubfiles(this) : true;
	}
	
	buf_.to(files_loc);
	while (true)
	{
		ci64 next_loc = buf_.next_i64();
		if (next_loc == HintEndOfList)
			break;
		if (next_loc == HintPlaceReserved)
			continue;
		
		auto *fei = ndff::FileEntryInfo::From(buf_, next_loc);
		CHECK_PTR(fei);
		result_vec.append(fei);
		
		if (fei->is_folder())
			CHECK_TRUE(fei->ReadSubfiles(this));
	}
	
	return true;
}

} // ods::ndff::
