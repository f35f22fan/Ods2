#pragma once

#include "../global.hxx"
#include "../types.hxx"

namespace ods::ndff {

const u8 MagicNumber[4] = {0xEC, 0x30, 0xA4, 0x34};

ci64 ReservedFeiTableValue = -2;
ci64 EndOfFeiTable = -1;

bool CheckMagicNumber(char *data);

enum Op: u8 { // Only up to 4 bits can be used!
	F8 = 0, // separate to be able to store an f64 with no loss
	S1 = 1, // string inline (0 - 15)
	S2 = 2, // string inline (0 - 4095)
	S4_PS = 3, // string inline (0 – 268435455), PS=PropertyStart
	S8 = 4, // string inline (60 bits for size)
	U1 = 5, // inline (0 - 15)
	U2 = 6, // inline (0 - 4095)
	U4_TE = 7, // inline (0 - 268435455) TE=TagEnd "/>"
	U8 = 8, // separate, to be able to store max value
	TS = 9, // TagStart "<"
	TCF_CMS = 10, // TagContentsFollow ">", CMS=Compressed
	SCT = 11, // SeparateClosingTag "</tag name>"
	FS1 = 12, // FreeSpace1
	FS8 = 13,
	JB4 = 14, // JumpBy4 is an unsigned number
	JB8 = 15, // JumpBy8 is unsigned
};

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
