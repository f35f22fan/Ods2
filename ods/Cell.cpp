#include "Cell.hpp"

#include "Attr.hpp"
#include "Book.hpp"
#include "Duration.hpp"
#include "Formula.hpp"
#include "Length.hpp"
#include "Ns.hpp"
#include "ns.hxx"
#include "Row.hpp"
#include "Sheet.hpp"
#include "Tag.hpp"

#include "ods.hh"
#include "StringOrInst.hpp"
#include "StringOrTag.hpp"

#include "formula/Value.hpp"

#include "inst/DrawFrame.hpp"
#include "inst/OfficeAutomaticStyles.hpp"
#include "inst/OfficeDocumentContent.hpp"
#include "inst/StyleFontFace.hpp"
#include "inst/StyleStyle.hpp"
#include "inst/StyleTableCellProperties.hpp"
#include "inst/StyleTextProperties.hpp"
#include "inst/TableTableColumn.hpp"
#include "inst/TextP.hpp"

#include <float.h>
#include <QFontMetrics>

namespace ods { // ods::

Cell::Cell(Row *parent, ods::Tag *tag)
: Abstract(parent, parent->ns(), id::TableTableCell),
row_(parent)
{
	Init(tag);
}

Cell::Cell(Row *parent)
: Abstract (parent, parent->ns(), id::TableTableCell),
row_(parent)
{}

Cell::Cell(const Cell &cloner)
: Abstract(cloner)
{}

Cell::~Cell()
{
	ClearValue(true);
}

void
Cell::AppendString(const QString &s)
{
	auto *inst = Get(id::TextP);
	
	if (inst != nullptr)
	{
		auto *textp = (ods::inst::TextP*)inst;
		textp->AppendString(s);
	} else {
		auto *textp = new inst::TextP(this);
		textp->SetFirstString(s);
		Append(textp);
	}
}

void
Cell::ClearValue(const bool delete_data)
{
	if (!is_value_set())
		return;
	
	if (delete_data)
	{
		if (is_double() || is_percentage() || is_currency())
			delete as_double();
		else if (is_date())
			delete as_date();
		else if (is_duration())
			delete as_duration();
		else if (is_boolean())
			delete as_boolean();
	}
	
	value_data_ = nullptr;
	office_value_type_ = ods::value::Type::None;
}

inst::Abstract*
Cell::Clone(inst::Abstract *parent) const
{
	auto *p = new Cell(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->value_data_ = CloneValue();
	p->office_value_type_ = office_value_type_;
	
	p->number_columns_repeated_ = number_columns_repeated_;
	p->number_columns_spanned_ = number_columns_spanned_;
	p->number_rows_spanned_ = number_rows_spanned_;
	
	p->office_currency_ = office_currency_;
	
	return p;
}

void*
Cell::CloneValue() const
{
	if (!is_value_set())
		return nullptr;
	
	if (is_double() || is_percentage() || is_currency())
		return new double(*as_double());
	else if (is_date())
		return new QDateTime(*as_date());
	else if (is_duration())
		return new ods::Duration(*as_duration());
	else if (is_boolean())
		return new bool(*as_boolean());
}

QString*
Cell::GetFirstString() const
{
	auto *inst = Get(id::TextP);
	
	if (inst == nullptr)
		return nullptr;
	
	auto *tp = (inst::TextP*)inst;
	return tp->GetFirstString();
}

inst::StyleStyle*
Cell::GetStyle() const
{
	return Get(table_style_name_);
}

void
Cell::Init(ods::Tag *tag)
{
	ReadValue(tag);
	tag->Copy(ns_->table(), ods::ns::kNumberColumnsRepeated, number_columns_repeated_);
	tag->Copy(ns_->table(), ods::ns::kNumberColumnsSpanned, number_columns_spanned_);
	tag->Copy(ns_->table(), ods::ns::kNumberRowsSpanned, number_rows_spanned_);
	tag->Copy(ns_->table(), ods::ns::kStyleName, table_style_name_);
	
	QString str;
	tag->Copy(ns_->office(), ods::ns::kValueType, str);
	
	if (!str.isEmpty())
		office_value_type_ = ods::TypeFromString(str);
	
	tag->Copy(ns_->office(), ods::ns::kCurrency, office_currency_);
	
	tag->Copy(ns_->table(), ods::ns::kFormula, str);
	formula_ = ods::Formula::FromString(str, this);
	
	Scan(tag);
}

inst::DrawFrame*
Cell::NewDrawFrame()
{
	auto *p = new inst::DrawFrame(this);
	Append(p);
	return p;
}

ods::Formula*
Cell::NewFormula()
{
	delete formula_;
	formula_ = new Formula(this);
	return formula_;
}

inst::StyleStyle*
Cell::NewStyle()
{
	auto *style = book_->NewCellStyle();
	SetStyle(style);
	return style;
}

QString
Cell::QueryAddress() const
{
	QString s = ods::ColumnNumberToLetters(QueryStart());
	s.append(QString::number(row_->QueryStart()));
	return s;
}

Length*
Cell::QueryDesiredHeight() const
{
	inst::StyleStyle *style = GetStyle();
	
	if (style == nullptr)
		return nullptr;
	
	auto *tcp = (inst::StyleTableCellProperties*)
		style->Get(id::StyleTableCellProperties);
	
	auto *stp = (inst::StyleTextProperties*)
		style->Get(id::StyleTextProperties);
	
	if (stp == nullptr)
	{
		mtl_warn();
		return nullptr;
	}
	
	Length *font_size = stp->font_size();
	
	if (font_size == nullptr)
	{
		mtl_warn();
		return nullptr;
	}
	
	const bool wrap = (tcp != nullptr) && tcp->wrap_option();
	
	if (!wrap)
		return font_size->Clone();
	
	auto *table_column = row_->sheet()->GetColumn(QueryStart());
	
	if (table_column == nullptr)
	{
		mtl_warn();
		return nullptr;
	}
	
	Length *col_width = table_column->QueryColumnWidth();
	
	if (col_width == nullptr)
	{
		mtl_warn();
		return nullptr;
	}
	
	QString value;
	
	if (is_string())
	{
		auto *s = GetFirstString();
		
		if (s == nullptr)
		{
			mtl_warn();
			return nullptr;
		}
		
		value = *s;
	} else {
		value = ValueToString();
	}
	
	if (value.isEmpty())
	{
		mtl_warn();
		return font_size->Clone();
	}
	auto ba = value.toLocal8Bit();
	mtl_line("String value: \"%s\"", ba.data());
	
	inst::StyleFontFace *font_face = QueryFontFace(style, table_column);
	
	ba = font_face->font_family().toLocal8Bit();
	auto ba2 = font_size->toString().toLocal8Bit();
	mtl_line("Font is %s, %s, in pt: %f", ba.data(), ba2.data(),
		font_size->toPt());
	
	QFont font(font_face->font_family(), int(font_size->toPt()));
	QFontMetrics fm(font);
	// fm.width(..) returns pixels
	// need to translate to points:
	double width = length::PxToPt(fm.width(value), ods::DPI());
	int line_count = width / col_width->toPt();
	int int_col_width = int(col_width->toPt());
	
	if ((int_col_width != 0) && (int(width) % int_col_width))
		line_count++;
	
	ba = col_width->toString().toLocal8Bit();
	mtl_line("line count: %d, str width pt: %f, col_width pt: %f, as is: %s\n\n",
		line_count, width, col_width->toPt(), ba.data());
	
	return new Length(line_count * font_size->toPt(), Measure::Pt);
}

inst::StyleFontFace*
Cell::QueryFontFace(inst::StyleStyle *cell_style,
	inst::TableTableColumn *table_column) const
{
	if (cell_style == nullptr)
		cell_style = GetStyle();
	
	if (cell_style != nullptr)
	{
		inst::StyleFontFace *ff = cell_style->QueryFontFace();
		
		if (ff != nullptr)
			return ff;
	}
	
	if (table_column == nullptr)
		table_column = row_->sheet()->GetColumn(QueryStart());
	
	if (table_column == nullptr)
		return nullptr;
	
	inst::StyleStyle *default_cell_style = table_column->GetDefaultCellStyle();
	
	if (default_cell_style == nullptr)
		return nullptr;
	
	return default_cell_style->QueryFontFace();
}

int
Cell::QueryStart() const
{
	return row_->QueryCellStart(this);
}

void
Cell::ReadValue(ods::Tag *tag)
{
	auto *ns = tag->ns();
	auto *type_attr = tag->Get(ns->office(), ods::ns::kValueType);

	if (type_attr == nullptr)
	{
		ClearValue(true);
		return;
	}
	
	const QString stype = type_attr->value();
	office_value_type_ = ods::TypeFromString(stype);
	
	if (is_double() || is_currency() || is_percentage())
	{
		auto *value_attr = tag->Get(ns->office(), ods::ns::kValue);
		
		if (value_attr != nullptr)
		{
			double num;
			if (value_attr->ToDouble(num))
			{
				SetValue(new double(num), office_value_type_);
			} else {
				mtl_warn("ToDouble()");
			}
		}
	} else if (is_date()) {
		auto *custom_attr = tag->Get(ns->office(), ods::ns::kDateValue);
		
		if (custom_attr == nullptr)
		{
			mtl_warn("custom_attr == nullptr");
			return;
		}
		
		auto dt = QDateTime::fromString(custom_attr->value(), Qt::ISODate);
		SetValue(new QDateTime(dt), office_value_type_);
	} else if (is_duration()) {
		auto *custom_attr = tag->Get(ns->office(), ods::ns::kTimeValue);

		if (custom_attr == nullptr)
		{
			mtl_warn("custom_attr == nullptr");
			return;
		}
		
		auto *t = new ods::Duration();
		t->Decode(custom_attr->value());
		SetValue(t, office_value_type_);
	} else if (is_boolean()) {
		auto *custom_attr = tag->Get(ns->office(), ods::ns::kBooleanValue);
		
		if (custom_attr != nullptr)
			SetBooleanFromString(custom_attr->value());
	} else if (is_string()) {
		// do nothing
	} else {
		office_value_type_ = ods::value::Type::None;
	}
}

void
Cell::Scan(ods::Tag *scan_tag)
{
	foreach (StringOrTag *x, scan_tag->nodes())
	{
		if (AddText(x))
			continue;
		
		auto *tag = x->as_tag();
		
		if (tag->IsTextP())
		{
			Append(new inst::TextP(this, tag));
		} else if (tag->Is(ns_->draw(), ods::ns::kFrame)) {
			Append(new inst::DrawFrame(this, tag));
		} else {
			Scan(tag);
		}
	}
}

void
Cell::SetBoolean(const bool flag)
{
	ClearValue(true);
	value_data_ = new bool(flag);
	office_value_type_ = ods::value::Type::Bool;
}

void
Cell::SetBooleanFromString(const QString &s)
{
	const bool flag = s.toLower() == QLatin1String("true");
	SetValue(new bool(flag), office_value_type_);
}

void
Cell::SetCurrency(const double d, const QString str)
{
	SetDouble(d);
	office_value_type_ = ods::value::Type::Currency;
	office_currency_ = str;
}

void
Cell::SetDate(const QDateTime *p)
{
	ClearValue(true);
	
	if (p != nullptr)
	{
		office_value_type_ = ods::value::Type::Date;
		value_data_ = new QDateTime(*p);
	}
}

void
Cell::SetDouble(const double d)
{
	ClearValue(true);
	office_value_type_ = ods::value::Type::Double;
	value_data_ = new double();
	*as_double() = d;
}

void
Cell::SetDuration(const ods::Duration *p)
{
	ClearValue(true);
	
	if (p != nullptr)
	{
		office_value_type_ = ods::value::Type::Duration;
		value_data_ = p->Clone();
	}
}

void
Cell::SetFirstString(const QString &s)
{
	auto *inst = Get(id::TextP);
	
	if (inst != nullptr)
	{
		auto *textp = (ods::inst::TextP*) inst;
		textp->SetFirstString(s);
	} else {
		auto *textp = new inst::TextP(this);
		textp->SetFirstString(s);
		Append(textp);
	}
	
	office_value_type_ = ods::value::Type::String;
}

void
Cell::SetFormula(ods::Formula *p)
{
	delete formula_;
	
	if (p == nullptr)
		formula_ = nullptr;
	else
		formula_ = p->Clone();
}

void
Cell::SetPercentage(const double d)
{
	SetDouble(d);
	office_value_type_ = ods::value::Type::Percentage;
}

void
Cell::SetStyle(Abstract *inst)
{
	if (!inst->IsStyle())
	{
		mtl_warn();
		return;
	}
	
	QString *name = nullptr;
	
	if (inst->Is(id::StyleStyle))
		name = ((inst::StyleStyle*)inst)->style_name();
	
	if (name != nullptr)
	{
		table_style_name_ = *name;
	} else {
		table_style_name_.clear();
		mtl_warn();
	}
}

QByteArray
Cell::TypeAndValueString() const
{
	if (is_string())
	{
		QString *s = GetFirstString();
		QString ret("[String] ");
		
		if (s != nullptr)
			ret.append(*s);
		
		return ret.toLocal8Bit();
	}
	
	return QString(QChar('['))
	.append(ods::TypeToString(office_value_type_))
	.append(QLatin1String("] "))
	.append(ValueToString()).toLocal8Bit();
}

QString
Cell::ValueToString() const
{
	const auto empty = QLatin1String();
	
	if (!is_value_set())
		return empty;
	
	if (is_double() || is_percentage() || is_currency())
		return QString::number(*as_double(), 'f', FLT_DIG);
	if (is_date())
		return as_date()->toString(Qt::ISODate);
	if (is_duration())
		return as_duration()->toString();
	if (is_boolean())
		return *as_boolean() ? QLatin1String("true") : QLatin1String("false");
	if (is_string())
		return empty;
	
	it_happened();
	return empty;
}

void
Cell::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->table(), ods::ns::kNumberColumnsRepeated, number_columns_repeated_, 1);
	Write(xml, ns_->table(), ods::ns::kNumberColumnsSpanned, number_columns_spanned_, 1);
	Write(xml, ns_->table(), ods::ns::kNumberRowsSpanned, number_rows_spanned_, 1);
	Write(xml, ns_->table(), ods::ns::kStyleName, table_style_name_);
	
