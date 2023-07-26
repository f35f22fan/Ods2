#pragma once

#include "ods.hxx"
#include "global.hxx"
#include "ndff/ndff.hh"

#include <QString>
#include <vector>

namespace ods {

enum class Pack: i8 {
	Standard,
	NDFF,
};

class ODS_API ByteArray {
public:
	ByteArray();
	ByteArray(const ByteArray &rhs);
	ByteArray(QStringView name);
	ByteArray& operator = (const ByteArray &rhs);
	bool operator == (const ByteArray &rhs);
	ByteArray* CloneFromHere();
	ByteArray* CloneRegion(ci64 offset, ci64 len);
	virtual ~ByteArray();
	
	void alloc(const isize n);
	
	void add_zeroes(cisize byte_count);
	void add(const ByteArray *rhs, const From from);
	void add(const char *p, cisize size, const ExactSize es = ExactSize::No);
	void add(const QByteArray *rhs) {
		add(rhs->data(), rhs->size(), ExactSize::Yes);
	}
	void add_i8(const i8 n);
	void add_u8(const u8 n);
	void add_i16(const i16 n);
	void add_u16(const u16 n);
	void add_i32(const i32 n);
	void add_u32(const u32 n);
	void add_i64(const i64 n);
	void add_u64(const u64 n);
	void add_f32(const float n);
	void add_f64(const f64 n);
	void add_string(QStringView s, const Pack p = Pack::Standard);
	isize at() const { return at_; }
	bool at_end() const { return at_ >= size_; }
	void Clear();
	bool Compress(const Compression);
	bool Decompress(const Compression);
	void DumpToTerminal();
	char *data() const { return data_; }
	const char *constData() const { return data_; }
	
	bool has_more(const isize n) const { return at_ + n <= size_; }
	bool has_more() const { return at_ < size_; }
	bool is_empty() const { return size_ == 0; }
	void next(char *p, const isize sz);
	bool next_ba(ByteArray &output, ci64 len);
	QString NextStringUtf8(ci64 len);
	void join_i(const ndff::Op op, ci64 n);
	void join_u(const ndff::Op op, cu64 n);
	void add_str_size(cu64 n);
	void add_unum(cu64 n);
	void add_inum(ci64 n);
	
	ndff::Op next_op() { 
		return (ndff::Op) (next_u8() & 0xFu);
	}
	
	i8 next_i8();
	u8 next_u8();
	i16 next_i16();
	u16 next_u16();
	i32 next_i32();
	u32 next_u32();
	i64 next_i64();
	u64 next_u64();
	f32 next_f32();
	f64 next_f64();
	
	QString next_string(const Pack p = Pack::Standard);
	u64 next_str_size();
	i64 next_inum();
	u64 next_unum();
	
	inline void set_i32(cisize at, ci32 value) {
		memcpy(data_ + at, &value, sizeof value);
	}
	
	inline void set_u32(cisize at, cu32 value) {
		memcpy(data_ + at, &value, sizeof value);
	}
	
	inline void set_i64(cisize at, ci64 value) {
		memcpy(data_ + at, &value, sizeof value);
	}
	
	inline void set_value(cisize at, void *value, cisize sz) {
		memcpy(data_ + at, value, sz);
	}
	
	isize size() const { return size_; }
	void size(cisize n) { size_ = n; }
	void skip_read(cisize n) { at_ += n; }
	isize heap_size() const { return heap_size_; }
	void MakeSure(cisize more_bytes, const ExactSize es = ExactSize::No);
	inline void to(cisize n) { at_ = n; }
	QString toString() const { return QString::fromLocal8Bit(data_, size_); }
	QString toUtf8String() const { return QString::fromUtf8(data_, size_); }
	void SetUtf8(QStringView s, const ods::Clear c = ods::Clear::Yes);

private:
///	NO_ASSIGN_COPY_MOVE(ByteArray);
	void TakeOver(char *buf, ci64 heap_size, ci64 size);
	
	isize size_ = 0;
	isize heap_size_ = 0;
	isize at_ = 0;
	char *data_ = nullptr;
};

}
