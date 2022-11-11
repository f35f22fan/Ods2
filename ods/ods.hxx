#pragma once

#include "global.hxx"
#include "types.hxx"
#include <QtGlobal>
#include <QString>

namespace ods {

enum class ODS_API DocumentFormat: i8 {
	Text,
	Binary
};

enum class ODS_API ClearTheRest: i8 {
	Yes,
	No
};

enum class ODS_API DevMode: i8 {
	Yes,
	No
};

enum class ODS_API Owns: i8 {
	Yes,
	No
};

enum class ODS_API Recursively: i8 {
	Yes,
	No
};

enum class ODS_API TakeOwnership: i8 {
	Yes,
	No
};

enum class ODS_API From: i8 {
	Start,
	CurrentPosition
};

enum class ODS_API ExactSize: i8 {
	Yes,
	No
};

enum class ODS_API ToStringArgs: u8 {
	None,
	IncludeQuotMarks
};

enum class ODS_API ValueType: u8
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

enum class ODS_API Brace : u8 {
	None,
	Open,
	Close
};

enum class ODS_API Bool : u8 {
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

enum class ODS_API UseNDFF: i8 {
	Yes,
	No
};

enum class ODS_API AddIfNeeded: i8
{
	Yes,
	No
};

enum class ODS_API CreateIfNeeded: i8
{
	Yes,
	No
};

enum class ODS_API Place : u8
{
	ContentFile,
	StylesFile,
	None
};


enum class ODS_API FormError: i16 {
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
