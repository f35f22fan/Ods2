#pragma once

#include "global.hxx"
#include "types.hxx"
#include <QtGlobal>
#include <QString>

namespace ods { // ods::

enum class Brace : u8 {
	None,
	Open,
	Close
};

enum class Bool : u8 {
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

} // ods::
