#include "examples1.hh"

#include "util.hh"

#include <ods/ods>
#include <ods/err.hpp>
#include <ods/inst/SvgDesc.hpp>
#include <ods/inst/SvgTitle.hpp>
#include <ods/inst/NumberBooleanStyle.hpp>
#include <ods/inst/NumberDay.hpp>
#include <ods/inst/NumberHours.hpp>
#include <ods/inst/NumberMinutes.hpp>
#include <ods/inst/NumberMonth.hpp>
#include <ods/inst/NumberSeconds.hpp>
#include <ods/inst/NumberText.hpp>
#include <ods/inst/NumberYear.hpp>

#include <QSize>

void TestBugJuly2023()
{
	QString read_from_path = QDir::home().filePath("Downloads/slaves.ods");
	QString save_to_path = QDir::home().filePath("SavedTo.ods");
	QString err;
	ods::Book *src_book = ods::Book::FromFile(read_from_path, &err);
	if (!err.isEmpty())
	{
		mtl_info("Error: %s", qPrintable(err));
		return;
	}
	auto *spreadsheet = src_book->spreadsheet();
	auto *sheet = spreadsheet->GetSheet(1);
	const int row_index = 2;
	const int col_index = 12;
	auto *row = sheet->GetRow(row_index);
	auto *cell = row->GetCell(col_index);
	if (cell)
	{
		if (cell->ncr() > 1) {
			mtl_info("Split the cell");
			cell = row->SplitOneCellAt(cell, col_index);
		} else {
			mtl_info("Didn't split the cell");
		}
		cell->SetDouble(213);
	} else {
		mtl_warn("No cell at row %d, col %d", row_index, col_index);
	}
	
	src_book->Save(save_to_path, &err);
	if (!err.isEmpty())
	{
		mtl_warn("Error: %s", qPrintable(err));
		return;
	} else {
		mtl_info("Saved to: %s", qPrintable(save_to_path));
	}
}

void CopyAnOdsFile()
{
	// Task: copy a file from example.ods to MyPath.ods and change
	// the latter's first cell value to "ABC".
	
	QString src_path = QDir::home().filePath("example.ods");
	QString err;
	ods::Book *src_book = ods::Book::FromFile(src_path, &err);
	
	if (!err.isEmpty())
	{
		mtl_info("Error at file %s: %s", qPrintable(src_path), qPrintable(err));
		return;
	}
	
	auto *sheet = src_book->GetSheet(0);
	auto *row = sheet->GetRow(0);
	auto *cell = row->GetCell(0);
	if (cell)
	{
		cell->SetValue("ABC");
	}
	const char *file_name = "MyCopy.ods";
	QString out_path = QDir::home().filePath(file_name);
	src_book->Save(out_path, &err);
	if (!err.isEmpty())
	{
		mtl_info("Error: %s", qPrintable(err));
		return;
	}
}

void ReadWriteNDFF(QStringView full_path)
{
	ods::Book *book = ods::Book::FromNDFF(full_path);
	if (!book)
	{
		mtl_warn("ods::Book == NULL");
		return;
	}
	
	book->dev_mode(true);
	//book->compression(true);
	ods::AutoDelete<ods::Book*> ad(book);
	util::Save(book);
}

void TestBug()
{
	auto *book = ods::Book::New();
	ods::AutoDelete<ods::Book*> ad(book);
	
	auto *spreadsheet = book->spreadsheet();
	auto *sheet = spreadsheet->NewSheet("Sheet name");
	
	auto *row = sheet->NewRowAt(0);
	auto *cell = row->NewCellAt(0);
	cell->ncr(2);
	
	
	cell = row->NewCellAt(2);
	auto *date = new QDate(QDate::currentDate());
	cell->SetDate(date);
	
	for (int i = 0; i < 3; i++) {
		auto *next = row->GetCell(i);
		mtl_info("Cell %d is empty: %s, (%s)",
			i, next->is_empty() ? "true" : "false",
			qPrintable(next->ValueToString()));
	}
	
	
	util::Save(book);
}

void SetColumnWidths()
{
	auto *book = ods::Book::New();
	ods::AutoDelete<ods::Book*> ad(book);
	
	auto *spreadsheet = book->spreadsheet();
	auto *sheet = spreadsheet->NewSheet("Sheet name");
	
	{
		// A new style per column:
		ods::inst::TableTableColumn *col = sheet->NewColumnAt(3);
		ods::Length width(7.5, ods::Unit::Cm); // 7.5 cm
		col->SetWidth(&width);
		//col->SetWidth(nullptr); // resets to default width
		
		col = sheet->NewColumnAt(1);
		ods::Length width2(1.5, ods::Unit::In); // 1.5 in
		col->SetWidth(&width2);
	}
	
	{
		// Same style for many columns, thus saving .ods file space:
		ods::inst::StyleStyle *style = book->NewColumnStyle();
		ods::inst::StyleTableColumnProperties *tcp = style->NewTableColumnProperties();
		ods::Length width3(65, ods::Unit::Px); // 65 px
		tcp->SetColumnWidth(&width3);
		
		for (int i = 6; i < 10; i++)
		{
			auto *col = sheet->NewColumnAt(i);
			col->SetStyle(style);
		}
	}
	
	util::Save(book);
}

