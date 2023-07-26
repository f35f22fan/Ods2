#pragma once

#include <ods/decl.hxx>
#include <ods/HAlign.hpp>
#include <ods/VAlign.hpp>
#include <ods/inst/decl.hxx>

#include <QString>

namespace util { // util::

QString FindFile(const QString &file_name);
void PrintBorder(ods::Cell *cell, const int row, const int col_index);
void PrintPercentage(ods::Cell *cell);
void PrintWidth(ods::inst::TableTableColumn *col);
int ReadFile();
void Save(ods::Book *book);

} // util::
