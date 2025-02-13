#include "ods.hh"

#include "CellRef.hpp"
#include "err.hpp"
#include "ns.hxx"

#include "Book.hpp"
#include "inst/OfficeSpreadsheet.hpp"
#include "Sheet.hpp"

#include <cmath>
#include <iostream>

namespace ods {

bool ColorsEnabled(bool* enable) {
#ifdef _WIN32
		static bool enabled = false;
#else
		static bool enabled = true;
#endif

	if (enable) {
		enabled = *enable;
	}
	return enabled;
}

const char* color_blue() { return ColorsEnabled() ? "\x1B[34m" : ""; }
const char* color_default() { return ColorsEnabled() ? "\x1B[0m" : ""; }
const char* color_green() { return ColorsEnabled() ? "\x1B[32m" : ""; }
const char* color_red() { return ColorsEnabled() ? "\x1B[0;91m" : ""; }
const char* color_yellow() { return ColorsEnabled() ? "\x1B[93m" : ""; }
const char* color_magenta() { return ColorsEnabled() ? "\x1B[35m" : ""; }
const char* color_blink_start() { return ColorsEnabled() ? "\x1B[5m" : ""; }
const char* color_blink_end() { return ColorsEnabled() ? "\x1B[25m" : ""; }
const char* color_bold_start() { return ColorsEnabled() ? "\x1B[1m" : ""; }
const char* color_bold_end() { return ColorsEnabled() ? "\x1B[0m" : ""; }
const char* color_underline_start() { return ColorsEnabled() ? "\x1B[4m" : ""; }
const char* color_underline_end() { return ColorsEnabled() ? "\x1B[0m" : ""; }

static double dpi = -1.0;

void ApplyBool(const QString &str, ods::Bool &b)
{
	if (str.isEmpty()) {
		b = ods::Bool::None;
	} else {
		QString s = str.toLower();
		if (s == ns::True) {
			b = ods::Bool::True;
		} else if (s == ns::False) {
			b = ods::Bool::False;
		} else {
			b = ods::Bool::None;
		}
	}
}

int ColumnLettersToNumber(QStringView letters)
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

QString ColumnNumberToLetters(const int column)
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

u32 CRC_32b(const char *s, ci64 n)
{
	u32 crc = 0xFFFFFFFF;
	
	for(i64 i = 0; i < n; i++)
	{
		char ch = s[i];
		for(i8 bit = 0; bit < 8; bit++)
		{
			cu32 b = (ch ^ crc) & 1;
			crc >>= 1;
			if (b)
				crc = crc ^ 0xEDB88320;
			ch >>= 1;
		}
	}
	
	return ~crc;
}

CellRef*
CreateCellRef(ods::Sheet *default_sheet, QStringView address, ods::CellRef *first_one)
{
	QStringView sheet_name;
	int dot = -1;
	MTL_CHECK_NULL(ParseTableName(address, sheet_name, &dot));
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
	
	const int start_at = (dot == -1) ? 0 : dot + 1;
	auto cell_name = address.mid(start_at);
	int digit_index = -1;
	const int count = cell_name.size();
	
	for (int i = 0; i < count; i++)
	{
		QChar c = cell_name.at(i);
		digit_index++;
		
		if (c.isDigit()) {
			break;
		}
	}

	MTL_CHECK_NULL(digit_index != -1);

	QStringView letters = cell_name.left(digit_index);
	if (letters.startsWith('$'))
		letters = letters.mid(1);
	if (letters.endsWith('$'))
		letters = letters.left(letters.size() - 1);
	
	const int col = ods::ColumnLettersToNumber(letters);
	QStringView digits = cell_name.right(count - digit_index);
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

double DPI()
{
	return dpi; 
}

void DPI(const double dpi_param)
{
	dpi = dpi_param;
}

int FindEndOfSingleQuotedString(QStringView s)
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

i32 FindIndexThatEndsWith(const QStringList &vec, QStringView s)
{
	ci32 end = vec.size();
	for (i32 i = 0; i < end; i++)
	{
		if (vec[i].endsWith(s))
			return i;
	}
	
	return -1;
}

int FindNonAscii(QStringView s, const int from)
{
	for (int i = from; i < s.size(); i++) {
		QChar c = s.at(i);
		bool is_letter = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
		
		if (!is_letter)
			return i;
	}
	
	return -1;
}

int FindNonWhitespace(QStringView str, const int from)
{
	for (int i = from; i < str.size(); i++) {
		const QChar c = str.at(i);
		
		if (c != ' ' && c != '\t' && c != '\x0B'
			&& c != '\r' && c != '\n' && c != '\f')
			return i;
	}
	
	return -1;
}

QString FontSizeToString(const double size, const style::FontSizeType size_type)
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

bool IsAnyCell(QStringView s)
{
	return (s == ns::kTableCell) || (s == ns::kCoveredTableCell);
}

bool ParseTableName(QStringView address, QStringView &name, int *ret_dot)
{
	MTL_CHECK(!address.isEmpty());
	int dot = address.lastIndexOf('.');
	
	if (dot == -1) {
		if (ret_dot != nullptr)
			*ret_dot = -1;
		return true; // doesn't contain sheet name
	}

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

ods::ValueType
TypeFromString(QStringView value_type)
{
	if (value_type.isEmpty())
		return ods::ValueType::None;
	if (value_type == ns::kDouble)
		return ods::ValueType::Double;
	if (value_type == ns::kString)
		// string support implemented as TextP
		return ods::ValueType::String;
	if (value_type == ns::kCurrency)
		return ods::ValueType::Currency;
	if (value_type == ns::kPercentage)
		return ods::ValueType::Percentage;
	if (value_type == ns::kDate)
		return ods::ValueType::Date;
	if (value_type == ns::kDateTime)
		return ods::ValueType::DateTime;
	if (value_type == ns::kTime)
		return ods::ValueType::Time;
	if (value_type == ns::kBoolean)
		return ods::ValueType::Bool;
	
	mtl_it_happened();
	return ods::ValueType::None;
}

QStringView
TypeToString(const ods::ValueType value_type)
{
	switch (value_type)
	{
	case ods::ValueType::Double: return ns::kDouble;
	case ods::ValueType::String: return ns::kString;
	case ods::ValueType::Currency: return ns::kCurrency;
	case ods::ValueType::Percentage: return ns::kPercentage;
	case ods::ValueType::Date: return ns::kDate;
	case ods::ValueType::DateTime: return ns::kDateTime;
	case ods::ValueType::Time: return ns::kTime;
	case ods::ValueType::Bool: return ns::kBoolean;
	default: mtl_it_happened(); return QString();
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

}


#if defined(_WIN32) 
#define WIN32_NO_STATUS
#include <windows.h>
#undef WIN32_NO_STATUS

#ifdef __MINGW32__
#include <_mingw.h>
#ifdef __MINGW64_VERSION_MAJOR
#include <ntstatus.h>
#else
#include <ddk/ntddk.h>
#endif
#else
#include <ntstatus.h>
#endif

#include <stdio.h>

namespace ods {
	void EnableConsoleColors()
	{
		RTL_OSVERSIONINFOW rtl_os = { sizeof rtl_os, };

		NTSTATUS(NTAPI * RtlGetVersion)
			(RTL_OSVERSIONINFOW * lpVersionInformation) =
			(NTSTATUS(NTAPI*)(RTL_OSVERSIONINFOW*))
			GetProcAddress(GetModuleHandleW(L"ntdll"), "RtlGetVersion");

		if (RtlGetVersion) {
			NTSTATUS ntstatus = RtlGetVersion(&rtl_os);
			if (ntstatus < 0) {
				printf("RtlGetVersion failed, error %lld\n", (i64)ntstatus);
			}

			//printf("RtlGetVersion: %lu.%lu.%lu\n", rtl_os.dwMajorVersion, rtl_os.dwMinorVersion, rtl_os.dwBuildNumber);
		}
		else {
			return;
		}

		if (rtl_os.dwBuildNumber < 10586) {
            mtl_info("Windows >=10 (build number >= 10.0.10586) is needed for console colors.");
			return;
		}

		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		DWORD consoleMode;
		GetConsoleMode(hConsole, &consoleMode);
		consoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

		if (!SetConsoleMode(hConsole, consoleMode))
		{
			auto e = GetLastError();
			std::cout << "error " << e << "\n";
		}
		else {
			bool enable = true;
			ColorsEnabled(&enable);
		}
	}
} // ods::
#endif