void ReadColumnWidths()
{
	auto full_path = util::FindFile("ColumnWidths.ods");
	if (full_path.isEmpty())
		return;
	
	QString err;
	auto *book = ods::Book::FromFile(full_path, &err);
	ods::AutoDelete<ods::Book*> ad(book);
	
	if (!err.isEmpty()) {
		mtl_warn("%s", qPrintable(err));
		return;
	}
	
	
	auto *spreadsheet = book->spreadsheet();
	auto *sheet = spreadsheet->GetSheet(0);
	util::PrintWidth(sheet->GetColumn(0));
	util::PrintWidth(sheet->GetColumn(1));
	util::PrintWidth(sheet->GetColumn(3));
	util::PrintWidth(sheet->GetColumn(6));
}

void CreateFont()
{
	auto *book = ods::Book::New();
	ods::AutoDelete<ods::Book*> ad(book);
	auto *spreadsheet = book->spreadsheet();
	auto *sheet = spreadsheet->NewSheet("Sheet name");
	auto *row = sheet->NewRowAt(0);
	
	const int col = 2;
	auto *cell = row->NewCellAt(col);
	cell->SetString("Row2Cel0");
	auto *style = cell->FetchStyle();
	ods::inst::StyleTextProperties *tp = style->FetchStyleTextProperties();

	// set font style:
	auto *font_style = new ods::attr::FoFontStyle();
	font_style->SetOblique();
	tp->SetFontStyle(font_style);

	// set font weight:
	auto *font_weight = new ods::attr::FoFontWeight();
	font_weight->SetBold();
	tp->SetFontWeight(font_weight);
	
	// set font size:
	auto *l = new ods::Length(0.8, ods::Unit::Cm);
	tp->SetFontSize(l);
	
	// set font face:
	auto *font_factory = book->GetFontFaceDecls();
	ods::inst::StyleFontFace *sff = font_factory->GetFontFace(
		QLatin1String("Noto Serif CJK KR"), ods::AddIfNeeded::Yes);
	tp->SetFontFace(sff);
	
	util::Save(book);
}

void ReadFont()
{
	auto full_path = util::FindFile("Font.ods");
	
	if (full_path.isEmpty())
		return;
	
	QString err;
	auto *book = ods::Book::FromFile(full_path, &err);
	ods::AutoDelete<ods::Book*> ad(book);
	if (!err.isEmpty())
	{
		mtl_warn("%s", qPrintable(err));
		return;
	}
	
	
	auto *spreadsheet = book->spreadsheet();
	auto *sheet = spreadsheet->GetSheet(0);
	auto *row = sheet->GetRow(0);
	auto *cell = row->GetCell(2);
	if (cell == nullptr)
	{
		mtl_info("No cell");
		return;
	}
	
	ods::inst::StyleStyle *style = cell->GetStyle();
	if (style == nullptr)
	{
		mtl_warn("no style");
		return;
	}
	
	auto *tp = (ods::inst::StyleTextProperties*)style->Get(ods::Id::StyleTextProperties);
	if (tp == nullptr)
	{
		mtl_warn("No text properties");
		return;
	}
	
	ods::attr::FoFontStyle* font_style = tp->font_style();
	if (font_style == nullptr)
	{
		mtl_info("No font style");
	} else {
		if (font_style->is_italic())
			mtl_info("Font style is italic");
		else if (font_style->is_normal())
			mtl_info("Font style is normal");
		// etc..
		
		auto ba = font_style->toString().toLocal8Bit();
		mtl_info("Font style toString(): %s", ba.data());
	}
	
	ods::attr::FoFontWeight *font_weight = tp->font_weight();
	if (font_weight == nullptr)
	{
		mtl_info("No font weight");
	} else {
		if (font_weight->is_bold())
			mtl_info("Font weight is bold");
		else if (font_weight->is_normal())
			mtl_info("Font weight is normal");
		// etc..
		
		auto ba = font_weight->toString().toLocal8Bit();
		mtl_info("Font weight toString(): %s", ba.data());
	}
	
	ods::Length *font_size = tp->font_size();
	
	if (font_size == nullptr)
	{
		mtl_info("No font size");
	} else {
		mtl_info("Font size value: %f", font_size->size());
		
		if (font_size->is_cm())
			mtl_info("Font size as cm");
		else if (font_size->is_in())
			mtl_info("Font size as in");
		// etc..
		
		auto ba = font_size->toString().toLocal8Bit();
		mtl_info("Font size toString(): %s", ba.data());
	}
	
	ods::inst::StyleFontFace *font_face = tp->font_face();
	
	if (font_face == nullptr)
	{
		mtl_info("No font face");
	} else {
		auto ba = font_face->font_family().toLocal8Bit();
		mtl_info("Font family: \"%s\"", ba.data());
	}
}

