#pragma once

#include "global.hxx"
#include <QtGlobal>
#include <QString>

namespace ods { // ods::

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

enum class AddMode: uint8_t
{
	None,
	Insert,
	Replace,
};

enum class ReplaceExisting: uint8_t
{
	Yes,
	No
};

enum class AddIfNeeded: uint8_t
{
	Yes,
	No
};

enum class Place : uint8_t
{
	ContentFile,
	StylesFile,
	None
};

enum class Grouping : quint8
{
	Open,
	Close
};

enum class Op : quint8
{
	None,
	Add,
	Subtract,
	Multiply,
	Divide,
};

} // ods::
