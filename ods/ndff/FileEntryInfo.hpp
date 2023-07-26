#pragma once

#include "../ByteArray.hpp"
#include "decl.hxx"
#include "../decl.hxx"
#include "../err.hpp"
#include "../types.hxx"
#include "../ods.hxx"

#include <QDateTime>

namespace ods::ndff {

using InfoType = u32;

enum class ODS_API FeiBit: InfoType {
	FileTypes         = 0xFu << 0, // 4 bits
	
	LastAccessed      = 1u << 4,
	TimeCreated       = 1u << 5,
	LastStatusChange  = 1u << 6,
	LastModified      = 1u << 7,
	
	Exec              = 1u << 8,
	Hidden            = 1u << 9,
	HiddenCrossPlatform= 1u << 10, // questionable feature
	EFA               = 1u << 11,
	
	Compressions      = 7u << 12,
	Encryptions       = 0xFu << 13, // 0=None, 1=AES, 2-15.. to be defined
	
	CRC_32b           = 1u << 19,
	
	Reserved          = 0xFFFu << 20 // the remaining 11 bits
};

const InfoType FileTypeRegular = 0;
const InfoType FileTypeFolder = 1;
const InfoType FileTypeSymlink = 2;

const InfoType AllFileTypes = static_cast<InfoType>(FeiBit::FileTypes); // all 4 bits

struct ODS_API Timestamp {
	i64 seconds = -1; // Seconds since the Epoch (UNIX time)
	i32 nano_sec = -1; // Nanoseconds since @seconds

	static Timestamp Now()
	{
		Timestamp now;
		QDateTime t = QDateTime::currentDateTimeUtc();
		now.seconds = t.toSecsSinceEpoch();
		now.nano_sec = t.time().msec() * 1'000'000L;
		
		return now;
	}
};

class ODS_API FileEntryInfo {
public:
	FileEntryInfo(ods::Book *p);
	virtual ~FileEntryInfo();
	
	static FileEntryInfo* From(ByteArray &ba, ci64 loc, Book *b);
	
	Compression compression() const {
		return Compression((info_ >> 12) & 7);
	}
	
	void compression(const Compression c) {
		info_ &= ~0x7000; // clears bits 12, 13, 14
		info_ |= InfoType(c) << 12;;
	}
	
	bool has(const FeiBit bit) const { return info_ & (InfoType)bit; }
	void has(const FeiBit bit, cbool flag, const ClearTheRest clear =
		ClearTheRest::No)
	{
		if (clear == ClearTheRest::Yes)
			info_ &= AllFileTypes;
		
		if (flag)
			info_ |= (InfoType)bit;
		else
			info_ &= ~((InfoType)bit);
	}
	
	bool is_folder() const {
		return (info_ & AllFileTypes) == FileTypeFolder;
	}
	
	bool is_regular_file() const {
		return (info_ & AllFileTypes) == FileTypeRegular;
	}
	
	bool is_symlink() const {
		return (info_ & AllFileTypes) == FileTypeSymlink;
	}
	
	i64 content_start() const { return content_start_; }
	void content_start(ci64 n) { content_start_ = n; }
	i64 content_start_offset() const
	{
		i64 n = ((sizeof info_) + 2) + path_.size();
		if (has(FeiBit::CRC_32b))
			n += 4;
		
		return n;
	}
	const char* efa_data() const { return efa_data_; }
	void efa_data(const char *p, cu32 len, const TakeOwnership o) {
		if (efa_data_ && owns_efa_)
			delete efa_data_;
		efa_data_ = (char*)p;
		efa_data_len_ = len;
		owns_efa_ = (o == TakeOwnership::Yes);
	}
	u32 efa_len() const { return efa_data_len_; }
	
	ByteArray& file_data() { return file_data_; }
	
	FileEntryInfo* GetFile(QStringView name) const;
	bool ReadSubfiles(ndff::Container *cntr);
	QVector<FileEntryInfo*>& subfiles() { return subfiles_; }
	i64 subfiles_loc() const { return subfiles_loc_; }
	void subfiles_loc(ci64 n) { subfiles_loc_= n; }
	InfoType info() const { return info_; }
	void PrintFileName(ByteArray &source);
	i64 self_loc() const { return self_loc_; }
	void self_loc(ci64 n) { self_loc_ = n; }
	
	void SetLastAccessed(const Timestamp &t)
	{
		info_ |= (InfoType)FeiBit::LastAccessed;
		last_accessed_ = t;
	}
	
	void SetLastModified(const Timestamp &t)
	{
		info_ |= (InfoType)FeiBit::LastModified;
		last_modified_ = t;
	}
	
	void SetTimeCreated(const Timestamp &t)
	{
		info_ |= (InfoType)FeiBit::TimeCreated;
		time_created_ = t;
	}
	
	void SetTimesToNow() {
		ndff::Timestamp ts = ndff::Timestamp::Now();
		SetLastAccessed(ts);
		SetLastModified(ts);
		SetTimeCreated(ts);
	}
	
	void SetRegularFile(QStringView path) {
		path_.SetUtf8(path);
		clear_file_type();
		info_ |= FileTypeRegular;
	}
	
	void SetFolder(QStringView path) {
		path_.SetUtf8(path);
		clear_file_type();
		info_ |= FileTypeFolder;
	}
	
	void SetSymlink(QStringView path, QStringView target) {
		path_.SetUtf8(path);
		link_target_.SetUtf8(target);
		clear_file_type();
		info_ |= FileTypeSymlink;
	}
	
	i64 subfiles_field_offset() const {
		return content_start_offset() + 8;
	}
	
	void AddFileData(ByteArray &output, QStringView file_path);
	
	const ByteArray& path() const { return path_; }
	void UpdateCompressionTo(ByteArray &parent, const Compression c);
	/* @size() Computes how much memory to allocate for this FEI inside
	the container */
	i64 size() const;
	void WriteEfa(ByteArray &buf);
	void WriteTo(ByteArray &buf);
	
private:
	
	inline void clear_file_type() {
		info_ &= ~AllFileTypes;
	}
	
	void Read(ods::ByteArray &buf);
	
	ods::Book *book_ = 0;
	ByteArray path_; // string in UTF-8
	i64 self_loc_ = -1;
	
	// for regular files:
	i64 content_start_ = -1;
	i64 extra_region_ = -1;
	
//==> for symlinks
	ByteArray link_target_; // string in UTF-8
//<== for symlinks
	
//==> for folders
	i64 subfiles_loc_ = -1;
	QVector<FileEntryInfo*> subfiles_; // FEI locations that are part of this folder.
//<== for folders

//==> EFA
	char *efa_data_ = nullptr;
	u32 efa_data_len_ = 0;
	bool owns_efa_ = false;
	i64 efa_loc_ = -1;
//<== EFA
	
	ByteArray file_data_;
	u32 crc_32b_ = 0;
	InfoType info_ = 0;
	Timestamp last_accessed_ = {};
	Timestamp last_modified_ = {};
	Timestamp time_created_ = {};
};

}