void CreateColorsAndUnderline()
{
	auto *book = ods::Book::New();
	ods::AutoDelete<ods::Book*> ad(book);
	auto *spreadsheet = book->spreadsheet();
	auto *sheet = spreadsheet->NewSheet("Sheet name");
	auto *row = sheet->NewRowAt(0);
	
	const int col = 2;
	auto *cell = row->NewCellAt(col);
	cell->SetString("This is some cell text");
	auto *style = cell->GetStyle();
	
	if (style == nullptr)
	{
		style = cell->NewStyle();
	} else {
		// derive new style to avoid changing other cells' appearance
		style = style->DeriveCellStyle();
		cell->SetStyle(style);
	}
	
	// set text color:
	auto *tp = style->FetchStyleTextProperties();
	tp->SetColor(new QColor(0, 0, 255));
	
	// set text underline color:
	auto *tuc = new ods::attr::StyleTextUnderlineColor();
	tuc->SetColor(new QColor(0, 255, 0));
	tp->SetTextUnderlineColor(tuc);
	
	// set text underline style:
	auto *tus = new ods::LineStyle();
	tus->SetSolid();
	tp->SetTextUnderlineStyle(tus);
	
	// set text underline width:
	auto *tuw = new ods::attr::StyleTextUnderlineWidth();
	tuw->SetBold();
	tp->SetTextUnderlineWidth(tuw);
	
	// set background color:
	auto *stcp = style->FetchStyleTableCellProperties();
	stcp->SetBackgroundColor(QColor(255, 255, 0));
	
	/// also wrap text:
	///stcp->SetWrapOption(true);
	
	auto ba = cell->TypeAndValueString();
	mtl_info("Cell type and value: %s", ba.data());
	
	util::Save(book);
}

void ReadColorsAndUnderline()
{
	auto full_path = util::FindFile("ColorsAndUnderline.ods");
	if (full_path.isEmpty())
		return;
	
	QString err;
	auto *book = ods::Book::FromFile(full_path, &err);
	ods::AutoDelete<ods::Book*> ad(book);
	if (!err.isEmpty())
	{
		mtl_warn("%s", qPrintable(err));
		return;
	}
	
	
	auto *spreadsheet = book->spreadsheet();
	auto *sheet = spreadsheet->GetSheet(0);
	auto *row = sheet->GetRow(0);
	auto *cell = row->GetCell(2);
	ods::inst::StyleStyle *style = cell->GetStyle();
	if (style == nullptr)
	{
		mtl_warn("No style");
		return;
	}
	
	auto *tp = (ods::inst::StyleTextProperties*) style->Get(ods::Id::StyleTextProperties);
	if (tp == nullptr)
	{
		mtl_warn("No text properties");
		return;
	}
	
	auto *color = tp->color();
	if (color == nullptr)
	{
		mtl_info("No text color");
	} else {
		auto ba = color->name().toLocal8Bit();
		mtl_info("Text color: %s", ba.data());
	}
	
	ods::attr::StyleTextUnderlineColor *tuc = tp->text_underline_color();
	if (tuc == nullptr)
	{
		mtl_info("No text underline color attribute");
	} else {
		QColor *c = tuc->color();
		
		if (c == nullptr)
		{
			mtl_info("No text underline color attribute");
		} else {
			auto ba = c->name().toLocal8Bit();
			mtl_info("Text underline color: %s", ba.data());
		}
	}
	
	ods::LineStyle *tus = tp->text_underline_style();
	
	if (tus == nullptr)
	{
		mtl_info("No text underline style attribute");
	} else {
		mtl_info("==>Text underline style properties");
		mtl_info("Is none: %s", tus->is_none() ? "true" : "false");
		mtl_info("Is dash: %s", tus->is_dash() ? "true" : "false");
		mtl_info("Is dot dash: %s", tus->is_dot_dash() ? "true" : "false");
		mtl_info("Is dot dot dash: %s", tus->is_dot_dot_dash() ? "true" : "false");
		mtl_info("Is dotted: %s", tus->is_dotted() ? "true" : "false");
		mtl_info("Is long dash: %s", tus->is_long_dash() ? "true" : "false");
		mtl_info("Is solid: %s", tus->is_solid() ? "true" : "false");
		mtl_info("Is wave: %s", tus->is_wave() ? "true" : "false");
		mtl_info("Is valid: %s", tus->is_valid() ? "true" : "false");
		
		mtl_info("toString(): %s", qPrintable(tus->toString()));
		mtl_info("<==Text underline style properties");
	}
	
	ods::attr::StyleTextUnderlineWidth *tuw = tp->text_underline_width();
	if (tuw == nullptr)
	{
		mtl_info("No text underline width");
	} else {
		auto ba = tuw->toString().toLocal8Bit();
		mtl_info("Text underline width: %s", ba.data());
	}
	
	auto *tcp = (ods::inst::StyleTableCellProperties*)style->Get(ods::Id::StyleTableCellProperties);
	if (tcp == nullptr)
	{
		mtl_info("No table cell properties");
	} else {
		QColor *bg = tcp->background_color();
		
		if (bg == nullptr)
		{
			mtl_info("No background color");
		} else {
			auto ba = bg->name().toLocal8Bit();
			mtl_info("Background color: %s", ba.data());
		}
	}
}

