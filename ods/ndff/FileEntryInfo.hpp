#pragma once

#include "../ByteArray.hpp"
#include "decl.hxx"
#include "../decl.hxx"
#include "../err.hpp"
#include "../types.hxx"
#include "../ods.hxx"

#include <QDateTime>

namespace ods::ndff {

// Note: using 3 bits, not 2 in case in the future
// might need to store other file types:
cu16 FileTypeSymlink = 1;
cu16 FileTypeFolder = 2;
cu16 FileTypeRegular = 3;
cu16 AllFileTypes = 7;

using InfoType = u16;
enum class ODS_API FeiBit: InfoType {
	LastAccessed      = 1 << 3,
	LastModified      = 1 << 4,
	TimeCreated       = 1 << 5,
	ExecBit           = 1 << 6,
	EFA               = 1 << 7,
	ZSTD              = 1 << 8,
	Compression2      = 1 << 9,
	Compression3      = 1 << 10,
	Encryption_AES    = 1 << 11,
	Encryption_2      = 1 << 12,
	Encryption_3      = 1 << 13,
	CRC_32b           = 1 << 14,
	Reserved_1        = 1 << 15
};

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
	FileEntryInfo();
	virtual ~FileEntryInfo();
	
	static FileEntryInfo* From(ByteArray &ba, ci64 loc);
	
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
	i64 content_start_offset() const {
		i64 n = 4 + path_.size();
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
	u16 info() const { return info_; }
	
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
		path_ = path.toUtf8();
		clear_file_type();
		info_ |= FileTypeRegular;
	}
	
	void SetFolder(QStringView path) {
		path_ = path.toUtf8();
		clear_file_type();
		info_ |= FileTypeFolder;
	}
	
	void SetSymlink(QStringView path, QStringView target) {
		path_ = path.toUtf8();
		link_target_ = target.toUtf8();
		clear_file_type();
		info_ |= FileTypeSymlink;
	}
	
	i64 subfiles_field_offset() const {
		return content_start_offset() + 8;
	}
	
	void AddFileData(ByteArray &main_buffer, const ByteArray &file_data, QStringView filename);
	
	const QByteArray& path() const { return path_; }
	
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
	
	QByteArray path_; // string in UTF-8
	i64 self_loc_ = -1;
	
	// for regular files:
	i64 content_start_ = -1;
	i64 extra_region_ = -1;
	
//==> for symlinks
	QByteArray link_target_; // string in UTF-8
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
