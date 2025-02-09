#include "FileEntryInfo.hpp"

#include "../Book.hpp"
#include "../ByteArray.hpp"
#include "Container.hpp"

namespace ods::ndff {

FileEntryInfo::FileEntryInfo(Book *p) : book_(p) {}

FileEntryInfo::~FileEntryInfo() {
	if (efa_data_ && owns_efa_)
	{
		delete[] efa_data_;
		efa_data_ = nullptr;
		owns_efa_ = false;
	}
}

FileEntryInfo* FileEntryInfo::From(ByteArray &from_buf, ci64 loc,
	ods::Book *book)
{ // must keep location, Container::ReadFiles() depends on it.
	FileEntryInfo *fei = new FileEntryInfo(book);
	cauto saved_loc = from_buf.at();
	from_buf.to(loc);
	fei->Read(from_buf);
	from_buf.to(saved_loc);
	return fei;
}

void FileEntryInfo::SetFei(inst::Abstract *top, QStringView fn,
	inst::NsHash &ns_hash, inst::Keywords &keywords,
	ByteArray &output, ci64 record_result_at)
{
	file_data_.Clear();
	top->WriteNDFF(ns_hash, keywords, nullptr, &file_data_);
	self_loc(output.at());
	SetRegularFile(fn);
	SetTimesToNow();
	WriteTo(output);
	
	if (record_result_at != -1) {
		output.set_i64(record_result_at, self_loc());
	} else {
		/* if -1 then this FEI will be an entry in a folder FEI
		which will be done separately by the method caller. */
		mtl_info("Not automatically recording address of %s",
			qPrintable(fn.toString()));
	}
}

void FileEntryInfo::SetFileData(ByteArray &output, QStringView file_path)
{
	content_start_ = output.at();
	output.set_i64(self_loc() + content_start_offset(), content_start_);
	ci64 uncompressed_size = file_data_.size();
	cauto cmpr = book_->WhatCompressionShouldBeUsed(file_path, uncompressed_size);
	//cauto cmpr = Compression::None;
	switch (cmpr) {
	case Compression::None: {
		output.add_i64(uncompressed_size);
		output.add(file_data_);
		auto str = file_path.toLocal8Bit();
		mtl_info("%s file_data size: %lld", str.data(), uncompressed_size);
		break;
	};
	case Compression::ZSTD: {
		ByteArray compressed_buf(file_data_);
		compressed_buf.Compress(Compression::ZSTD);
		cauto compressed_size = compressed_buf.size();
		output.add_i64(compressed_size);
		output.add(compressed_buf);
		UpdateCompressionTo(output, Compression::ZSTD);
		auto str = file_path.toLocal8Bit();
		mtl_info("%s file_data size: %lld (compressed: %lld)",
			str.data(), uncompressed_size, compressed_size);
		break;
	};
	default: {
		mtl_tbd();
	}
	}
}

void FileEntryInfo::SetFolderFei(ByteArray &output, QStringView filename, ci64 record_result_loc)
{
	self_loc(output.at());
	SetFolder(filename);
	SetTimesToNow();
	WriteTo(output);
	if (record_result_loc != -1)
		output.set_i64(record_result_loc, self_loc());
}

FileEntryInfo* FileEntryInfo::GetFile(QStringView name) const
{
	ByteArray ba(name);
	
	for (FileEntryInfo *next: subfiles_)
	{
		if (ba == next->path_)
			return next;
	}
	
	return 0;
}

void FileEntryInfo::PrintFileName(ByteArray &source)
{
	ci64 saved = source.at();
	source.to(self_loc_);
	source.next_u32(); // info
	ci32 str_len = source.next_u16();
	mtl_info("Len: %d, name:", str_len);
	ByteArray name;
	source.next_ba(name, str_len);
	name.to(0);
	for (int i = 0; i < str_len; i++)
	{
		printf("%0X ", *(name.data() + i));
	}
	
	auto s = QString::fromUtf8(name.data(), str_len);
	mtl_info("\nDone: \"%s\"\n", qPrintable(s));
	
	source.to(saved);
}

void FileEntryInfo::Read(ods::ByteArray &input)
{
	info_ = input.next_u32();
	//compression_ = ../
	ci32 str_len = input.next_u16();
	input.next_ba(path_, str_len);
	QString p = path_.toUtf8String();
	mtl_info("Reading FEI: \"%s\", size: %d", qPrintable(p), str_len);
	
	if (has(FeiBit::CRC_32b))
		crc_32b_ = input.next_u32();
	
	if (is_regular_file())
	{
		content_start_ = input.next_i64();
		extra_region_ = input.next_i64();
	} else if (is_folder()) {
		subfiles_loc_ = input.next_i64();
	} else if (is_symlink()) {
		cu16 len = input.next_u16();
		input.next_ba(link_target_, len);
	}
	
	if (has(FeiBit::EFA))
	{
		//mtl_trace("===============================GOT EFA");
		efa_loc_ = input.next_i64();
		if (efa_loc_ != -1)
		{
			efa_data_len_ = input.next_u32();
			efa_data_ = new char[efa_data_len_];
			memcpy(efa_data_, input.data(), efa_data_len_);
			owns_efa_ = true;
		}
	}
}

bool FileEntryInfo::ReadSubfiles(ndff::Container *cntr)
{
	return cntr->ReadFiles(subfiles_loc_, subfiles_);
}

i64 FileEntryInfo::size() const
{
	i64 n = (sizeof info_) + 2 + path_.size();
	
	if (has(FeiBit::CRC_32b))
		n += 4;
	
	if (is_regular_file())
	{
		n += 16; // sizeof content_start_ + sizeof extra_region_;
	} else if (is_folder()) {
		n += 8; // sizeof subfiles_loc_
	} else if (is_symlink()) {
		n += 2 + link_target_.size();
	} else {
		mtl_warn();
	}
	
	if (has(FeiBit::EFA))
		n += 4;
	
	return n;
}

void FileEntryInfo::UpdateCompressionTo(ByteArray &parent, const Compression c)
{
	compression(c);
	cauto saved_loc = parent.at();
	parent.set_u32(self_loc_, info_);
	parent.to(saved_loc);
}

void FileEntryInfo::WriteEfa(ByteArray &output)
{
	output.add_i64(efa_loc_);
	if (efa_loc_ != -1)
		output.add(efa_data_, efa_data_len_, ExactSize::Yes);
}

void FileEntryInfo::WriteTo(ByteArray &output)
{
	output.add_u32(info_);
	mtl_info("Writing FEI: %s, name_len: %d",
		qPrintable(path_.toUtf8String()), path_.size());
	ci32 path_len = path_.size();
	output.add_u16(path_len);
	output.add(path_);
	if (has(FeiBit::CRC_32b))
		output.add_u32(0); // TBD
	
	if (is_regular_file())
	{
		output.add_i64(content_start_);
		output.add_i64(extra_region_);
	} else if (is_folder()) {
		output.add_i64(subfiles_loc_);
	} else if (is_symlink()) {
		output.add_u16(link_target_.size());
		output.add(link_target_);
	}
	
	if (has(FeiBit::EFA))
		WriteEfa(output);
}

}
