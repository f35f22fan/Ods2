#pragma once

#include "global.hxx"
#include "types.hxx"
#include <QtGlobal>
#include <QString>

namespace ods {

enum class Dir: i8 {
	Yes,
	No
};

enum class CloneChildren: i8 {
	Yes,
	No
};

enum class EnableNdff: i8 {
	Yes,
	No
};

enum class Compression: u8 {
	None = 0,
	ZSTD = 1,
};

//using CCompression = const Compression;

inline u64 operator | (cu64 n, const Compression c) {
	return u64(c) | n;
}

inline i64 operator | (ci64 n, const Compression c) {
	return i64(c) | n;
}

inline u32 operator | (cu32 n, const Compression c) {
	return u32(c) | n;
}

inline i32 operator | (ci32 n, const Compression c) {
	return i32(c) | n;
}

enum class DocumentFormat: i8 {
	Text,
	Binary
};

enum class Clear: i8 {
	Yes,
	No
};

enum class ClearTheRest: i8 {
	Yes,
	No
};

enum class DevMode: i8 {
	Yes,
	No
};

enum class Owns: i8 {
	Yes,
	No
};

enum class Recursively: i8 {
	Yes,
	No
};

enum class TakeOwnership: i8 {
	Yes,
	No
};

enum class From: i8 {
	Start,
	CurrentPosition
};

enum class ExactSize: i8 {
	Yes,
	No
};

enum class ToStringArgs: u8 {
	None,
	IncludeQuotMarks
};

enum class ValueType: u8
{
	Bool = 1,
	Currency,
	Date,
	DateTime,
	Double,
	Time,
	None, // default
	Percentage,
	String, // string support implemented as TextP
};

enum class Brace: u8 {
	None,
	Open,
	Close
};

enum class Bool: u8 {
	None,
	True,
	False,
};

class Ns;
namespace inst { // ods::inst::
class Abstract;
} // ods::inst::

namespace id { // ods::id::
typedef void (*func)(Ns *ns, inst::Abstract *a);
} // ods::id::

static const auto kFormulaPrefix = QStringLiteral("of:=");
const double kInchesInAPoint = 0.01388888889;
const double kPointsInAnInch = 72.0;
const double kCmInAPoint = 0.0352777778;
const double kPointsInACm = 28.3464567;
const double kInchesInACm = 0.393701;
const double kCmInAnInch = 2.54;

enum class PrintText: i8 {
	Yes,
	No
};

enum class UseNDFF: i8 {
	Yes,
	No
};

enum class AddIfNeeded: i8
{
	Yes,
	No
};

enum class CreateIfNeeded: i8
{
	Yes,
	No
};

enum class Place: u8
{
	ContentFile,
	StylesFile,
	None
};


enum class FormError: i16 {
// https://wiki.openoffice.org/wiki/Calc_Error_Codes
	None				= 0,
	
	//==> LibreOffice
	WrongArgType		= 504,
	MissingOp			= 509,
	MissingVar			= 510,
	Value				= 519,
	CircularReference	= 522,
	Ref					= 524,
	Name				= 525,
	Div0				= 532,
	//<== LibreOffice
	
	//==> Custom ones (mine)
	Num					= 5000,
	NotImplemented		= 5001,
	Other				= INT16_MAX,
};

} // ods::
