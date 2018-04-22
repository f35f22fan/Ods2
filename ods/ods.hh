#pragma once

#include "decl.hxx"
#include "global.hxx"
#include "ods.hxx"
#include "style.hxx"
#include "value.hxx"

namespace ods { // ods::

ODS_API QChar
CharFromOp(const ods::Op);

ODS_API ods::Op // error => ods::Op::NONE
CharToOp(const QChar sign);

//ODS_API ods::CellRef* // error => nullptr
//CreateCellRef(const QStringRef &s, ods::Book *book);

double
DPI();

void
DPI(const double dpi);

//ODS_API ods::Cell* // error => nullptr
//FindCell(ods::CellRef *ref, ods::Cell *source);

ODS_API QString
FontSizeToString(const double size, const style::FontSizeType tp);

ODS_API int
ColumnLettersToNumber(const QStringRef &letters);

ODS_API QString
ColumnNumberToLetters(const int kColumn);

bool
higher_priority(const ods::Op op, const Op than_op);

int
priority(const Op op);

ODS_API value::Type
TypeFromString(const QString &ods_type);

ODS_API const char* // nullptr if not found
TypeToString(const value::Type value_type);

} // ods::