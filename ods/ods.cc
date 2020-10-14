#include "ods.hh"

#include "err.hpp"
#include "ns.hxx"
#include "str.hxx"

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

QChar
CharFromOp(const ods::Op op)
{
	switch (op)
	{
	case ods::Op::Multiply:	return '*';
	case ods::Op::Divide:	return '/';
	case ods::Op::Add:		return '+';
	case ods::Op::Subtract:	return '-';
	default: it_happened(); return '?';
	}
}

ods::Op
CharToOp(const QChar c)
{
	if (c == '*')
		return Op::Multiply;
	if (c == '/')
		return Op::Divide;
	if (c == '+')
		return Op::Add;
	if (c == '-')
		return Op::Subtract;
	
	return Op::None;
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


/*
ods::Cell*
FindCell(ods::CellRef *ref, ods::Cell *source)
{
	ods::Sheet *sheet = ref->sheet();
	
	if (sheet == nullptr)
		sheet = source->row()->sheet();
	
	auto *row = sheet->row(ref->row());
	
	if (row == nullptr)
	{
		mtl_line("No such row: %d , row count: %d", ref->row(),
			sheet->CalcRowCount());
		return nullptr;
	}
	
	auto *cell = row->cell(ref->col());
	
	if (cell == nullptr)
	{
		mtl_line("No such cell: %d, row: %d, col count: %d",
			ref->col(),	ref->row(), row->column_count());
		return nullptr;
	}
	
	return cell;
} */

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

/*
ods::CellRef*
CreateCellRef(const QStringRef &full_cell_name, ods::Book *book)
{
	int index = full_cell_name.indexOf('.');
	// example: table:formula="of:=[.A1]+[Sheet2.A1]"
	QStringRef cell_name = full_cell_name.right(full_cell_name.size() - 1 - index);
	const bool current_sheet = index == 0;
	QString sheet_name;
	auto *cell_ref = new ods::CellRef();

	if (!current_sheet)
	{
		sheet_name = full_cell_name.left(index).toString();
		cell_ref->sheet_set(book->sheet(sheet_name));
	} else {
		sheet_name = QLatin1String(".");
	}

	const int count = cell_name.size();
	
	for (int i = 0; i < count; i++)
	{
		QChar c = cell_name.at(i);

		if (!c.isDigit())
			continue;
		
		QStringRef letters = cell_name.left(i);
		cell_ref->col_set(ods::ColumnLettersToNumber(letters));
		QStringRef digits = cell_name.right(count - i);
		bool ok;
		cell_ref->row_set(digits.toInt(&ok));

		if (ok)
		{
			cell_ref->row_set(cell_ref->row() - 1);
			break;
		} else {
			delete cell_ref;
			return nullptr;
		}
	}
	
	return cell_ref;
} */

bool
higher_priority(const ods::Op op, const ods::Op than_op)
{
	return priority(op) > priority(than_op);
}

int
priority(const Op op)
{
	switch(op)
	{
	case Op::None: return 0; // lowest priority, not a bug
	case Op::Add: return 1;
	case Op::Subtract: return 1;
	case Op::Multiply: return 2;
	case Op::Divide: return 2;
	}
	
	it_happened();
	return -1;
}

ods::value::Type
TypeFromString(const QString &value_type)
{
	if (value_type.isEmpty())
		return ods::value::Type::None;
	if (value_type == ods::ns::kDouble)
		return ods::value::Type::Double;
	if (value_type == ods::ns::kString)
		// string support implemented as TextP
		return ods::value::Type::String;
	if (value_type == ods::ns::kCurrency)
		return ods::value::Type::Currency;
	if (value_type == ods::ns::kPercentage)
		return ods::value::Type::Percentage;
	if (value_type == ods::ns::kDate)
		return ods::value::Type::Date;
	if (value_type == ods::ns::kTime)
		return ods::value::Type::Duration;
	if (value_type == ods::ns::kBoolean)
		return ods::value::Type::Bool;
	
	it_happened();
	return ods::value::Type::None;
}

const char*
TypeToString(const ods::value::Type value_type)
{
	switch (value_type)
	{
	case ods::value::Type::Double: return ods::ns::kDouble;
	case ods::value::Type::String: return ods::ns::kString;
	case ods::value::Type::Currency: return ods::ns::kCurrency;
	case ods::value::Type::Percentage: return ods::ns::kPercentage;
	case ods::value::Type::Date: return ods::ns::kDate;
	case ods::value::Type::Duration: return ods::ns::kTime;
	case ods::value::Type::Bool: return ods::ns::kBoolean;
	case ods::value::Type::None: return "[not set]";
	default: it_happened(); return "";
	}
}

} // ods::