	if (is_value_set())
	{
		auto str = ods::TypeToString(office_value_type_);
		Write(xml, ns_->office(), ods::ns::kValueType, str);
	}
	
	WriteValue(xml);
	WriteNodes(xml);
}

void
Cell::WriteValue(QXmlStreamWriter &xml)
{
	if (is_double())
	{
		Write(xml, ns_->office(), ods::ns::kValue, QString::number(*as_double()));
	} else if (is_currency()) {
		Write(xml, ns_->office(), ods::ns::kValue, QString::number(*as_double()));
		Write(xml, ns_->office(), ods::ns::kCurrency, office_currency_);
	} else if (is_percentage()) {
		Write(xml, ns_->office(), ods::ns::kValue, QString::number(*as_double()));
	} else if (is_date()) {
//<table:table-cell table:style-name="ce5" office:value-type="date"
//		office:date-value="1983-12-30">
//<text:p>30.12.1983</text:p>
//</table:table-cell>
		auto *dt = as_date();
		QString date_value = dt->toString(Qt::ISODate);
		Write(xml, ns_->office(), ods::ns::kDateValue, date_value);
	} else if (is_duration()) {
		auto *dd = as_duration();
		
		if (dd->IsValid())
		{
			QString dur_value = dd->toString();
			Write(xml, ns_->office(), ods::ns::kTimeValue, dur_value);
		}
	} else if (is_boolean()) {
		QString str = *as_boolean() ? QLatin1String("true") : QLatin1String("false");
		Write(xml, ns_->office(), ods::ns::kBooleanValue, str);
	} else if (is_string()) {
		// do nothing
	} else if (!is_value_set()) {
	} else {
		it_happened();
	}
	
	if (formula_ != nullptr)
		Write(xml, ns_->table(), ods::ns::kFormula, formula_->toString());
}

} // ods::
