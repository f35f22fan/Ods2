#pragma once

#include <QString>

namespace ods {
namespace formula {

static const auto Prefix = QStringLiteral("of:=");
// table:formula="of:=[.E15]*[.F15]"
// table:formula="of:=MIN([.E5:.E8])"
// table:formula="of:=MAX([.F5:.F8])"
// table:formula="of:=COUNT([.D5:.D11])"
// table:formula="of:=AVERAGE([.C5:.C14])"
// table:formula="of:=SUM([.B5:.B25])"

// =A1*16%
// Displays 16% of the contents of A1.

// =ROUND(A1;1)
// Displays the contents of cell A1 rounded to one decimal place.

// =EFFECTIVE(5%;12)
// Calculates the effective interest for
// 5% annual nominal interest with 12 payments a year.

// =B8-SUM(B10:B14)
// Calculates B8 minus the sum of the cells B10 to B14.

// =SUM(B8;SUM(B10:B14))
// Calculates the sum of cells B10 to B14 and adds the value to B8.
}} // ods::formula::
