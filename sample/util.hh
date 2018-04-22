#pragma once

#include <ods/decl.hxx>
#include <ods/HAlign.hpp>
#include <ods/VAlign.hpp>
#include <ods/inst/decl.hxx>

#include <QString>

namespace util { // util::

QString
FindFile(const QString &file_name);

void
PrintBorder(ods::Cell *cell, const int row, const int col_index);

void
PrintPercentage(ods::Cell *cell);

void
PrintWidth(ods::inst::TableTableColumn *col);

int
ReadFile();

void
Save(ods::Book *book);

void
SetHAlignment(ods::inst::StyleStyle *style, const ods::halign::Value a);

void
SetPercentage(ods::inst::StyleStyle *style, const int min_integer_digits,
	const int min_decimal_places);

void
SetVAlignment(ods::inst::StyleStyle *style, const ods::valign::Value a);

template <class A_Type> class AutoDelete {
public:
	AutoDelete(A_Type x) : x_(x) {}
	virtual ~AutoDelete() { delete x_; }
	
private:
	A_Type x_ = nullptr;
};

} // util::
/*
AutoDelete::AutoDelete(ods::Book *book) : book_(book)
{}

AutoDelete::~AutoDelete()
{
	delete book_;
}
*/