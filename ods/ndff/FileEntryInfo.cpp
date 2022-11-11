#include "FileEntryInfo.hpp"

#include "../ByteArray.hpp"

namespace ods::ndff {

FileEntryInfo::FileEntryInfo() {
	
}

FileEntryInfo::~FileEntryInfo() {
	if (efa_data_ && owns_efa_)
	{
		delete[] efa_data_;
		efa_data_ = nullptr;
		owns_efa_ = false;
	}
}

FileEntryInfo* FileEntryInfo::From(ByteArray &buf)
{
	FileEntryInfo *p = new FileEntryInfo();
	p->Read(buf);
	return p;
}

void FileEntryInfo::Read(ods::ByteArray &buf)
{
	info_ = buf.next_u16();
	cu16 len = buf.next_u16();
	path_ = buf.next_string_utf8(len);
	//mtl_info("Reading file: \"%s\"", path_.data());
	
	if (has(FeiBit::CRC_32b))
		crc_32b_ = buf.next_u32();
	
	if (is_regular_file())
	{
		content_start_ = buf.next_i64();
		extra_region_ = buf.next_i64();
	} else if (is_folder()) {
		subfiles_loc_ = buf.next_i64();
	} else if (is_symlink()) {
		cu16 len = buf.next_u16();
		link_target_ = buf.next_string_utf8(len);
	}
	
	if (has(FeiBit::EFA))
	{
		efa_loc_ = buf.next_i64();
		if (efa_loc_ != -1)
		{
			efa_data_len_ = buf.next_u32();
			efa_data_ = new char[efa_data_len_];
			memcpy(efa_data_, buf.data(), efa_data_len_);
			owns_efa_ = true;
		}
	}
}

i64 FileEntryInfo::size() const
{
	i64 n = 4 + path_.size();
	
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

void FileEntryInfo::AddFileData(ByteArray &main_buffer,
	const ByteArray &file_data, QStringView filename)
{
	content_start_ = main_buffer.at();
	main_buffer.add(&file_data, From::Start);
	main_buffer.set_i64(self_loc() + content_start_offset(), content_start_);
	auto str = filename.toLocal8Bit();
	mtl_info("%s file_data size: %ld", str.data(), file_data.size());
}

void FileEntryInfo::WriteEfa(ByteArray &main_buffer)
{
	main_buffer.add_i64(efa_loc_);
	if (efa_loc_ != -1)
		main_buffer.add(efa_data_, efa_data_len_, ExactSize::Yes);
}

void FileEntryInfo::WriteTo(ByteArray &main_buffer)
{
	main_buffer.add_u16(info_);
	mtl_info("Writing file: \"%s\"", path_.data());
	main_buffer.add_u16(path_.size());
	main_buffer.add(path_.data(), path_.size(), ExactSize::Yes);
	
	if (has(FeiBit::CRC_32b))
		main_buffer.add_u32(0); // TBD
	
	if (is_regular_file())
	{
		main_buffer.add_i64(content_start_);
		main_buffer.add_i64(extra_region_);
	} else if (is_folder()) {
		main_buffer.add_i64(subfiles_loc_);
	} else if (is_symlink()) {
		main_buffer.add_u16(link_target_.size());
		main_buffer.add(link_target_.data(), link_target_.size(), ExactSize::Yes);
	}
	
	if (has(FeiBit::EFA))
	{
		WriteEfa(main_buffer);
	}
}

}
