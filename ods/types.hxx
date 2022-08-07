#pragma once

#include <cstdint>
#include <cstddef>

#if defined(_MSC_VER)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#else
#include <unistd.h>
#endif

using u8 = uint8_t;
using i8 = int8_t;
using u16 = uint16_t;
using i16 = int16_t;
using u32 = uint32_t;
using i32 = int32_t;
using u64 = uint64_t;
using i64 = int64_t;
using usize = size_t;
using isize = ssize_t;
using f32 = float;
using f64 = double;
using uchar = unsigned char;

using cu8 = const u8;
using ci8 = const i8;
using cu16 = const u16;
using ci16 = const i16;
using cu32 = const u32;
using ci32 = const i32;
using cu64 = const u64;
using ci64 = const i64;
using cusize = const usize;
using cisize = const isize;
using cint = const int;
using cf32 = const f32;
using cf64 = const f64;
using cbool = const bool;
#define cauto const auto