void CellSpan()
{
	auto *book = ods::Book::New();
	ods::AutoDelete<ods::Book*> ad(book);
	auto *spreadsheet = book->spreadsheet();
	auto *sheet = spreadsheet->NewSheet("Sheet name");
	auto *row = sheet->NewRowAt(0);
	auto *cell = row->NewCellAt(0);
	
	// AFAIK cells can either span or repeat over multiple
	// rows/columns, but not both at the same time.
	// Also note, when a cell spans over other cells those cells are still
	// readable/writable, they're just "covered cells".
	
	cell->SetString("2 cols, 3 rows");
	cell->number_columns_spanned(2); // default is 1
	cell->number_rows_spanned(3); // default is 1
	
	cell = row->NewCellAt(5);
	cell->SetString("4 cols repeated");
	cell->number_columns_repeated(4); // default is 1
	
	util::Save(book);
}

void ReadCellSpan()
{
	auto full_path = util::FindFile("CellSpan.ods");
	if (full_path.isEmpty())
		return;
	
	QString err;
	auto *book = ods::Book::FromFile(full_path, &err);
	ods::AutoDelete<ods::Book*> ad(book);
	if (!err.isEmpty())
	{
		mtl_warn("%s", qPrintable(err));
		return;
	}
	
	auto *spreadsheet = book->spreadsheet();
	auto *sheet = spreadsheet->GetSheet(0);
	auto *row = sheet->GetRow(0);
	auto *cell = row->GetCell(0);
	
	mtl_info("Cell at 0:0: num_cols_spanned: %d, num_rows_spanned:%d",
		cell->number_columns_spanned(), cell->number_rows_spanned());
	
	cell = row->GetCell(5);
	mtl_info("Cell at 0:5: num_cols_repeated: %d",
		cell->number_columns_repeated());
	
}

void CreatePercentage()
{
	auto *book = ods::Book::New();
	ods::AutoDelete<ods::Book*> ad(book);
	
	auto *spr = book->spreadsheet();
	auto *sheet = spr->NewSheet("Sheet name");
	auto *row = sheet->NewRowAt(0);

	auto *cell = row->NewCellAt(0);
	cell->SetPercentage(0.83); // 83%
	ods::inst::StyleStyle *style = cell->FetchStyle();
	style->SetPercentage(3, 4); // Display as "083.0000%"
	
	cell = row->NewCellAt(1);
	style = cell->FetchStyle();
	cell->SetPercentage(1.33); // 130%
	style->SetPercentage(4, 2); // Display as "0133.00%"
	
	cell = row->NewCellAt(2);
	style = cell->FetchStyle();
	cell->SetPercentage(0.5); // 50%
	style->SetPercentage(0, 0); // Display as "50%"
	
	util::Save(book);
}

void ReadPercentage()
{
	auto full_path = util::FindFile("Percentage.ods");
	if (full_path.isEmpty())
		return;
	
	QString err;
	auto *book = ods::Book::FromFile(full_path, &err);
	ods::AutoDelete<ods::Book*> ad(book);
	if (!err.isEmpty())
	{
		mtl_warn("%s", qPrintable(err));
		return;
	}
	
	auto *spreadsheet = book->spreadsheet();
	auto *sheet = spreadsheet->GetSheet(0);
	auto *row = sheet->GetRow(0);
	
	for (int i = 0; i < 3; i++)
	{
		auto *cell = row->GetCell(i);
		if (cell == nullptr)
		{
			mtl_warn("Cell at %d == nullptr", i);
			break;
		}
		
		util::PrintPercentage(cell);
		printf("\n");
	}
}

void CreateHAlignment()
{
	auto *book = ods::Book::New();
	ods::AutoDelete<ods::Book*> ad(book);
	
	auto *spr = book->spreadsheet();
	auto *sheet = spr->NewSheet("Sheet name");
	auto *row = sheet->NewRowAt(0);
	auto *cell = row->NewCellAt(0);
	cell->SetString("Hello");
	
	ods::inst::StyleStyle *style = cell->FetchStyle();
	style->SetHAlignment(ods::HAlignSide::Center);
	
	cell = row->NewCellAt(2);
	cell->SetDouble(3.1415);
	style = book->NewCellStyle();
	cell->SetStyle(style);
	style->SetHAlignment(ods::HAlignSide::Right);
	
	cell = row->NewCellAt(3);
	cell->SetDouble(932.0);
	style = book->NewCellStyle();
	cell->SetStyle(style);
	style->SetHAlignment(ods::HAlignSide::Left);
	
	util::Save(book);
}

