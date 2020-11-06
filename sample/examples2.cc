#include "examples2.hh"

#include "util.hh"
#include <ods/ods>
#include <ods/op.hh>

void
CreateCurrency()
{
	auto *book = ods::Book::New();
	util::AutoDelete<ods::Book*> ad(book);
	auto *spreadsheet = book->spreadsheet();
	auto *sheet = spreadsheet->NewSheet("Sheet name");
	auto *row = sheet->NewRowAt(0);
	auto *cell = row->NewCellAt(0);
	
	cell->SetCurrency(49.2, ods::currency::USD);
	
	// other currency info and formatting rules:
	ods::inst::StyleStyle *style = cell->FetchStyle();
	ods::inst::NumberCurrencyStyle *ncs = style->FetchNumberCurrencyStyle();
	auto *cs = ncs->FetchCurrencySymbol();
	
	cs->language(ods::lang::English.str);
	cs->country(ods::country::USA);
	cs->SetSymbol(ods::currency::USD);
	auto *nn = ncs->FetchNumber();
	{
		nn->grouping(1);
		nn->min_integer_digits(5);
		nn->decimal_places(3);
		// This makes 49.2 to be displayed as "$00,049.200"
		// If nn->grouping(0) then "$00049.200" will be displayed.
		// -1 means value not set.
		// Note: Calligra Sheets doesn't display currency formatting properly
	}
	
	util::Save(book);
}

void
ReadCurrency()
{
	auto full_path = util::FindFile("Currency.ods");
	
	if (full_path.isEmpty())
		return;
	
	QString err;
	auto *book = ods::Book::FromFile(full_path, &err);
	
	if (!err.isEmpty()) {
		auto ba = err.toLocal8Bit();
		mtl_warn("%s", ba.data());
		return;
	}
	
	util::AutoDelete<ods::Book*> ad(book);
	auto *spreadsheet = book->spreadsheet();
	auto *sheet = spreadsheet->GetSheet(0);
	auto *row = sheet->GetRow(2);
	
	if (row == nullptr)
	{
		mtl_warn();
		return;
	}
	
	auto *cell = row->GetCell(3);
	
	if (cell == nullptr)
	{
		mtl_warn();
		return;
	}
	
	if (!cell->is_currency())
	{
		mtl_warn("Not currency");
		return;
	}
	
	mtl_line("currency value: %f", *cell->as_currency());
	
	auto *style = cell->GetStyle();
	
	if (style == nullptr)
	{
		mtl_warn("no style");
		return;
	}
	
	auto *ncs = style->GetCurrencyStyle();
	
	if (ncs == nullptr)
	{
		mtl_warn("no ncs");
		return;
	}
	
	auto *cs = (ods::inst::NumberCurrencySymbol*)
		ncs->Get(ods::Id::NumberCurrencySymbol);
	
	if (cs != nullptr)
	{
		auto lang = cs->language().toLocal8Bit();
		mtl_line("language: %s", lang.data());
		
		auto country = cs->country().toLocal8Bit();
		mtl_line("country: %s", country.data());
		
		auto symbol = cs->GetSymbol().toLocal8Bit();
		mtl_line("symbol: %s", symbol.data());
	} else {
		mtl_warn("no currency symbol");
	}
	
	auto *nn = (ods::inst::NumberNumber*) ncs->Get(ods::Id::NumberNumber);
	
	if (nn != nullptr)
	{
		mtl_line("grouping: %d, min_integer_digits: %d, decimal_places: %d",
		nn->grouping(), nn->min_integer_digits(), nn->decimal_places());
	} else {
		mtl_line("no nn");
	}
	
	util::Save(book);
}

