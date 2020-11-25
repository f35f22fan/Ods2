#pragma once

#include "decl.hxx"
#include "types.hxx"
#include <QString>

//#define DEBUG_FORMULA_PARSING
//#define DEBUG_FORMULA_EVAL
//#define DEBUG_SUMIF_LIKE_FUNCTIONS
namespace ods {

// This lets the parsing function know it's parsing
// formula function params
const u8 ParsingFunctionParams = 1u << 1;
const u8 ReachedFunctionEnd = 1u << 2;
const u8 ReachedParamSeparator = 1u << 3;


// used by ods::Formula::ParseString()
typedef u8 ParsingSettings;
const u8 TreatRemainderAsString = 1u << 0;

const u32 FlattenOutParamsBit = 1u << 0;
const u32 DefaultFunctionSettings = FlattenOutParamsBit;

enum class RoundType : u8 {
	Ceil,
	Floor,
	Round
};

enum class DMY {
	Day,
	Month,
	Year
};

enum class FunctionId : u16 {
	None,
	And,
	Average,
	Columns,
	Concatenate,
	Count,
	CountA,
	CountBlank,
	CountIf,
	Date,
	Day,
	False,
	If,
	Max,
	Min,
	Mod,
	Month,
	Not,
	Now,
	Or,
	Power,
	Quotient,
	Round,
	RoundUp,
	RoundDown,
	Sum,
	SumIf,
	Product,
	Today,
	True,
	Year,
};

struct FunctionMeta {
	const char *name;
	FunctionId id;
	u32 settings; // possible value: ods::FlattenOutParamsBit
	
	FunctionMeta(const char *n, const FunctionId fi, const u32 bits = ods::DefaultFunctionSettings) {
		name = n;
		id = fi;
		settings = bits;
	}
};

namespace formula {
static const auto Prefix = QStringLiteral("of:=");
} // ods::formula::
} // ods::


