#include "ByteArray.hpp"

#include "err.hpp"
#include "io.hh"

#include <QtEndian>
#include <QSysInfo>

#include <string.h>
//#include <zstd.h>

namespace ods {

ByteArray::ByteArray() {}
ByteArray::ByteArray(const ByteArray &rhs)
{
	add(rhs.data(), rhs.size());
	at_ = 0;
}
ByteArray::ByteArray(QStringView name)
{
	SetUtf8(name, ods::Clear::No);
}
ByteArray::~ByteArray()
{
	Clear();
}

ByteArray& ByteArray::operator = (const ByteArray &rhs)
{
	Clear();
	add(rhs.data(), rhs.size());
	at_ = 0;
	return *this;
}

bool ByteArray::operator == (const ByteArray &rhs)
{
	if (size_ != rhs.size_)
		return false;
	
	return (memcmp(data_, rhs.data(), size_) == 0);
}

void ByteArray::Clear()
{
	delete[] data_;
	data_ = nullptr;
	size_ = heap_size_ = at_ = 0;
}

ByteArray* ByteArray::CloneFromHere()
{
	ci64 left = size_ - at_;
	if (left <= 0)
		return nullptr;
	
	ByteArray *ret = new ByteArray();
	ret->add(*this, From::CurrentPosition);
	
	return ret;
}

ByteArray* ByteArray::CloneRegion(ci64 from, ci64 how_much)
{
	ByteArray *p = new ByteArray();
	p->add(data_ + from, how_much, ExactSize::Yes);
	p->to(0);
	return p;
}

void ByteArray::add(const ByteArray &rhs, const From from)
{
	char *buf;
	i64 buf_len;
	if (from == From::CurrentPosition)
	{
		buf = rhs.data() + rhs.at();
		buf_len = rhs.size() - rhs.at();
	} else {
		buf = rhs.data();
		buf_len = rhs.size();
	}
	
	add(buf, buf_len, ExactSize::Yes);
}

void ByteArray::add(const char *p, cisize size, const ExactSize es)
{
	MakeSure(size, es);
	memcpy(data_ + at_, p, size);
	at_ += size;
	size_ += size;
}

void ByteArray::add_zeroes(cisize byte_count)
{
	if (byte_count > 0)
	{
		MakeSure(byte_count, ExactSize::Yes);
		memset(data_ + at_, 0, byte_count);
		at_ += byte_count;
		size_ += byte_count;
	}
}

void ByteArray::add_i8(const i8 n) {
	add(reinterpret_cast<const char*>(&n), sizeof n);
}

void ByteArray::add_u8(const u8 n) {
	add(reinterpret_cast<const char*>(&n), sizeof n);
}

void ByteArray::add_i16(const i16 n) {
	add(reinterpret_cast<const char*>(&n), sizeof n);
}

void ByteArray::add_u16(const u16 n) {
	add(reinterpret_cast<const char*>(&n), sizeof n);
}

void ByteArray::add_i32(const i32 n) {
	add(reinterpret_cast<const char*>(&n), sizeof n);
}

void ByteArray::add_u32(const u32 n) {
	add(reinterpret_cast<const char*>(&n), sizeof n);
}

void ByteArray::add_i64(const i64 n) {
	add(reinterpret_cast<const char*>(&n), sizeof n);
}

void ByteArray::add_u64(const u64 n) {
	add(reinterpret_cast<const char*>(&n), sizeof n);
}

void ByteArray::add_f32(const f32 n)
{
	add(reinterpret_cast<const char*>(&n), sizeof n);
}

void ByteArray::add_f64(const f64 n)
{
	add(reinterpret_cast<const char*>(&n), sizeof n);
}

void ByteArray::add_string(QStringView s, const Pack p)
{
	if (p == Pack::Standard)
	{
		auto ba = s.toLocal8Bit();
		ci32 size = ba.size();
		add(reinterpret_cast<const char*>(&size), sizeof size);
		add(ba.data(), size);
	} else if (p == Pack::NDFF) {
		auto str_ba = s.toUtf8();
		ci64 size = str_ba.size();
		add_str_size(size);
		add(str_ba.data(), size, ExactSize::Yes);
	} else {
		mtl_trace();
	}
}

void ByteArray::add_str_size(cu64 n)
{
	// ndff::OP must be in the first byte
	if (n <= (0xFFu >> 4)) {
		add_u8((u8(n) << 4) | ndff::Op::S8);
	} else if (n <= (0xFFFFu >> 4)) {
		add_u16((u16(n) << 4) | u16(ndff::Op::S16));
	} else if (n <= (0xFFFFFFFFu >> 4)) {
		add_u32((u32(n) << 4) | u32(ndff::Op::S32_PS));
	} else {
		add_u64((n << 4) | u64(ndff::Op::S64));
	}
}

void ByteArray::add_inum(ci64 n)
{ // ndff::OP must be in the first byte
	cint leave_3_bits = 5;
	if (n < (0xFF >> leave_3_bits)) {
		add_i8((i8(n) << 4) | ndff::Op::N8);
	} else if (n < (0xFFFF >> leave_3_bits)) {
		add_i16((i16(n) << 4) | i16(ndff::Op::N16));
	} else if (n < (0xFFFFFFFF >> leave_3_bits)) {
		add_i32((i32(n) << 4) | i32(ndff::Op::N32_TE));
	} else {
		add_u8(ndff::Op::N64);
		add_i64(n);
	}
}

void ByteArray::add_unum(cu64 n)
{ // ndff::OP must be in the first byte
	if (n <= (0xFFu >> 4)) {
		add_u8((u8(n) << 4) | ndff::Op::N8);
	} else if (n <= (0xFFFFu >> 4)) {
		add_u16((u16(n) << 4) | u16(ndff::Op::N16));
	} else if (n <= (0xFFFFFFFFu >> 4)) {
		add_u32((u32(n) << 4) | u32(ndff::Op::N32_TE));
	} else {
		//mtl_info("==========UNUM: %lu", n);
		add_u8(ndff::Op::N64);
		add_u64(n);
	}
}

bool ByteArray::Compress(const Compression compr)
{
	MTL_CHECK(compr == Compression::ZSTD);
	if (true) {
		mtl_tbd();
		return false;
	}
	/// Compresses the whole buffer
	// auto *zstd_cctx = ZSTD_createCCtx();
	// const char *src_buf = (const char*)data();
	// ci64 src_size = size();
	// ci64 dst_size = ZSTD_compressBound(src_size);
	// char *dst_buf = new char[dst_size];
	// ci64 compressed_size = ZSTD_compressCCtx(zstd_cctx,
	// 	 dst_buf, dst_size, src_buf, src_size, 1);
	// TakeOver(dst_buf, dst_size, compressed_size);
	
	return true;
}

bool ByteArray::Decompress(const Compression compr)
{
	MTL_CHECK(compr == Compression::ZSTD);
	
	if (true) {
		mtl_tbd();
		return false;
	}
	
	// ZSTD_DCtx *zstd_dctx = ZSTD_createDCtx();
	// char *src_buf = data_;
	// ci64 src_size = size_;
	// cu64 dst_size = ZSTD_getFrameContentSize(src_buf, src_size);
	// if (dst_size == ZSTD_CONTENTSIZE_UNKNOWN)
	// 	mtl_warn("Unknown zstd error");
	// else if (dst_size == ZSTD_CONTENTSIZE_ERROR) {
	// 	mtl_warn("ZSTD content error");
	// 	DumpToTerminal();
	// }
	// uchar *dst_buf = new uchar[dst_size];
	// ci64 decompressed_size = ZSTD_decompressDCtx(zstd_dctx,
	//  dst_buf, dst_size, src_buf, src_size);
	// mtl_info("src_size: %lu, dst_size: %lu, decompr_size: %lu",
	// 	src_size, dst_size, decompressed_size);
	// TakeOver((char*)dst_buf, dst_size, decompressed_size);
	
	return true;
}

bool ByteArray::DumpToTerminal(QStringView full_path)
{
	if (!full_path.isEmpty())
	{
		MTL_CHECK(io::WriteToFile(full_path, data_, size_));
		return true;
	}
	
	ci64 max = (size_ > 255) ? 255 : size_;
	mtl_info("Dumping %ld bytes:", max);
	for (i64 i = 0; i < max; i++)
	{
		printf("%0X ", data_[i]);
	}
	
	printf("\n");
	
	return true;
}

i64 ByteArray::next_inum()
{
	cu8 bits = next_u8() & 0x0F;
	skip(-1);
	if (bits == ndff::Op::N8) {
		return next_i8() >> 4;
	} else if (bits == ndff::Op::N16) {
		return next_i16() >> 4;
	} else if (bits == ndff::Op::N32_TE) {
		return next_i32() >> 4;
	} else if (bits == ndff::Op::N64) {
		skip(1); // because it's not inline
		return next_i64();
	}
	
	mtl_warn();
	return -1;
}

u64 ByteArray::next_str_size()
{
	cu8 bits = next_u8() & 0x0Fu;
	skip(-1);
	if (bits == ndff::Op::S8) {
		return next_u8() >> 4;
	} else if (bits == ndff::Op::S16) {
		return next_u16() >> 4;
	} else if (bits == ndff::Op::S32_PS) {
		return next_u32() >> 4;
	} else if (bits == ndff::Op::S64) {
		return next_u64() >> 4;
	} else {
		mtl_warn();
		return -1;
	}
}

u64 ByteArray::next_unum()
{
	cu8 bits = next_u8() & 0x0F;
	skip(-1);
	if (bits == ndff::Op::N8) {
		return next_u8() >> 4;
	} else if (bits == ndff::Op::N16) {
		return next_u16() >> 4;
	} else if (bits == ndff::Op::N32_TE) {
		return next_u32() >> 4;
	} else if (bits == ndff::Op::N64) {
		skip(1); // because it's not inline
		return next_u64();
	} else {
		mtl_warn();
		return 0;
	}
}

void ByteArray::alloc(const isize n)
{
// Usage:
// ByteArray ba;
// ba.alloc(vallen);
// vallen = lgetxattr(full_path.constData(), key, ba.data(), ba.size());
	MakeSure(n, ExactSize::Yes);
	size_ = n;
}

void ByteArray::MakeSure(cisize more, const ExactSize es)
{
	if (heap_size_ >= at_ + more)
		return;
	
	heap_size_ += more;
	if (es != ExactSize::Yes)
		heap_size_ *= 1.3;
	
	char *p = new char[heap_size_];
	if (data_ != nullptr)
	{
		memcpy(p, data_, size_);
		delete[] data_;
	}
	
	data_ = p;
}

void ByteArray::next(char *p, const isize sz) {
	memcpy(p, data_ + at_, sz);
	at_ += sz;
}

bool ByteArray::next_ba(ByteArray &output, ci64 len)
{
	output.add(data_ + at_, len);
	at_ += len;
	
	return (at_ <= size_);
}

QString ByteArray::NextStringUtf8(ci64 len)
{
	auto s = QString::fromUtf8(data_ + at_, len);
	at_ += len;
	if (at_ > size_)
		mtl_trace();
	
	return s;
}

i8 ByteArray::next_i8() {
	i8 n;
	next(reinterpret_cast<char*>(&n), sizeof n);
	return n;
}

u8 ByteArray::next_u8() {
	u8 n;
	next(reinterpret_cast<char*>(&n), sizeof n);
	return n;
}

i16 ByteArray::next_i16() {
	i16 n;
	next(reinterpret_cast<char*>(&n), sizeof n);
	return n;
}

u16 ByteArray::next_u16() {
	u16 n;
	next(reinterpret_cast<char*>(&n), sizeof n);
	return n;
}

i32 ByteArray::next_i32() {
	i32 n;
	next(reinterpret_cast<char*>(&n), sizeof n);
	return n;
}

u32 ByteArray::next_u32() {
	u32 n;
	next(reinterpret_cast<char*>(&n), sizeof n);
	return n;
}

i64 ByteArray::next_i64() { 
	i64 n;
	next(reinterpret_cast<char*>(&n), sizeof n);
	return n;
}

u64 ByteArray::next_u64() {
	u64 n;
	next(reinterpret_cast<char*>(&n), sizeof n);
	return n;
}

f32 ByteArray::next_f32()
{
	f32 n;
	next(reinterpret_cast<char*>(&n), sizeof n);
	return n;
}

f64 ByteArray::next_f64()
{
	f64 n;
	next(reinterpret_cast<char*>(&n), sizeof n);
	return n;
}

QString ByteArray::next_string(const Pack p)
{
	if (p == Pack::Standard)
	{
		ci32 size = next_i32();
		auto s = QString::fromLocal8Bit(data_ + at_, size);
		at_ += size;
		return s;
	}
	
	cu64 len = next_str_size();
	auto s = QString::fromUtf8(data_ + at_, len);
	at_ += len;
	
	return s;
}

void ByteArray::SetUtf8(QStringView s, const ods::Clear c)
{
	if (c == ods::Clear::Yes)
		Clear();
	QByteArray ba = s.toUtf8();
	add(ba);
	to(0);
}

void ByteArray::TakeOver(char *buf, ci64 heap_size, ci64 size)
{
	Clear();
	data_ = buf;
	heap_size_ = heap_size;
	size_ = size;
}

}
