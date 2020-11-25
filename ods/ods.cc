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

static double dpi = -1.0;

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

bool
ParseTableName(const QStringRef &address, QStringRef &name, int *ret_dot)
{
	CHECK_TRUE((!address.isEmpty()));
	int dot = address.lastIndexOf('.');
	CHECK_TRUE((dot != -1));

	auto sheet_name = address.left(dot);
	
	if (sheet_name.startsWith('$'))
		sheet_name = sheet_name.mid(1);
	
	// Must remove starting and ending single quote
	// if it's a single quoted string
	const QChar SingleQuote('\'');
	if (sheet_name.startsWith(SingleQuote)) {
		sheet_name = sheet_name.mid(1, sheet_name.size() - 2);
	}
	
	if (ret_dot != nullptr)
		*ret_dot = dot;
	
	name = sheet_name;
	return true;
}


CellRef*
CreateCellRef(ods::Sheet *default_sheet, QStringRef address, ods::CellRef *first_one)
{
	QStringRef sheet_name;
	int dot;
	CHECK_TRUE_NULL(ParseTableName(address, sheet_name, &dot));
	auto *book = default_sheet->book();
	auto *sp = book->spreadsheet();
	auto *sheet = sp->GetSheet(sheet_name);
	
	if (sheet == nullptr) {
//		auto ba = sheet_name.toLocal8Bit();
//		mtl_warn("Couldn't find sheet by name: \"%s\"", ba.data());
		// it means the address didn't have a sheet name,
		// e.g. ".A2" which means one should use the "current" sheet.
		if ((first_one != nullptr) && (first_one->sheet() != nullptr)) {
			sheet = first_one->sheet();
		} else {
			sheet = default_sheet;
		}
	}
	
	auto cell_name = address.mid(dot + 1);
	int index = -1;
	const int count = cell_name.size();
	bool found = false;
	
	for (int i = 0; i < count; i++)
	{
		QChar c = cell_name.at(i);
		index++;
		
		if (c.isDigit()) {
			found = true;
			break;
		}
	}
	
	CHECK_TRUE_NULL(found);
	
	QStringRef letters = cell_name.left(index);
	if (letters.startsWith('$'))
		letters = letters.mid(1);
	if (letters.endsWith('$'))
		letters = letters.left(letters.size() - 1);
	
	const int col = ods::ColumnLettersToNumber(letters);
	QStringRef digits = cell_name.right(count - index);
//	mtl_printq2("letters: ", letters.toString());
//	mtl_printq2("digits: ", digits.toString());
//	mtl_printq2("address: ", address.toString());
	bool ok;
	// "- 1" because e.g. "1" in [.B1] is row 0
	const int row = digits.toInt(&ok) - 1;
	
	if (ok)
		return CellRef::New(sheet, row, col);
	
	mtl_trace();
	return nullptr;
}

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
FindEndOfSingleQuotedString(const QStringRef &s)
{
	const QChar SingleQuote('\'');
	const int start_at = 1; // skip first single quote
	
	for (int i = start_at; i < s.size(); i++) {
		QChar c = s.at(i);
		
		if (c == SingleQuote) {
			if (i == start_at)
				return i; // empty string
			// else check if it's not preceded by a backslash
			if (s.at(i - 1) != '\\') {
				return i;
			}
		}
	}
	
	return -1;
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
	if (value_type == ods::ns::kDateTime)
		return ods::ValueType::DateTime;
	if (value_type == ods::ns::kTime)
		return ods::ValueType::Time;
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
	case ods::ValueType::DateTime: return ods::ns::kDateTime;
	case ods::ValueType::Time: return ods::ns::kTime;
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
