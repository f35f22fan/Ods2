#include "Invoice.hpp"

#include "InvoiceItem.hpp"
#include <ods/line.hxx>

#include <QtCore>

namespace sample {

Invoice::Invoice() {
	Init();
}

Invoice::~Invoice() {
	delete book_;
}

int
Invoice::CreateBillToHeader(const int kLastRow)
{
	int last_row_index = kLastRow + 1;
	auto *row = sheet_->NewRowAt(last_row_index);
	
	auto *cell = row->NewCellAt(0);
	auto *style = book_->NewCellStyle();
	style->SetBoldText(true);
	
	cell->SetStyle(style);
	cell->SetValue("Bill To:");
	
	const int col1 = 1;
	const int col2 = 5;
	
	row->NewCellAt(col1)->SetValue("C20423");
	
	cell = row->NewCellAt(col2 - 1);
	cell->SetValue("Ship To:");
	cell->SetStyle(style);
	row->NewCellAt(col2)->SetValue("Ship name 1");
	
	row = sheet_->NewRowAt(++last_row_index);
	row->NewCellAt(col1)->SetValue("ABC company");
	row->NewCellAt(col2)->SetValue("Ship Address 1");
	
	row = sheet_->NewRowAt(++last_row_index);
	row->NewCellAt(col1)->SetValue("232 Brooklyn Street");
	row->NewCellAt(col2)->SetValue("Ship Address 1");
	
	row = sheet_->NewRowAt(++last_row_index);
	row->NewCellAt(col1)->SetValue("New York");
	row->NewCellAt(col2)->SetValue("Whatever");
	
	row = sheet_->NewRowAt(++last_row_index);
	row->NewCellAt(col1)->SetValue("USA");
	row->NewCellAt(col2)->SetValue("Canada");
	
	return last_row_index;
}

int
Invoice::CreateSellerHeader()
{
	auto *row1 = sheet_->NewRowAt(1);
	auto *cell = row1->NewCellAt(0);
	const QString kName = "company-logo.png";
	auto path = QDir(QDir::homePath()).filePath(kName); 
	QFile file(path);
	
	if (!file.exists()) {
		auto ba = kName.toLocal8Bit();
		mtl_line("If there was %s "
			"in your home dir, I'd use it as the \"company logo\" "
			"in the invoice.", ba.data());
	} else {
		auto [draw_frame, draw_image, sz] = cell->NewDrawFrame(path);
		draw_frame->SetSize(60, 60, ods::Unit::Px);
		
	}
	
	ods::inst::StyleStyle *style = cell->FetchStyle();
	ods::inst::StyleTextProperties *tp = style->GetStyleTextProperties
		(ods::AddIfNeeded::Yes);
	
	// set font weight:
	auto *font_weight = new ods::attr::FoFontWeight();
	font_weight->SetBold();
	tp->SetFontWeight(font_weight);
		
	// set font size:
	auto *l = new ods::Length(18, ods::Unit::Px);
	tp->SetFontSize(l);
	
	const int kCompanyCol = 1;
	const int kShipToCol = 5;
	
	cell = row1->NewCellAt(kCompanyCol);
	cell->SetStyle(style);
	
	cell->SetFirstString("Your company name");
	cell->SetRowColSpan(1, 4);
	
	row1->SetOptimalHeight();
	
	auto *row2 = sheet_->NewRowAt(2);
	row2->NewCellAt(kCompanyCol)->SetString("Street Address");
	cell = row2->NewCellAt(kShipToCol);
	cell->SetRowColSpan(1, 3);
	auto date = QDate::currentDate();
	cell->SetString(QString("Date: ") + date.toString());
	
	auto *row = sheet_->NewRowAt(3);
	row->NewCellAt(kCompanyCol)->SetString("City, ST, ZIP code");
	cell = row->NewCellAt(kShipToCol);
	cell->SetRowColSpan(1, 3);
	cell->SetString("INVOICE #: INV4203");
	
	row = sheet_->NewRowAt(4);
	row->NewCellAt(kCompanyCol)->SetString("Phone number, Web address");
	
	style = book_->NewCellStyle();
	style->SetBorder(ods::Length(5.0, ods::Unit::Pt),
		QColor(200, 200, 200), ods::line::Style::Solid, ods::BorderTop);
	
	const int last_col = 6;
	row = sheet_->NewRowAt(last_col);
	
	for (int i = 0; i <= kShipToCol+2; i++)
	{
		cell = row->NewCellAt(i);
		cell->SetStyle(style);
	}
	
	return last_col;
}

int
Invoice::CreateTable(QVector<InvoiceItem*> *vec, const int kLastRow)
{
	int last_row_index = kLastRow + 1;
	auto *ds = book_->document_styles();
	auto *ncs = ds->office_styles()->NewNumberCurrencyStyle();

	auto *cs = ncs->FetchCurrencySymbol();
	{
		cs->SetSymbol(ods::currency::EUR);
		cs->language(ods::lang::German.str);
		cs->country(ods::country::Germany);
	}
	
	auto *nn = ncs->FetchNumber();
	{
		nn->grouping(1);
		//nn->min_integer_digits(2);
		nn->decimal_places(4);
		nn->min_decimal_places(2);
		// -1 means value not set.
		// Note: Calligra Sheets doesn't display currency formatting properly
	}
	
	auto *light_style = book_->NewStyle(ods::Place::StylesFile,
		ods::style::Family::Cell);
	light_style->SetBorder(ods::Length(1.0, ods::Unit::Px), QColor(0, 0, 0),
		ods::line::Style::Solid, ods::BorderLeft | ods::BorderRight);
	light_style->SetBackgroundColor(QColor(255, 255, 255));
	
	auto *dark_style = book_->NewStyle(ods::Place::StylesFile,
		ods::style::Family::Cell);
	dark_style->SetBorder(ods::Length(1.0, ods::Unit::Px), QColor(0, 0, 0),
		ods::line::Style::Solid, ods::BorderLeft | ods::BorderRight);
	dark_style->SetBackgroundColor(QColor(230, 230, 230));
	
	auto *light_style_currency = light_style->DeriveCellStyle();
	light_style_currency->SetDataStyle(ncs);
	auto *dark_style_currency = dark_style->DeriveCellStyle();
	dark_style_currency->SetDataStyle(ncs);

	QVector<ods::Cell*> line_total_cells;
	
	for(int i = 0; i < vec->size(); i++, last_row_index++)
	{
		auto *item = vec->at(i);
		int next_col = 0;
		
		auto *style = (i % 2) ? dark_style : light_style;
		auto *currency_style = (i % 2) ? dark_style_currency : light_style_currency;
		auto *row = sheet_->NewRowAt(last_row_index);
		
		auto *cell = row->NewCellAt(next_col);
		QString id_str = QString("IT0") + QString::number(item->id());
		cell->SetValue(id_str);
		cell->SetStyle(style);
		
		cell = row->NewCellAt(++next_col);
		cell->SetRowColSpan(1, 3);
		cell->SetValue(item->description());
		cell->SetStyle(style);
		
		next_col += 3;
		auto *qtty_cell = row->NewCellAt(next_col);
		qtty_cell->SetValue(item->qtty_ordered());
		qtty_cell->SetStyle(style);
		
		auto *price_cell = row->NewCellAt(++next_col);
		price_cell->SetCurrency(item->unit_price(), ods::currency::EUR);
		price_cell->SetStyle(currency_style);
		
		auto *line_total_cell = row->NewCellAt(++next_col);
		line_total_cell->SetRowColSpan(1, 2);
		line_total_cell->SetStyle(currency_style);
		
		ods::Formula *formula = line_total_cell->NewFormula();
		formula->Add(qtty_cell);
		formula->Add(ods::Op::Multiply);
		formula->Add(price_cell);
		
		line_total_cells.append(line_total_cell);
	}
	
	auto *row = sheet_->NewRowAt(last_row_index++);
	
	const int TotalStrCol = 5;
	auto *total_str_cell = row->NewCellAt(TotalStrCol);
	total_str_cell->SetValue("TOTAL:");
	auto *style = book_->NewCellStyle();
	style->SetHAlignment(ods::HAlignSide::Right);
	total_str_cell->SetStyle(style);
	
	const int TotalCol = TotalStrCol + 1;
	auto *total_cell = row->NewCellAt(TotalCol);
	total_cell->SetRowColSpan(1, 2);
	auto *total_style = book_->NewCellStyle();
	total_style->SetDataStyle(ncs);
	total_cell->SetStyle(total_style);
	total_style->SetBorder(ods::Length(1.0, ods::Unit::Px), QColor(0, 0, 0),
		ods::line::Style::Solid, ods::BorderAll);
	
	ods::Formula *total_formula = total_cell->NewFormula();
	const auto kCount = line_total_cells.size();
	
	for(int i = 0; i < kCount; i++)
	{
		total_formula->Add(line_total_cells[i]);
		
		if (i < kCount - 1)
			total_formula->Add(ods::Op::Add);
	}
	
	row = sheet_->NewRowAt(++last_row_index);
	auto *notes_cell = row->NewCellAt(0);
	notes_cell->SetValue("Notes:");
	auto *notes_style = notes_cell->FetchStyle();
	notes_style->SetBoldText(true);
	
	row = sheet_->NewRowAt(++last_row_index);
	auto *area_cell = row->NewCellAt(0);
	area_cell->SetRowColSpan(6, 5);
	auto *area_style = area_cell->FetchStyle();
	area_style->SetBorder(ods::Length(1.0, ods::Unit::Px),
		QColor(0, 0, 0), ods::line::Style::Dotted);
	
	last_row_index += area_cell->number_rows_spanned() + 1;
	row = sheet_->NewRowAt(last_row_index);
	auto *thanks_cell = row->NewCellAt(0);
	thanks_cell->SetRowColSpan(2, 8);
	thanks_cell->SetValue("Thank You For Your Business!");
	auto *thanks_style = thanks_cell->FetchStyle();
	thanks_style->SetBackgroundColor(QColor(230, 230, 230));
	thanks_style->SetHAlignment(ods::HAlignSide::Center);
	thanks_style->SetVAlignment(ods::VAlignSide::Middle);
	thanks_style->SetFontStyle(ods::attr::FontStyle::Italic);

	return last_row_index;
}

int
Invoice::CreateTableHeader(const int kLastRow)
{
	int last_row_index = kLastRow + 2;
	auto *row = sheet_->NewRowAt(last_row_index);
	
	auto *style = book_->NewCellStyle();
	style->SetBoldText(true);
	
	const int c = 220;
	style->SetBackgroundColor(QColor(c, c, c));
	style->SetHAlignment(ods::HAlignSide::Center);

	style->SetBorder(ods::Length(1.0, ods::Unit::Px),
		QColor(0, 0, 0), ods::line::Style::Solid);
	
	int next_cell_index = 0;
	auto *cell = row->NewCellAt(next_cell_index++);
	cell->SetStyle(style);
	cell->SetValue("ID");
	
	cell = row->NewCellAt(next_cell_index++);
	cell->SetRowColSpan(1, 3);
	cell->SetStyle(style);
	cell->SetValue("Description");
	
	next_cell_index += 2;
	cell = row->NewCellAt(next_cell_index);
	cell->SetStyle(style);
	cell->SetValue("Qtty");
	
	cell = row->NewCellAt(++next_cell_index);
	cell->SetStyle(style);
	cell->SetValue("Price");
	
	cell = row->NewCellAt(++next_cell_index);
	cell->SetStyle(style);
	cell->SetRowColSpan(1, 2);
	cell->SetValue("Line Total");
	
	return last_row_index;
}

QVector<InvoiceItem*>*
Invoice::GenItems()
{
	auto *vec = new QVector<InvoiceItem*>();
	vec->append(new InvoiceItem("Motorola F20", 225.5, 10));
	vec->append(new InvoiceItem("Android 4", 310.0, 2));
	vec->append(new InvoiceItem("Nokia MT", 400.0, 20));
	vec->append(new InvoiceItem("MS Vaporware", 100.0, 10));
	vec->append(new InvoiceItem("Intel SP", 180.0, 1));
	vec->append(new InvoiceItem("GM FX 2", 360, 1));
	vec->append(new InvoiceItem("PS 4", 300, 2));
	vec->append(new InvoiceItem("Xbox 2", 150, 5));
	
	return vec;
}

void
Invoice::Init()
{
	book_ = ods::Book::New();
	auto *spr = book_->spreadsheet();
	sheet_ = spr->NewSheet("Invoice");
	
	int last_index = CreateSellerHeader();
	
	if (last_index == -1) {
		qDebug() << "CreateSellerHeader() failed";
		return;
	}
	
	last_index = CreateBillToHeader(last_index);
	
	if (last_index == -1) {
		qDebug() << "CreateBillToHeader() failed";
		return;
	}
	
	last_index = CreateTableHeader(last_index);
	
	if (last_index == -1) {
		qDebug() << "CreateTableHeader() failed";
		return;
	}

	QVector<InvoiceItem*> *items = GenItems();
	last_index = CreateTable(items, last_index);
	
	if (last_index == -1) {
		qDebug() << "CreateTable() failed";
		return;
	}
	/*
	last_index = CreateNotes(last_index);
	
	if (last_index == -1) {
		qDebug() << "CreateNotes() failed";
		return;
	} */
	
	auto path = QDir(QDir::homePath()).filePath("Invoice.ods");
	QFile target(path);
	QString err;
	
	if (book_->Save(target, &err))
		qDebug() << "Saved to:" << target.fileName();
	else
		qDebug() << "Error saving ods file:" << err;
}


}
