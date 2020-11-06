#include "ods.hh"

#include "CellRef.hpp"
#include "err.hpp"
#include "ns.hxx"
#include "str.hxx"

#include "Book.hpp"
#include "inst/OfficeSpreadsheet.hpp"
#include "Sheet.hpp"

#include <cmath>

namespace ods { // ods::

void
ApplyBool(const QString &str, ods::Bool &b)
{
	if (str.isEmpty()) {
		b = ods::Bool::None;
	} else {
		QString s = str.toLower();
		if (s == ods::str::True) {
			b = ods::Bool::True;
		} else if (s == ods::str::False) {
			b = ods::Bool::False;
		} else {
			b = ods::Bool::None;
		}
	}
}

static double dpi = -1.0;

double
DPI()
{
	return dpi; 
}

void
DPI(const double dpi_param)
{
	dpi = dpi_param;
}

int
FindNonAscii(const QStringRef &s, const int from)
{
	for (int i = from; i < s.size(); i++) {
		QChar c = s.at(i);
		bool is_letter = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
		
		if (!is_letter)
			return i;
	}
	
	return -1;
}

int
FindNonWhitespace(const QStringRef &str, const int from)
{
	for (int i = from; i < str.size(); i++) {
		const QChar c = str.at(i);
		
		if (c != ' ' && c != '\t' && c != '\x0B'
			&& c != '\r' && c != '\n' && c != '\f')
			return i;
	}
	
	return -1;
}

QString
FontSizeToString(const double size, const style::FontSizeType size_type)
{
	QString str = QString::number(size);
	
	if (size_type == style::FontSizeType::Pt)
		str += QLatin1String("pt");
	else if (size_type == style::FontSizeType::Cm)
		str += QLatin1String("cm");
	else if (size_type == style::FontSizeType::In)
		str += QLatin1String("in");
	
	return str;
}

int
ColumnLettersToNumber(const QStringRef &letters)
{
	const auto char_A_code = QChar('A').unicode();
	int col = 0;
	const int kCount = letters.size();
	
	for (int j = 0; j < kCount; j++)
	{
		const int diff = letters.at(j).unicode() - char_A_code;
		
		if (kCount - 1 == j)
			col += diff;
		else
			col += pow(26, kCount - j - 1) * (diff + 1);
	}
	
	return col;
}

QString
ColumnNumberToLetters(const int column)
{
	if (column < 0)
		return QString();
	
	int digit_count = 1;
	int offset = 0;
	
	// 26 => alphabet size
	for (int top = 26; column >= top + offset; top *= 26)
	{
		offset += top;
		++digit_count;
	}
	
	QString ret;
	int col = column - offset;
	const auto kCharACode = QChar('A').unicode();
	
	while (digit_count > 0)
	{
		QChar c(kCharACode + (col % 26));
		ret.prepend(c);
		--digit_count;
		col /= 26;
	}
	
	return ret;
}

CellRef*
CreateCellRef(ods::Sheet *default_sheet, QStringRef address)
{
	if (address.isEmpty()) {
		mtl_trace();
		return nullptr;
	}
	
	int dot = address.indexOf('.');
	
	if (dot == -1) {
		mtl_trace();
		return nullptr;
	}
	
	auto sheet_name = address.left(dot);
	
	if (sheet_name.startsWith('$'))
	{
		sheet_name = sheet_name.mid(1);
		mtl_warn();
	}
	
	auto *book = default_sheet->book();
	auto *sp = book->spreadsheet();
	auto *sheet = sp->GetSheet(sheet_name);
	
	if (sheet == nullptr) {
		// it means the address didn't have a sheet name,
		// e.g. ".A2" which means one should use the "current" sheet.
		sheet = default_sheet;
	}
	
	auto cell_name = address.mid(dot + 1);
	int index = -1;
	const int count = cell_name.size();
	bool found = false;
	
	for (int i = 0; i < count; i++)
	{
		QChar c = cell_name.at(i);
		index++;
		
		if (c.isDigit())
		{
			found = true;
			break;
		}
	}
	
	if (!found) {
		mtl_trace();
		return nullptr;
	}
	
	auto letters = cell_name.left(index);
	const int col = ods::ColumnLettersToNumber(letters);
	auto digits = cell_name.right(count - index);
	bool ok;
	const int row = digits.toInt(&ok);
	
	if (ok)
		return CellRef::New(sheet, row, col);
	
	mtl_trace();
	return nullptr;
}

ods::ValueType
TypeFromString(const QString &value_type)
{
	if (value_type.isEmpty())
		return ods::ValueType::None;
	if (value_type == ods::ns::kDouble)
		return ods::ValueType::Double;
	if (value_type == ods::ns::kString)
		// string support implemented as TextP
		return ods::ValueType::String;
	if (value_type == ods::ns::kCurrency)
		return ods::ValueType::Currency;
	if (value_type == ods::ns::kPercentage)
		return ods::ValueType::Percentage;
	if (value_type == ods::ns::kDate)
		return ods::ValueType::Date;
	if (value_type == ods::ns::kTime)
		return ods::ValueType::Duration;
	if (value_type == ods::ns::kBoolean)
		return ods::ValueType::Bool;
	
	it_happened();
	return ods::ValueType::None;
}

const char*
TypeToString(const ods::ValueType value_type)
{
	switch (value_type)
	{
	case ods::ValueType::Double: return ods::ns::kDouble;
	case ods::ValueType::String: return ods::ns::kString;
	case ods::ValueType::Currency: return ods::ns::kCurrency;
	case ods::ValueType::Percentage: return ods::ns::kPercentage;
	case ods::ValueType::Date: return ods::ns::kDate;
	case ods::ValueType::Duration: return ods::ns::kTime;
	case ods::ValueType::Bool: return ods::ns::kBoolean;
	case ods::ValueType::None: return "[not set]";
	default: it_happened(); return "";
	}
}


const QString OpenStr = QLatin1String("(");
const QString CloseStr = QLatin1String(")");

QString
ToString(const Brace p) {
	
	if (p == Brace::Open)
		return OpenStr;
	if (p == Brace::Close)
		return CloseStr;
	
	mtl_trace();
	return QString();
}

Brace
FromString(const QString &s) {
	if (s == OpenStr)
		return Brace::Open;
	if (s == CloseStr)
		return Brace::Close;
	
	mtl_trace();
	return Brace::None;
}

} // ods::