void ReadHAlignment()
{
	auto full_path = util::FindFile("HAlignment.ods");
	if (full_path.isEmpty())
		return;
	
	QString err;
	auto *book = ods::Book::FromFile(full_path, &err);
	ods::AutoDelete<ods::Book*> ad(book);
	if (!err.isEmpty())
	{
		mtl_warn("%s", qPrintable(err));
		return;
	}
	
	auto *spreadsheet = book->spreadsheet();
	auto *sheet = spreadsheet->GetSheet(0);
	auto *row = sheet->GetRow(0);
	const int col = 3;
	auto *cell = row->GetCell(col);
	if (cell == nullptr)
	{
		mtl_warn("No cell at %d", col);
		return;
	}
	
	auto *style = cell->GetStyle();
	if (style == nullptr)
	{
		mtl_warn("No style");
		return;
	}
	
	auto *spp = (ods::inst::StyleParagraphProperties*)
		style->Get(ods::Id::StyleParagraphProperties);
	if (spp == nullptr)
	{
		mtl_warn("No style paragraph properties");
		return;
	}
	
	ods::HAlign *ha = spp->text_align();
	if (ha == nullptr)
	{
		mtl_warn("text alignment not specified");
		return;
	}
	
	if (ha->is_left())
		mtl_info("Align left");
	else if (ha->is_center())
		mtl_info("Align center");
	else if (ha->is_right())
		mtl_info("Align right");
	else
		mtl_info("Align invalid");
}

void CreateVAlignment()
{
	auto *book = ods::Book::New();
	ods::AutoDelete<ods::Book*> ad(book);
	
	auto *spr = book->spreadsheet();
	auto *sheet = spr->NewSheet("Sheet name");
	auto *row = sheet->NewRowAt(0);
	auto *cell = row->NewCellAt(0);
	cell->SetString("Hello");
	
	auto *style = cell->GetStyle();
	
	if (style == nullptr)
		style = cell->NewStyle();
	
	style->SetVAlignment(ods::VAlignSide::Bottom);
	
	cell = row->NewCellAt(1);
	cell->SetString("world!");
	style = book->NewCellStyle();
	cell->SetStyle(style);
	style->SetVAlignment(ods::VAlignSide::Middle);
	
	cell = row->NewCellAt(2);
	cell->SetDouble(3.14);
	style = book->NewCellStyle();
	cell->SetStyle(style);
	style->SetVAlignment(ods::VAlignSide::Top);
	
	util::Save(book);
}

void ReadVAlignment()
{
	auto full_path = util::FindFile("VAlignment.ods");
	if (full_path.isEmpty())
		return;
	
	QString err;
	auto *book = ods::Book::FromFile(full_path, &err);
	ods::AutoDelete<ods::Book*> ad(book);
	if (!err.isEmpty())
	{
		mtl_warn("%s", qPrintable(err));
		return;
	}
	
	auto *spreadsheet = book->spreadsheet();
	auto *sheet = spreadsheet->GetSheet(0);
	auto *row = sheet->GetRow(0);
	const int col = 2;
	auto *cell = row->GetCell(col);
	if (cell == nullptr)
	{
		mtl_warn("No cell at %d", col);
		return;
	}
	
	auto *style = cell->GetStyle();
	if (style == nullptr)
	{
		mtl_warn("No style");
		return;
	}
	
	auto *tcp = (ods::inst::StyleTableCellProperties*)
		style->Get(ods::Id::StyleTableCellProperties);
	if (tcp == nullptr)
	{
		mtl_warn("No table cell properties");
		return;
	}
	
	ods::VAlign *va = tcp->vertical_align();
	if (va == nullptr)
	{
		mtl_warn("vertical alignment not specified");
		return;
	}
	
	if (va->is_bottom())
		mtl_info("Align bottom");
	else if (va->is_middle())
		mtl_info("Align middle");
	else if (va->is_top())
		mtl_info("Align top");
	else
		mtl_info("Vertical align invalid");
	
}

void CreateBorders()
{
	auto *book = ods::Book::New();
	ods::AutoDelete<ods::Book*> ad(book);
	
	auto *spr = book->spreadsheet();
	auto *sheet = spr->NewSheet("Sheet name");
	auto *row = sheet->NewRowAt(3);
	auto *cell = row->NewCellAt(3);
	cell->SetString("Hello");
	
	auto *style = cell->FetchStyle();
	auto *tcp = style->FetchTableCellProperties();
	
	ods::attr::Border border;
	
	ods::LineStyle line_style(ods::line::Style::Dotted);
	border.line_style(&line_style);
	
	ods::Length width(2.0, ods::Unit::Mm);
	border.width(&width);
	
	QColor color(0, 0, 255);
	border.color(&color);
	
	tcp->border_top(&border);
	tcp->border_bottom(&border);
	
	// tcp->border(&border); to set all sides to same border.
	// If set, the other borders must be set to null if already set:
	// tcp->border_top(nullptr); tcp->border_bottom(nullptr);
	
	util::Save(book);
}

