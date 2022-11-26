#include "FileEntryInfo.hpp"

#include "../ByteArray.hpp"

#include "Container.hpp"

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

FileEntryInfo* FileEntryInfo::From(ByteArray &buf, ci64 loc)
{ // must keep location, Container::ReadFiles() depends on it.
	FileEntryInfo *p = new FileEntryInfo();
	cauto saved = buf.at();
	buf.to(loc);
	p->Read(buf);
	buf.to(saved);
	return p;
}

FileEntryInfo* FileEntryInfo::GetFile(QStringView name) const
{
	auto utf8_name = name.toUtf8();
	
	for (FileEntryInfo *next: subfiles_)
	{
		if (utf8_name == next->path_)
			return next;
	}
	
	return 0;
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

bool FileEntryInfo::ReadSubfiles(ndff::Container *cntr)
{
	return cntr->ReadFiles(subfiles_loc_, subfiles_);
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

void FileEntryInfo::WriteEfa(ByteArray &buf)
{
	buf.add_i64(efa_loc_);
	if (efa_loc_ != -1)
		buf.add(efa_data_, efa_data_len_, ExactSize::Yes);
}

void FileEntryInfo::WriteTo(ByteArray &buf)
{
	buf.add_u16(info_);
	mtl_info("Writing file: %s\"%s\"%s", MTL_BOLD, path_.data(), MTL_BOLD_END);
	buf.add_u16(path_.size());
	buf.add(path_.data(), path_.size(), ExactSize::Yes);
	
	if (has(FeiBit::CRC_32b))
		buf.add_u32(0); // TBD
	
	if (is_regular_file())
	{
		buf.add_i64(content_start_);
		buf.add_i64(extra_region_);
	} else if (is_folder()) {
		buf.add_i64(subfiles_loc_);
	} else if (is_symlink()) {
		buf.add_u16(link_target_.size());
		buf.add(link_target_.data(), link_target_.size(), ExactSize::Yes);
	}
	
	if (has(FeiBit::EFA))
		WriteEfa(buf);
}

}