void
CreateFormula()
{
	mtl_warn("TBD");
	/*
	auto *book = ods::Book::New();
	util::AutoDelete<ods::Book*> ad(book);
	auto *spreadsheet = book->spreadsheet();
	auto *sheet = spreadsheet->NewSheet("Sheet name");
	auto *row = sheet->NewRowAt(0);
	auto *cell0 = row->NewCellAt(0);
	cell0->SetDouble(5.0);
	
	auto *cell1 = row->NewCellAt(1);
	cell1->SetDouble(4.0);
	
	auto *cell2 = row->NewCellAt(2);
	ods::Formula *formula = cell2->NewFormula();
	formula->Add(ods::Parenthesis::Open);
	formula->Add(3.0);
	formula->Add(ods::Op::Subtract);
	formula->Add(2.0);
	formula->Add(ods::Parenthesis::Close);
	formula->Add(ods::Op::Multiply);
	formula->Add(1.5);
	// = (3.0 - 2.0) * 1.5
	ods::formula::Value result;
	formula->Eval(result);
	
	util::Save(book);
	*/
}

void
ReadFormula()
{
	auto full_path = util::FindFile("Formula.ods");
	
	if (full_path.isEmpty())
		return;
	
	QString err;
	auto *book = ods::Book::FromFile(full_path, &err);
	
	if (!err.isEmpty()) {
		auto ba = err.toLocal8Bit();
		mtl_warn("%s", ba.data());
		return;
	}
	
	util::AutoDelete<ods::Book*> ad(book);
	auto *spreadsheet = book->spreadsheet();
	auto *sheet = spreadsheet->GetSheet(0);
	const int row_index = 2;
	auto *row = sheet->GetRow(row_index);
	
	if (row == nullptr)
	{
		mtl_warn("No row at %d", row_index);
		return;
	}
	
	const int col = 0;
	auto *cell = row->GetCell(col);
	
	if (cell == nullptr) {
		mtl_warn("No cell at %d", col);
		return;
	}
	
	if (!cell->has_formula()) {
		mtl_warn("The cell has no formula");
		return;
	}
	
	ods::Formula *f = cell->formula();
	const ods::Value &value = f->Eval();
	
	if (value.has_error()) {
		auto ba = value.error().toLocal8Bit();
		mtl_warn("Formula error: %s", ba.data());
		return;
	}
	
	if (value.is_double()) {
		double num = *value.as_double();
		mtl_line("Formula value: %.2f", num);
	} else if (value.is_none()) {
		mtl_warn("Value not set");
	} else {
		mtl_warn("Formula value of a non-double type");
	}
}

void
CreateCalc() {
	auto *book = ods::Book::New();
	util::AutoDelete<ods::Book*> ad(book);
	auto *spreadsheet = book->spreadsheet();
	auto *sheet = spreadsheet->NewSheet("Sheet name");
	auto *row = sheet->NewRowAt(0);
	auto *cell0 = row->NewCellAt(0);
	cell0->SetDouble(5.0);
	
	auto *cell1 = row->NewCellAt(1);
	cell1->SetDouble(4.0);
	
	auto *result_cell = row->NewCellAt(2);
	QString form_str = "of:=SUM([.B1];20.9;[.B2];[.C1:.C2];MIN([.A1];5))";
	//"of:=(3.0 - 2.0)*4 + [.A2]*SUM([.A1:.B1])";
	
	ods::Formula *f = ods::Formula::FromString(form_str, result_cell);
	f->Eval();
	const ods::Value &value = f->value();
	
	if (value.ok()) {
		if (value.is_double()) {
			mtl_line("Double: %.3f", *value.as_double());
		} else {
			mtl_line("Some other value");
		}
	} else if (value.has_error()) {
		auto ba = value.error().toLocal8Bit();
		mtl_line("An error occured: %s", ba.data());
	} else if (value.is_none()) {
		mtl_line("Value is none");
	} else {
		it_happened();
	}
	if (true)
		return;
	
	
	ods::Formula *formula = result_cell->NewFormula();
	formula->AddOpenBrace();
	formula->Add(3.0);
	formula->Add(ods::Op::Minus);
	formula->Add(2.0);
	formula->AddCloseBrace();
	formula->Add(ods::Op::Multiply);
	formula->Add(1.5);
	
	auto ba = formula->raw_str().toLocal8Bit();
	mtl_line("%s", ba.data());
//	// = (3.0 - 2.0) * 1.5
//	ods::formula::Value result;
//	calc->Eval(result);
	
	//util::Save(book);
}