void ReadBorders()
{
	auto full_path = util::FindFile("Borders.ods");
	if (full_path.isEmpty())
		return;
	
	QString err;
	auto *book = ods::Book::FromFile(full_path, &err);
	ods::AutoDelete<ods::Book*> ad(book);
	if (!err.isEmpty())
	{
		mtl_warn("%s", qPrintable(err));
		return;
	}
	
	auto *spreadsheet = book->spreadsheet();
	auto *sheet = spreadsheet->GetSheet(0);

	for (int row_index = 1; row_index < 6; row_index++)
	{
		auto *row = sheet->GetRow(row_index);
		
		for (int j = 0; j < 7; j++)
		{
			auto *cell = row->GetCell(j);
			util::PrintBorder(cell, row_index, j);
		}
	}
}

void CreateImage()
{
	auto *book = ods::Book::New();
	ods::AutoDelete<ods::Book*> ad(book);
	
	auto *spr = book->spreadsheet();
	auto *sheet = spr->NewSheet("Sheet name");
	auto *row = sheet->NewRowAt(3);
	auto *cell = row->NewCellAt(3);
	
	auto *draw_frame = (ods::inst::DrawFrame*) cell->Get(ods::Id::DrawFrame);
	if (draw_frame == nullptr)
		draw_frame = cell->NewDrawFrame();
	
	auto *draw_image = (ods::inst::DrawImage*)
		draw_frame->Get(ods::Id::DrawImage);
	if (draw_image == nullptr)
		draw_image = draw_frame->NewDrawImage();
	
	QString full_path = QDir::home().filePath("company-logo.png");
	QSize sz;
	draw_image->LoadImage(full_path, sz);
	
	// Optional stuff:
	
	ods::Length xy(50, ods::Unit::Px);
	draw_frame->x(&xy); // X offset from the cell the draw_frame is bound to
	draw_frame->y(&xy); // Y offset from the cell
	
	ods::Length width(sz.width() / 2, ods::Unit::Px);
	draw_frame->width(&width);
	
	ods::Length height(sz.height() / 2, ods::Unit::Px);
	draw_frame->height(&height);
	
	auto *svg_desc = (ods::inst::SvgDesc*) draw_frame->Get(ods::Id::SvgDesc);
	if (svg_desc == nullptr)
		svg_desc = draw_frame->NewSvgDesc();
	
	svg_desc->SetString("svg description");
	
	auto *svg_title = (ods::inst::SvgTitle*) draw_frame->Get(ods::Id::SvgTitle);
	if (svg_title == nullptr)
		svg_title = draw_frame->NewSvgTitle();
	
	svg_title->SetString("svg title");
	
	util::Save(book);
}

void ReadImage()
{
	auto full_path = util::FindFile("Image.ods");
	if (full_path.isEmpty())
		return;
	
	QString err;
	auto *book = ods::Book::FromFile(full_path, &err);
	ods::AutoDelete<ods::Book*> ad(book);
	if (!err.isEmpty())
	{
		mtl_warn("%s", qPrintable(err));
		return;
	}
	
	auto *spreadsheet = book->spreadsheet();
	auto *sheet = spreadsheet->GetSheet(0);
	auto *row = sheet->GetRow(3);
	auto *cell = row->GetCell(3);
	
	auto *df = (ods::inst::DrawFrame*) cell->Get(ods::Id::DrawFrame);
	if (df == nullptr)
	{
		mtl_warn("No draw frame");
		return;
	}
	
	auto *di = (ods::inst::DrawImage*) df->Get(ods::Id::DrawImage);
	if (di == nullptr)
	{
		mtl_warn("No draw image");
		return;
	}
	
	QString from_path = book->temp_dir_path() + '/' + di->href();
	
	// from_path is the full path to the temporary image file,
	// now one can do anything: e.g. load the image or copy it somewhere,
	// let's copy it to the home dir:
	
	QString file_name = di->href();
	// file_name likely also has the subfolder name, e.g. "media/file.ext"
	// let's remove it:
	const int slash_index = file_name.lastIndexOf('/');
	if (slash_index != -1)
		file_name = file_name.mid(slash_index + 1);
	
	QString to_path = QDir::home().filePath(file_name);
	auto from = from_path.toLocal8Bit();
	auto to = to_path.toLocal8Bit();
	
	// Note: QFile::copy() will fail if a file at to_path already exists
	
	if (QFile::copy(from_path, to_path))
	{
		mtl_info("Image coppied successfully from:\n%s\nTo:\n%s",
			from.data(), to.data());
	} else {
		mtl_info("Failed to copy image from:\n%s\nTo:\n%s",
			from.data(), to.data());
	}
}

