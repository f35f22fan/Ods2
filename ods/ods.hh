#pragma once

#include "decl.hxx"
#include "global.hxx"
#include "ods.hxx"
#include "op.hh"
#include "style.hxx"
#include "value.hxx"

namespace ods { // ods::

QString ToString(const Brace p);
Brace FromString(const QString &s);

ODS_API void
ApplyBool(const QString &str, ods::Bool &b);

CellRef*
CreateCellRef(Sheet *default_sheet, QStringRef address);

double
DPI();

void
DPI(const double dpi);

int
FindNonAscii(const QStringRef &s, const int from = 0);

int // error = -1
FindNonWhitespace(const QStringRef &str, const int from = 0);

ODS_API QString
FontSizeToString(const double size, const style::FontSizeType tp);

ODS_API int
ColumnLettersToNumber(const QStringRef &letters);

ODS_API QString
ColumnNumberToLetters(const int kColumn);

ODS_API ValueType
TypeFromString(const QString &ods_type);

ODS_API const char* // nullptr if not found
TypeToString(const ValueType value_type);

} // ods::
