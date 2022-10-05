#pragma once

#include "AutoDelete.hh"
#include "decl.hxx"
#include "global.hxx"
#include "ods.hxx"
#include "op.hh"
#include "style.hxx"

namespace ods { // ods::

u32 CRC_32b(const char *s, ci64 count);

i32 FindIndexThatEndsWith(const QStringList &vec, QStringView s);

ODS_API QString ToString(const Brace p);
ODS_API Brace FromString(const QString &s);

ODS_API void ApplyBool(const QString &str, ods::Bool &b);

ODS_API CellRef *CreateCellRef(Sheet *default_sheet, QStringView address,
	CellRef *first_one = nullptr);

ODS_API double DPI();

ODS_API void DPI(const double dpi);

/** Returns the index of the last single quote, not past it. **/
int FindEndOfSingleQuotedString(QStringView s);

ODS_API int FindNonAscii(QStringView s, const int from = 0);

// error = -1
ODS_API int FindNonWhitespace(QStringView str, const int from = 0);

ODS_API QString FontSizeToString(const double size, const style::FontSizeType tp);

ODS_API int ColumnLettersToNumber(QStringView letters);

ODS_API QString ColumnNumberToLetters(const int kColumn);

bool ParseTableName(QStringView address, QStringView &name,
	int *ret_dot = nullptr);

ODS_API ValueType TypeFromString(QStringView value_type);

// nullptr if not found
ODS_API QStringView TypeToString(const ValueType value_type);

} // ods::