void CreateDate()
{
	ods::Book *book = ods::Book::New(ods::DevMode::Yes);
	ods::AutoDelete<ods::Book*> ad(book);
	
	auto *spr = book->spreadsheet();
	auto *sheet = spr->NewSheet("Sheet name");
	auto *row = sheet->NewRowAt(0);
	auto *cell = row->NewCellAt(0);
	
	auto *date = new QDate(QDate::currentDate());
	cell->SetDate(date);
	
	auto *style = cell->GetStyle(ods::CreateIfNeeded::Yes);
	auto *date_style = style->GetDateStyle();
	
	if (date_style == nullptr)
		date_style = style->NewDateStyle();
	
	// First clear previous formatting if any, in this
	// case there shouldn't be any since the book was
	// not loaded from a file.
	date_style->DeleteNodes();
	
	// If "long" is applied to e.g. "month" then 25/3/2018 becomes 25/03/2018
	const QString num_style = QLatin1String("long");
	
	// Display as "day.month.year hours:minutes:seconds":
	date_style->NewDay()->style(num_style);
	date_style->NewText(".");
	date_style->NewMonth()->style(num_style);
	date_style->NewText(".");
	date_style->NewYear()->style(num_style);
	
	if (cell->is_date_time()) {
		// if it's not a QDate but a QDateTime:
		date_style->NewText(" ");
		date_style->NewHours()->style(num_style);
		date_style->NewText(":");
		date_style->NewMinutes()->style(num_style);
		date_style->NewText(":");
		date_style->NewSeconds()->style(num_style);
	}
	
	util::Save(book);
}

void ReadDate()
{
	auto full_path = util::FindFile("Date.ods");
	
	if (full_path.isEmpty())
		return;
	
	QString err;
	auto *book = ods::Book::FromFile(full_path, &err);
	
	if (!err.isEmpty()) {
		auto ba = err.toLocal8Bit();
		mtl_warn("%s", ba.data());
		return;
	}
	
	ods::AutoDelete<ods::Book*> ad(book);
	auto *spreadsheet = book->spreadsheet();
	auto *sheet = spreadsheet->GetSheet(0);
	auto *row = sheet->GetRow(0);
	auto *cell = row->GetCell(0);
	
	if (cell->is_date())
	{
		QDate *dt = cell->as_date();
		auto ba = dt->toString(Qt::ISODate).toLocal8Bit();
		mtl_info("Date: %s", ba.data());
	} else if (cell->is_date_time()) {
		QDateTime *dt = cell->as_date_time();
		auto ba = dt->toString(Qt::ISODate).toLocal8Bit();
		mtl_info("DateTime: %s", ba.data());
	} else {
		mtl_warn("Cell is not a date or date-time");
		return;
	}
	
	// Now query date formatting rules,
	// should print a string like "Day.Month.Year Hours:Minutes:Seconds"
	mtl_info("Formatting:");
	
	ods::inst::StyleStyle *style = cell->GetStyle();
	
	if (style == nullptr)
	{
		mtl_warn("No cell style");
		return;
	}
	
	ods::inst::NumberDateStyle *date_style = style->GetDateStyle();
	
	if (date_style == nullptr)
	{
		mtl_warn("No date style");
		return;
	}
	
	for (ods::StringOrInst *node: *date_style->nodes())
	{
		if (!node->is_inst())
			continue; // a string, not a class, skip.
		
		auto *inst = node->as_inst(); // pointer to the base class ods::inst::Abstract
		
		// Find out what class it is:
		if (inst->Is(ods::Id::NumberYear))
		{
			printf("Year");
		} else if (inst->Is(ods::Id::NumberMonth)) {
			printf("Month");
		} else if (inst->Is(ods::Id::NumberDay)) {
			printf("Day");
		} else if (inst->Is(ods::Id::NumberHours)) {
			printf("Hours");
		} else if (inst->Is(ods::Id::NumberMinutes)) {
			printf("Minutes");
		} else if (inst->Is(ods::Id::NumberSeconds)) {
			printf("Seconds");
		} else if (inst->Is(ods::Id::NumberText)) {
			auto *t = (ods::inst::NumberText*) inst;
			const QString *s = t->GetString();
			
			if (s == nullptr)
			{
				printf("[no text]");
			} else {
				auto ba = s->toLocal8Bit();
				printf("%s", ba.data());
			}
		} else {
			printf("Unknown date format");
		}
	}
	
	printf("\n");
}

void CreateTime()
{
	auto *book = ods::Book::New();
	ods::AutoDelete<ods::Book*> ad(book);
	
	auto *spr = book->spreadsheet();
	auto *sheet = spr->NewSheet("Sheet name");
	auto *row = sheet->NewRowAt(0);
	auto *cell = row->NewCellAt(0);
	
	// 9h 32m 40s
	cell->SetTime(new ods::Time(9, 32, 40));
	
	// Now set up formatting styles:
	auto *style = cell->GetStyle();
	
	if (style == nullptr)
		style = cell->NewStyle();
	
	ods::inst::NumberTimeStyle *time_style = style->GetTimeStyle();
	
	if (time_style == nullptr)
		time_style = style->NewTimeStyle();
	
	// clear previous formatting if any
	time_style->DeleteNodes();
	
	const QString param = QLatin1String("long");
	
	// set formatting as HH:MM::SS
	time_style->NewHours()->style(param);
	time_style->NewText(":");
	time_style->NewMinutes()->style(param);
	time_style->NewText(":");
	time_style->NewSeconds()->style(param);
	
	util::Save(book);
}

