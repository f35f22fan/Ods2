#pragma once

#include "../global.hxx"
#include "../types.hxx"

namespace ods::ndff {

const u8 MagicNumber[4] = {0xEC, 0x30, 0xA4, 0x34};

ci64 ReservedFeiTableValue = -2;
ci64 EndOfFeiTable = -1;

bool CheckMagicNumber(char *data);

// The fields of this enum with an underline have double and
// unrelated usage. For example S4_PS can be used as an S4 size
// for strings or as a Property Start - the exact meaning is
// determined by context.
enum Op: u8 { // Only up to 4 bits can be used!
	F64 = 0, // separate to be able to store an f64 with no loss
	S8 = 1, // string inline (0 - 15)
	S16 = 2, // string inline (0 - 4095)
	S32_PS = 4, // PS=PropertyStart //string inline (0 – 268435455)
	S64 = 3, // string inline (60 bits for size)
	N8 = 5, // inline (0 - 15)
	N16 = 6, // inline (0 - 4095)
	N32_TE = 7, // inline (0 - 268435455) TE=TagEnd "/>"
	N64 = 8, // separate, to be able to store max value
	TS = 9, // TagStart "<"
	TCF_CMS = 10, // TagContentsFollow ">", CMS=Compressed
	SCT = 11, // SeparateClosingTag "</tag name>"
	FS8 = 12, // FreeSpace
	FS64 = 13,
	JB64 = 14, // JumpBy8 is unsigned
    None = 15,
};

inline bool is_text(const Op op) { return op >= Op::S8 && op <= Op::S64; }

enum MainHeaderPlaces: i16 {
	magic_num = 0,
	info = 4,
	maj_version = 5,
	min_version = 6,
	namespaces_loc = 8,
	dictionary_loc = 16,
	top_files_loc = 24,
	doc_type_len = 32,
	doc_type = 33,
	MinSize = 34
};

}
