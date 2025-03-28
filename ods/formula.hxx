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

enum class HMS: i8 {
	Hour,
	Minute,
	Second
};

enum class DMY : u8 {
	Day,
	Month,
	Year
};

enum class ColsOrRows: u8 {
	Columns,
	Rows
};

enum class FunctionId : u16 {
	None,
	Acos,
	Acot,
	Asin,
	Atan,
	Atan2,
	Abs,
	And,
	Average,
	BitAnd,
	BitLShift,
	BitOr,
	BitRShift,
	BitXor,
	Columns,
	Concatenate,
	Count,
	CountA,
	CountBlank,
	CountIf,
	Cos,
	Cot,
	Date,
	Day,
	False,
	Formula,
	Hour,
	If,
	Indirect,
	Match,
	Max,
	Min,
	Minute,
	Mod,
	Month,
	Not,
	Now,
	Offset,
	Or,
	PI,
	Power,
	Quotient,
	Rand,
	RandBetween,
	Round,
	RoundUp,
	RoundDown,
	Rows,
	Second,
	Sin,
	Sum,
	SumIf,
	Product,
	Tan,
	Text,
	Time,
	TimeValue,
	Today,
	True,
	Year,
};

struct FunctionMeta {
	QString name;
	FunctionId id = FunctionId::None;
	u32 settings = 0; // possible value: ods::FlattenOutParamsBit
	
	FunctionMeta(const char *n, const FunctionId fi, cu32 bits = ods::DefaultFunctionSettings) {
		name = QLatin1String(n);
		id = fi;
		settings = bits;
	}
};

namespace formula {
static const auto Prefix = QStringLiteral("of:=");
} // ods::formula::
} // ods::