void ReadTime()
{
	auto full_path = util::FindFile("Time.ods");
	
	if (full_path.isEmpty())
		return;
	
	QString err;
	auto *book = ods::Book::FromFile(full_path, &err);
	
	if (!err.isEmpty()) {
		auto ba = err.toLocal8Bit();
		mtl_warn("%s", ba.data());
		return;
	}
	
	ods::AutoDelete<ods::Book*> ad(book);
	auto *spreadsheet = book->spreadsheet();
	auto *sheet = spreadsheet->GetSheet(0);
	auto *row = sheet->GetRow(0);
	
	for (int i = 0; i < 2; i++) {
		auto *cell = row->GetCell(i);
		if (cell == nullptr) {
			mtl_trace("Cell at: %d = null", i);
			continue;
		}
		
		if (!cell->is_time())
		{
			mtl_warn("Cell at %d type is not \"time\"", i);
			return;
		}
		
		ods::Time *t = cell->as_time();
		auto ba = t->toString().toLocal8Bit();
		mtl_info("Time: %s", ba.data());
	}
	
	// Now query time formatting rules,
	// should print a string like "Hours:Minutes:Seconds"
	printf("Formatting: ");
	
	ods::inst::StyleStyle *style = row->GetCell(0)->GetStyle();
	
	if (style == nullptr)
	{
		mtl_warn("No cell style");
		return;
	}
	
	ods::inst::NumberTimeStyle *time_style = style->GetTimeStyle();
	
	if (time_style == nullptr)
	{
		mtl_warn("No time style");
		return;
	}
	
	for (ods::StringOrInst *node: *time_style->nodes())
	{
		if (!node->is_inst())
			continue; // a string, not a class, skip.
		
		auto *inst = node->as_inst(); // pointer to the abstract class ods::inst::Abstract
		
		// Find out what class it is:
		if (inst->Is(ods::Id::NumberYear))
		{
			printf("Year");
		} else if (inst->Is(ods::Id::NumberMonth)) {
			printf("Month");
		} else if (inst->Is(ods::Id::NumberDay)) {
			printf("Day");
		} else if (inst->Is(ods::Id::NumberHours)) {
			printf("Hours");
		} else if (inst->Is(ods::Id::NumberMinutes)) {
			printf("Minutes");
		} else if (inst->Is(ods::Id::NumberSeconds)) {
			printf("Seconds");
		} else if (inst->Is(ods::Id::NumberText)) {
			auto *t = (ods::inst::NumberText*) inst;
			const QString *s = t->GetString();
			
			if (s == nullptr)
			{
				printf("[no text]");
			} else {
				auto ba = s->toLocal8Bit();
				printf("%s", ba.data());
			}
		} else {
			printf("Unknown time format");
		}
	}
	
	printf("\n");
}

void CreateBoolean()
{
	auto *book = ods::Book::New();
	ods::AutoDelete<ods::Book*> ad(book);
	
	auto *spr = book->spreadsheet();
	auto *sheet = spr->NewSheet("Sheet name");
	auto *row = sheet->NewRowAt(0);
	
	auto *cell = row->NewCellAt(0);
	cell->SetBoolean(true);
	
	cell = row->NewCellAt(1);
	cell->SetBoolean(false);
	
	// Next is formatting/styling which isn't necessary unless you know
	// what you're doing.
	
	auto *style = cell->GetStyle();
	
	if (style == nullptr)
		style = cell->NewStyle();
	
	auto *boolean_style = style->GetBooleanStyle();
	
	if (boolean_style == nullptr)
		boolean_style = style->NewBooleanStyle();
	
	// clear formatting if any
	boolean_style->DeleteNodes();
	
	boolean_style->NewText("[");
	boolean_style->NewBoolean();
	boolean_style->NewText("]");
	
	util::Save(book);
}

void ReadBoolean()
{
	auto full_path = util::FindFile("Boolean.ods");
	
	if (full_path.isEmpty())
		return;
	
	QString err;
	auto *book = ods::Book::FromFile(full_path, &err);
	
	if (!err.isEmpty()) {
		auto ba = err.toLocal8Bit();
		mtl_warn("%s", ba.data());
		return;
	}
	
	ods::AutoDelete<ods::Book*> ad(book);
	auto *spreadsheet = book->spreadsheet();
	auto *sheet = spreadsheet->GetSheet(0);
	auto *row = sheet->GetRow(0);
	
	for (int i = 0; i < 3; i++)
	{
		auto *cell = row->GetCell(i);
		
		if (cell->is_boolean())
		{
			bool yes = *cell->as_boolean();
			const char *s = yes ? "true" : "false";
			mtl_info("Cell boolean value: %s", s);
		} else {
			mtl_warn("Cell type not boolean");
		}
	}
}

void TestReportedBug()
{
mtl_trace();
	auto *book = ods::Book::New();
	ods::AutoDelete<ods::Book*> ad(book);
	auto *spreadsheet = book->spreadsheet();
	auto *sheet = spreadsheet->NewSheet("Sheet name"); // creating new sheet
	auto *row = sheet->NewRowAt(0);
	auto *cell = row->NewCellAt(0);
	
	cell->SetString("first cell");
	cell->number_rows_spanned(3); // code which doesn't work
	
	QFile file(QDir::home().filePath("bug.ods"));
	book->Save(file, nullptr);
mtl_trace();
}
