#include "Cell.hpp"

#include "Attr.hpp"
#include "Book.hpp"
#include "CellRef.hpp"
#include "currency.hh"
#include "Formula.hpp"
#include "Length.hpp"
#include "Ns.hpp"
#include "ns.hxx"
#include "ods.hh"
#include "Row.hpp"
#include "Sheet.hpp"
#include "StringOrInst.hpp"
#include "StringOrTag.hpp"
#include "Tag.hpp"
#include "Time.hpp"

#include "inst/DrawFrame.hpp"
#include "inst/DrawImage.hpp"
#include "inst/NumberCurrencyStyle.hpp"
#include "inst/NumberCurrencySymbol.hpp"
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

#include "ndff/Container.hpp"
#include "ndff/Property.hpp"

namespace ods { // ods::

Cell::Cell(Row *parent, ods::Tag *tag, ndff::Container *cntr)
: Abstract(parent, parent->ns(), id::TableTableCell),
row_(parent)
{
	if (cntr)
		Init(cntr);
	else if (tag)
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
	delete formula_;
	formula_ = nullptr;
}

void
Cell::AppendString(const QString &s)
{
	auto *inst = Get(Id::TextP);
	
	if (inst != nullptr)
	{
		auto *textp = (ods::inst::TextP*)inst;
		textp->AppendString(s);
	} else {
		auto *textp = new inst::TextP(this);
		textp->SetString(s);
		Append(textp, TakeOwnership::Yes);
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
		else if (is_integer())
			delete as_integer();
		else if (is_date_time())
			delete as_date_time();
		else if (is_date())
			delete as_date();
		else if (is_time())
			delete as_time();
		else if (is_boolean())
			delete as_boolean();
	}
	
	value_data_ = nullptr;
	office_value_type_ = ods::ValueType::None;
}

inst::Abstract*
Cell::Clone(inst::Abstract *parent) const
{
	auto *p = new Cell(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->value_data_ = CloneValue();
	p->office_value_type_ = office_value_type_;
	
	p->ncr_ = ncr_;
	p->ncs_ = ncs_;
	p->nrs_ = nrs_;
	
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
		return new QDateTime(QDate(*as_date()).startOfDay());
	else if (is_time())
		return new ods::Time(*as_time());
	else if (is_boolean())
		return new bool(*as_boolean());
	mtl_trace();
	
	return nullptr;
}

inst::StyleStyle*
Cell::FetchStyle()
{
	auto *style = Get(table_style_name_);
	
	if (style == nullptr)
		style = NewStyle();
	
	return style;
}

ods::FormulaNode* Cell::formulaNode()
{
	return ods::FormulaNode::Reference(row_->sheet()->NewReference(this));
}

QString
Cell::FullName() const
{
	if (covered())
		return prefix_->With(ns::kCoveredTableCell);
	
	return prefix_->With(tag_name_);
}

const QString*
Cell::GetCellString() const
{
	auto *inst = Get(Id::TextP);
	
	if (inst == nullptr)
		return nullptr;
	
	auto *tp = (inst::TextP*)inst;
	return tp->GetString();
}

inst::StyleStyle*
Cell::GetStyle(const CreateIfNeeded cin)
{
	auto *style = Get(table_style_name_);
	
	return (cin == CreateIfNeeded::Yes && style == nullptr) ?
		NewStyle() : style;
}

void Cell::Init(ndff::Container *cntr)
{
	using Op = ndff::Op;
	ndff::Property prop;
	NdffAttrs attrs;
	Op op = cntr->Next(prop, Op::TS, &attrs);
	ReadValue(0, &attrs);
	CopyAttrI32(attrs, ns_->table(), ns::kNumberColumnsRepeated, ncr_);
	CopyAttrI32(attrs, ns_->table(), ns::kNumberColumnsSpanned, ncs_);
	CopyAttrI32(attrs, ns_->table(), ns::kNumberRowsSpanned, nrs_);
	CopyAttr(attrs, ns_->table(), ns::kStyleName, table_style_name_);
	
	QString str;
	if (office_value_type_ == ValueType::None)
	{
		CopyAttr(attrs, ns_->office(), ns::kValueType, str);
		if (!str.isEmpty())
			office_value_type_ = ods::TypeFromString(str);
	}
	
	CopyAttr(attrs, ns_->office(), ns::kCurrency, office_currency_);
	CopyAttr(attrs, ns_->table(), ns::kFormula, str);
	formula_ = ods::Formula::FromString(str, this);

	if (op == Op::N32_TE)
		return;

	if (op == Op::TCF_CMS)
		op = cntr->Next(prop, op);

	while (true)
	{
		if (op == Op::TS)
		{
			if (prop.is(ns_->text()))
			{
				if (prop.name == ns::kP)
					Append(new inst::TextP(this, 0, cntr), TakeOwnership::Yes);
			} else if (prop.is(ns_->draw())) {
				if (prop.name == ns::kFrame)
					Append(new inst::DrawFrame(this, 0, cntr), TakeOwnership::Yes);
			}
		} else if (ndff::is_text(op)) {
			Append(cntr->NextString());
		} else {
			break;
		}
		op = cntr->Next(prop, op);
	}

	if (op != Op::SCT)
		mtl_trace("Unexpected op: %d", op);
}

void Cell::Init(ods::Tag *tag)
{
	ReadValue(tag, 0);
	tag->Copy(ns_->table(), ns::kNumberColumnsRepeated, ncr_);
	tag->Copy(ns_->table(), ns::kNumberColumnsSpanned, ncs_);
	tag->Copy(ns_->table(), ns::kNumberRowsSpanned, nrs_);
	tag->Copy(ns_->table(), ns::kStyleName, table_style_name_);
	 // calcext:value-type="time"
	QString str;
	if (office_value_type_ == ValueType::None)
	{
		tag->Copy(ns_->office(), ns::kValueType, str);
		if (!str.isEmpty())
			office_value_type_ = ods::TypeFromString(str);
	}
	
	tag->Copy(ns_->office(), ns::kCurrency, office_currency_);
	tag->Copy(ns_->table(), ns::kFormula, str);
	formula_ = ods::Formula::FromString(str, this);
	
	Scan(tag);
}

void Cell::ListKeywords(inst::Keywords &words_hash, const inst::LimitTo lt)
{
	inst::AddKeywords({tag_name(),
		ns::kNumberColumnsRepeated,
		ns::kNumberColumnsSpanned,
		ns::kNumberRowsSpanned,
		ns::kStyleName,
		ns::kValueType,
		ns::kCurrency,
		ns::kFormula,
		ns::kValue,
		ns::kDateValue,
		ns::kTimeValue,
		ns::kBooleanValue}, words_hash);
}

void Cell::ListUsedNamespaces(inst::NsHash &list)
{
	inst::Add(ns_->table(), list);
	
	if (is_value_set())
	{
		inst::Add(ns_->office(), list);
	}
}

QString Cell::GetAttr(ods::Tag *tag, NdffAttrs *attrs, Prefix *prefix,
	QStringView key)
{
	if (attrs)
	{
		QString ret_val;
		CopyAttr(*attrs, prefix, key, ret_val);
		return ret_val;
	}

	auto *v = tag->GetAttr(prefix, key);
	return v ? v->value() : QString();
}

inst::DrawFrame*
Cell::NewDrawFrame()
{
	auto *p = new inst::DrawFrame(this, 0);
	Append(p, TakeOwnership::Yes);
	return p;
}

inst::DrawImage*
Cell::NewDrawFrame(const QString &full_path, QSize *real_size)
{
	auto *draw_frame = (inst::DrawFrame*) Get(ods::Id::DrawFrame);
	if (!draw_frame)
		draw_frame = NewDrawFrame();
	
	auto *draw_image = (inst::DrawImage*) draw_frame->Get(ods::Id::DrawImage);
	if (!draw_image)
		draw_image = draw_frame->NewDrawImage();
	
	QSize sz;
	if (draw_image->LoadImage(full_path, sz))
	{
		if (real_size)
			*real_size = sz;
		return draw_image;
	}
	
	delete draw_image;
	
	return 0;
}

ods::Formula*
Cell::NewFormula()
{
	delete formula_;
	formula_ = new Formula(this);
	return formula_;
}

ods::CellRef*
Cell::NewRef()
{
	ods::Sheet *sheet = row_->sheet();
	int row_index = sheet->QueryRowStart(row_);
	int col = row_->QueryCellStart(this);
	return CellRef::New(sheet, row_index, col);
}

ods::CellRef*
Cell::NewRef(const i32 r, const i32 c)
{
	ods::Sheet *sheet = row_->sheet();
	MTL_CHECK_ARG(sheet, nullptr);
	int row_index = sheet->QueryRowStart(row_);
	int col_index = row_->QueryCellStart(this);
	int row = row_index + r;
	int col = col_index + c;
	return CellRef::New(sheet, row, col);
}

inst::StyleStyle*
Cell::NewStyle()
{
	auto *style = book_->NewCellStyle();
	SetStyle(style);
	return style;
}

void Cell::number_columns_spanned(const int n)
{
	ncs_ = n;
	
//	if (ncs_ == 1)
//		return;
	
	// Unlikely event:
	// If ncs_ was 3 and was set to 1
	// setting the adjacent 2 cells to non-covered
	// isn't supported, yet.
	
	QVector<Cell*> &cells = row_->cells();
	const int cell_count = cells.size();
	
	for (int i = 0; i < cell_count; i++) {
		Cell *next = cells[i];
		
		if (next == this) {
			row_->MarkCoveredCellsAfter(this, i);
			return;
		}
	}
}

QString Cell::QueryAddress() const
{
	QString s = ods::ColumnNumberToLetters(QueryStart());
	// row + 1 because .ods indexing starts at 1
	// [.B1] means col 1 and row 0
	s.append(QString::number(row_->QueryStart() + 1));
	return s;
}

ods::Currency* Cell::QueryCurrencyObject()
{
	auto *style = GetStyle();
	MTL_CHECK_ARG(style, nullptr);
	auto ncs = style->GetCurrencyStyle();
	MTL_CHECK_ARG(ncs, nullptr);
	auto *cs = (ods::inst::NumberCurrencySymbol*)
		ncs->Get(ods::Id::NumberCurrencySymbol);
	const QString country = cs->country();
	const QString symbol = cs->GetSymbol();
	ods::Currency *c = ods::currency::Query(country, symbol);
	MTL_CHECK_ARG(c, nullptr);
	c->qtty = *as_currency();
	return c;
}

Length* Cell::QueryDesiredHeight() const
{
	inst::StyleStyle *style = GetStyle();
	
	if (style == nullptr)
		return nullptr;
	
	auto *tcp = (inst::StyleTableCellProperties*)
		style->Get(Id::StyleTableCellProperties);
	
	auto *stp = (inst::StyleTextProperties*)
		style->Get(Id::StyleTextProperties);
	
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
		auto *s = GetCellString();
		
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
//	auto ba = value.toLocal8Bit();
//	mtl_line("String value: \"%s\"", ba.data());
	
	inst::StyleFontFace *font_face = QueryFontFace(style, table_column);
	
//	ba = font_face->font_family().toLocal8Bit();
//	auto ba2 = font_size->toString().toLocal8Bit();
//	mtl_line("Font is %s, %s, in pt: %f", ba.data(), ba2.data(),
//		font_size->toPt());
	
	QFont font(font_face->font_family(), int(font_size->toPt()));
	QFontMetrics fm(font);
	// fm.width(..) returns pixels
	// need to translate to points:
	double width = length::PxToPt(fm.horizontalAdvance(value), ods::DPI());
	int line_count = width / col_width->toPt();
	int int_col_width = int(col_width->toPt());
	
	if ((int_col_width != 0) && (int(width) % int_col_width))
		line_count++;
	
//	ba = col_width->toString().toLocal8Bit();
//	mtl_line("line count: %d, str width pt: %f, col_width pt: %f, as is: %s\n\n",
//		line_count, width, col_width->toPt(), ba.data());
	
	return new Length(line_count * font_size->toPt(), Unit::Pt);
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

int Cell::QueryStart() const
{
	return row_->QueryCellStart(this);
}

void Cell::ReadValue(ods::Tag *tag, NdffAttrs *attrs)
{
	QString stype = GetAttr(tag, attrs, ns_->office(), ns::kValueType);
	
	if (stype.isEmpty())
	{
		ClearValue(true);
		return;
	}
	
	office_value_type_ = ods::TypeFromString(stype);
	
	if (is_double() || is_currency() || is_percentage())
	{
		QString attr = GetAttr(tag, attrs, ns_->office(), ns::kValue);
		if (!attr.isEmpty())
		{
			bool ok;
			double num = attr.toDouble(&ok);
			if (ok)
			{
				SetValue(new double(num), office_value_type_);
			} else {
				mtl_warn("ToDouble() failed");
			}
		}
	} else if (is_date()) {
		// office:value-type="date" can be represented as
		// a date or date-time, so check for ":" to guess which one.
		
		QString date_value_str = GetAttr(tag, attrs, ns_->office(), ns::kDateValue);
		//mtl_printq2("date_value_str: ", date_value_str);
		if (date_value_str.indexOf(':') != -1) {
			auto dt = QDateTime::fromString(date_value_str, Qt::ISODate);
			SetDateTime(new QDateTime(dt));
		} else {
			auto dt = QDate::fromString(date_value_str, Qt::ISODate);
			SetDate(new QDate(dt));
		}
	} else if (is_time()) {
		QString attr = GetAttr(tag, attrs, ns_->office(), ns::kTimeValue);
		auto *t = new ods::Time();
		if (!t->Parse(attr)) {
			auto ba = attr.toLocal8Bit();
			mtl_trace("Failed to parse: \"%s\"", ba.data());
			delete t;
		} else {
			SetValue(t, office_value_type_);
		}
	} else if (is_boolean()) {
		QString attr = GetAttr(tag, attrs, ns_->office(), ns::kBooleanValue);
		
		if (!attr.isEmpty())
			SetBooleanFromString(attr);
	} else if (is_string()) {
		// do nothing
	} else {
		office_value_type_ = ods::ValueType::None;
	}
}

void Cell::Scan(ods::Tag *scan_tag)
{
	foreach (StringOrTag *x, scan_tag->nodes())
	{
		if (AddText(x))
			continue;
		
		auto *tag = x->as_tag();
		
		if (tag->IsTextP())
		{
			Append(new inst::TextP(this, tag), TakeOwnership::Yes);
		} else if (tag->Is(ns_->draw(), ns::kFrame)) {
			Append(new inst::DrawFrame(this, tag), TakeOwnership::Yes);
		} else {
			Scan(tag);
		}
	}
}

void Cell::SetBoolean(const bool flag)
{
	ClearValue(true);
	value_data_ = new bool(flag);
	office_value_type_ = ods::ValueType::Bool;
}

void Cell::SetBooleanFromString(QStringView s)
{
	if (s.isEmpty())
		return;
	
	cbool flag = s == QLatin1String("true");
	SetValue(new bool(flag), office_value_type_);
}

void Cell::SetCurrency(const Currency &c)
{
	SetDouble(c.qtty);
	office_value_type_ = ods::ValueType::Currency;
	CurrencyInfo info = ods::currency::info(c.id);
	office_currency_ = info.str;
}

void Cell::SetDate(QDate *p)
{
	ClearValue(true);
	
	if (p != nullptr)
	{
		office_value_type_ = ods::ValueType::Date;
		value_data_ = p;
	}
}

void Cell::SetDateTime(QDateTime *p)
{
	ClearValue(true);
	
	if (p != nullptr) {
		office_value_type_ = ods::ValueType::DateTime;
		value_data_ = p;
	}
}

void Cell::SetDouble(const double d)
{
	ClearValue(true);
	office_value_type_ = ods::ValueType::Double;
	value_data_ = new double(d);
}

void Cell::SetInteger(ci64 n) {
	ClearValue(true);
	office_value_type_ = ods::ValueType::Integer;
	value_data_ = new i64(n);
}

void Cell::SetFirstString(const QString &s, bool change_value_type)
{
	auto *inst = Get(Id::TextP);
	
	if (inst != nullptr)
	{
		auto *textp = (ods::inst::TextP*) inst;
		textp->SetString(s);
	} else {
		auto *textp = new inst::TextP(this);
		textp->SetString(s);
		Append(textp, TakeOwnership::Yes);
	}
	
	if (change_value_type)
		office_value_type_ = ods::ValueType::String;
}

void Cell::SetFormula(ods::Formula *p)
{
	delete formula_;
	formula_ = p;
}

void Cell::SetPercentage(const double d)
{
	SetDouble(d);
	office_value_type_ = ods::ValueType::Percentage;
}

void Cell::SetRowColSpan(int rows, int cols) {
	number_rows_spanned(rows);
	number_columns_spanned(cols);
}

void Cell::SetStyle(Abstract *inst)
{
	if (!inst->IsStyle())
	{
		mtl_warn();
		return;
	}
	
	QString *name = nullptr;
	
	if (inst->Is(Id::StyleStyle))
		name = ((inst::StyleStyle*)inst)->style_name();
	
	if (name != nullptr)
	{
		table_style_name_ = *name;
	} else {
		table_style_name_.clear();
		mtl_warn();
	}
}

void Cell::SetTime(ods::Time *p)
{
	ClearValue(true);
	
	if (p != nullptr)
	{
		office_value_type_ = ods::ValueType::Time;
		value_data_ = p;
	}
}

QString
Cell::ToSchemaString() const
{
	QString s;
	if (covered()) {
		s = QLatin1String("C ") + QString::number(ncr_);
	} else if (ncs_ > 1) {
		s = QLatin1String("S ") + QString::number(ncs_);
	} else {
		s = QLatin1String("R ") + QString::number(ncr_);
	}
	
	s = QChar('[') + s + QChar(']');
	
	if (selected()) {
		return QString(ods::color_red()) + s + QString(ods::color_default());
	}
	
	return s;
}

QByteArray
Cell::TypeAndValueString() const
{
	if (is_string())
	{
		const QString *s = GetCellString();
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
	if (is_date_time())
		return as_date_time()->toString(Qt::ISODate);
	if (is_time())
		return as_time()->toString();
	if (is_boolean())
		return *as_boolean() ? QLatin1String("true") : QLatin1String("false");
	if (is_string()) {
		const QString *s = GetCellString();
		return (s == nullptr) ? empty : *s;
	}
	
	mtl_it_happened();
	return empty;
}

void Cell::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->table(), ns::kNumberColumnsRepeated, ncr_, 1);
	
	{
		/// Workaround for MS Office (2019): if ncs_ > 1
		/// one must print out the nrs_ too, otherwise MS Office
		/// will treat ncs_ as 1 regardless of its real value
		
		/// Update 2: Both ncs_ and nrs_ must be written out in case
		/// any of them is != 1
		
		cint force_if_needed = (ncs_ != 1 || nrs_ != 1) ? -1 : 1;
		
		Write(xml, ns_->table(), ns::kNumberRowsSpanned, nrs_, force_if_needed);
		Write(xml, ns_->table(), ns::kNumberColumnsSpanned, ncs_, force_if_needed);
	}
	
	Write(xml, ns_->table(), ns::kStyleName, table_style_name_);
	
	if (is_value_set())
	{
		auto str = ods::TypeToString(office_value_type_);
		Write(xml, ns_->office(), ns::kValueType, str);
	}
	
	WriteValue(xml);
	WriteNodes(xml);
}

void Cell::WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba)
{
	MTL_CHECK_VOID(ba);
	WriteTag(kw, *ba);
	
	WriteNdffProp(kw, *ba, ns_->table(), ns::kNumberColumnsRepeated, ncr_, 1);
	
	{
		/// Workaround for MS Office (2019): if ncs_ > 1
		/// one must print out the nrs_ too, otherwise MS Office
		/// will treat ncs_ as 1 regardless of its real value
		
		/// Update 2: Both ncs_ and nrs_ must be written out in case
		/// any of them is != 1
		
		cint force_if_needed = (ncs_ != 1 || nrs_ != 1) ? -1 : 1;
		
		WriteNdffProp(kw, *ba, ns_->table(), ns::kNumberRowsSpanned, nrs_, force_if_needed);
		WriteNdffProp(kw, *ba, ns_->table(), ns::kNumberColumnsSpanned, ncs_, force_if_needed);
	}
	
	WriteNdffProp(kw, *ba, ns_->table(), ns::kStyleName, table_style_name_);
	
	if (is_value_set())
	{
		auto str = ods::TypeToString(office_value_type_);
		WriteNdffProp(kw, *ba, ns_->office(), ns::kValueType, str);
	}
	
	WriteValueNDFF(h, kw, file, ba);
	
	CloseBasedOnChildren(h, kw, file, ba);
}

void Cell::WriteValue(QXmlStreamWriter &xml)
{
	if (is_double())
	{
		Write(xml, ns_->office(), ns::kValue, QString::number(*as_double()));
	} else if (is_integer()) {
		Write(xml, ns_->office(), ns::kValue, QString::number(*as_integer()));
	} else if (is_currency()) {
		Write(xml, ns_->office(), ns::kValue, QString::number(*as_double()));
		Write(xml, ns_->office(), ns::kCurrency, office_currency_);
	} else if (is_percentage()) {
		Write(xml, ns_->office(), ns::kValue, QString::number(*as_double()));
	} else if (is_date()) {
//<table:table-cell table:style-name="ce5" office:value-type="date"
//		office:date-value="1983-12-30">
//<text:p>30.12.1983</text:p>
//</table:table-cell>
		auto *dt = as_date();
		QString date_value = dt->toString(Qt::ISODate);
		Write(xml, ns_->office(), ns::kDateValue, date_value);
	} else if (is_date_time()) {
		QDateTime *dt = as_date_time();
		QString date_value = dt->toString(Qt::ISODate);
		Write(xml, ns_->office(), ns::kDateValue, date_value);
	} else if (is_time()) {
		auto *dd = as_time();
		MTL_CHECK_VOID(dd);
		QString dur_value = dd->toString();
		Write(xml, ns_->office(), ns::kTimeValue, dur_value);
	} else if (is_boolean()) {
		QString str = *as_boolean() ? QLatin1String("true") : QLatin1String("false");
		Write(xml, ns_->office(), ns::kBooleanValue, str);
	} else if (is_string()) {
		// do nothing
	} else if (!is_value_set()) {
	} else {
		auto str = QueryAddress();
		auto *sheet = row_->sheet();
		QString s = str + QLatin1String(", sheet name: ") + sheet->name();
		mtl_printq(s);
	}
	
	if (formula_ != nullptr) {
		Write(xml, ns_->table(), ns::kFormula, formula_->ToXmlString());
	}
}

void Cell::WriteValueNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba)
{
	if (is_double())
	{
		WriteNdffProp(kw, *ba, ns_->office(), ns::kValue, QString::number(*as_double()));
	} else if (is_currency()) {
		WriteNdffProp(kw, *ba, ns_->office(), ns::kValue, QString::number(*as_double()));
		WriteNdffProp(kw, *ba, ns_->office(), ns::kCurrency, office_currency_);
	} else if (is_percentage()) {
		WriteNdffProp(kw, *ba, ns_->office(), ns::kValue, QString::number(*as_double()));
	} else if (is_date()) {
//<table:table-cell table:style-name="ce5" office:value-type="date"
//		office:date-value="1983-12-30">
//<text:p>30.12.1983</text:p>
//</table:table-cell>
		auto *dt = as_date();
		QString date_value = dt->toString(Qt::ISODate);
		WriteNdffProp(kw, *ba, ns_->office(), ns::kDateValue, date_value);
	} else if (is_date_time()) {
		QDateTime *dt = as_date_time();
		QString date_value = dt->toString(Qt::ISODate);
		WriteNdffProp(kw, *ba, ns_->office(), ns::kDateValue, date_value);
	} else if (is_time()) {
		auto *dd = as_time();
		MTL_CHECK_VOID(dd);
		QString dur_value = dd->toString();
		WriteNdffProp(kw, *ba, ns_->office(), ns::kTimeValue, dur_value);
	} else if (is_boolean()) {
		QString str = *as_boolean() ? ns::True : ns::False;
		WriteNdffProp(kw, *ba, ns_->office(), ns::kBooleanValue, str);
	} else if (is_string()) {
		// do nothing
	} else if (!is_value_set()) {
	} else {
		auto str = QueryAddress();
		auto *sheet = row_->sheet();
		QString s = str + QLatin1String(", sheet name: ") + sheet->name();
		mtl_printq(s);
	}
	
	if (formula_ != nullptr) {
		WriteNdffProp(kw, *ba, ns_->table(), ns::kFormula, formula_->ToXmlString());
	}
}

} // ods::
