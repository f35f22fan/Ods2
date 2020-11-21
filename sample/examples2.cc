#include "examples2.hh"

#include "util.hh"
#include <ods/ods>
#include <float.h>

void
CreateCurrency()
{
	auto *book = ods::Book::New();
	ods::AutoDelete<ods::Book*> ad(book);
	auto *spreadsheet = book->spreadsheet();
	auto *sheet = spreadsheet->NewSheet("Sheet name");
	auto *row = sheet->NewRowAt(0);
	auto *cell = row->NewCellAt(0);
	
	ods::Currency usd = ods::currency::USD(49.2);
	cell->SetCurrency(usd);
	
	// other currency info and formatting rules:
	ods::inst::StyleStyle *style = cell->FetchStyle();
	ods::inst::NumberCurrencyStyle *ncs = style->FetchNumberCurrencyStyle();
	auto *cs = ncs->FetchCurrencySymbol();
	
	cs->language(ods::lang::English.str);
	cs->country(ods::country::USA);
	cs->SetSymbol(usd);
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
	
	ods::AutoDelete<ods::Book*> ad(book);
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
	
	mtl_info("currency value: %f", *cell->as_currency());
	
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
		mtl_info("language: %s", lang.data());
		
		auto country = cs->country().toLocal8Bit();
		mtl_info("country: %s", country.data());
		
		auto symbol = cs->GetSymbol().toLocal8Bit();
		mtl_info("symbol: %s", symbol.data());
	} else {
		mtl_warn("no currency symbol");
	}
	
	auto *nn = (ods::inst::NumberNumber*) ncs->Get(ods::Id::NumberNumber);
	
	if (nn != nullptr)
	{
		mtl_info("grouping: %d, min_integer_digits: %d, decimal_places: %d",
		nn->grouping(), nn->min_integer_digits(), nn->decimal_places());
	} else {
		mtl_info("no nn");
	}
	
	util::Save(book);
}

void
CreateFormula()
{
	auto *book = ods::Book::New();
	ods::AutoDelete<ods::Book*> ad(book);
	auto *spreadsheet = book->spreadsheet();
	auto *sheet = spreadsheet->NewSheet("Sheet name");
	
	{ // Formula = "(3.0 - 2.0) * 1.5"
		auto *row = sheet->NewRowAt(0);
		auto *cell0 = row->NewCellAt(0);
		cell0->SetDouble(5.0);
		
		auto *cell1 = row->NewCellAt(1);
		cell1->SetDouble(4.0);
		auto *cell2 = row->NewCellAt(2);
		
		ods::Formula *formula = cell2->NewFormula();
		formula->AddOpenBrace();
		formula->Add(3.0);
		formula->Add(ods::Op::Minus);
		formula->Add(2.0);
		formula->AddCloseBrace();
		formula->Add(ods::Op::Multiply);
		formula->Add(1.5);
		
		ods::FormulaNode *result = formula->Eval();
		
		if (result == nullptr) {
			mtl_info("Formula evaluation failed");
		} else {
			auto ba = result->toString().toLocal8Bit();
			mtl_info("Formula evaluates to: %s", ba.data());
		}
	}
	
	{ // Formula = "Sum(A2:F2) * ('Sheet 2'.B2 - 40)"
	  // where A2:F2 => cell range [A2, B2, C2, D2, E2, F2]
		auto *row = sheet->NewRowAt(1);
		for (int i = 0; i < 6; i++) {
			auto *cell = row->NewCellAt(i);
			cell->SetDouble(double(i));
		}
		
		auto *start_cell = row->GetCell(0);
		auto *end_cell = row->GetCell(5);
		auto *sheet2 = spreadsheet->NewSheet("Sheet 2");
		auto *sheet2_row2 = sheet2->NewRowAt(1);
		auto *sheet2_cell = sheet2_row2->NewCellAt(1);
		sheet2_cell->SetDouble(45.0);
		
		auto *formula_cell = row->NewCellAt(7);
		ods::Formula *formula2 = formula_cell->NewFormula();
		auto *function = ods::Function::New(ods::FunctionId::Sum);
		auto *cell_range = sheet->NewAddress(start_cell, end_cell);
		function->AddArg(cell_range);
		formula2->Add(function);
		formula2->Add(ods::Op::Multiply);
		formula2->AddOpenBrace();
		formula2->Add(sheet2_cell);
		formula2->Add(ods::Op::Minus);
		formula2->Add(40.0);
		formula2->AddCloseBrace();
		
		// One can print its nodes to the terminal, like for debugging purposes:
		formula2->PrintNodes("Formula2 nodes are");
		
		auto *result = formula2->Eval();
		
		if (result == nullptr) {
			mtl_info("formula2 eval failed");
			return;
		} else {
			auto ba = result->toString().toLocal8Bit();
			mtl_info("Formula2 value: %s", ba.data());
		}
	}
	
	{ // use the PRODUCT() function
		auto *row3 = sheet->NewRowAt(2);
		QVector<ods::Cell*> cells;
		
		for (int i = 0; i < 3; i++) {
			auto *cell = row3->NewCellAt(i);
			cell->SetDouble(i + 2);
			cells.append(cell);
		}
		
		auto *formula_cell = row3->NewCellAt(cells.size());
		auto *formula = formula_cell->NewFormula();
		auto *function = ods::Function::New(ods::FunctionId::Product);
		ods::Cell *start_cell = cells[0];
		ods::Cell *end_cell = cells[cells.size() - 1];
		auto *cell_range = sheet->NewAddress(start_cell, end_cell);
		function->AddArg(cell_range);
		formula->Add(function);
		auto *node = formula->Eval();
		CHECK_PTR_VOID(node);
		auto ba = node->toString().toLocal8Bit();
		mtl_info("PRODUCT() formula value: %s", ba.data());
	}
	
	{ // concatenate strings
		auto *row = sheet->NewRowAt(3);
		auto *cell = row->NewCellAt(0);
		cell->SetString("Hello ");
		auto *cell2 = row->NewCellAt(1);
		cell2->SetString("World");
		auto *formula_cell = row->NewCellAt(2);
		auto *formula = formula_cell->NewFormula();
		formula->Add(cell);
		formula->Add(ods::Op::Ampersand); // concatenates strings
		formula->Add(cell2);
		formula->Add(ods::Op::Ampersand); // concatenate some more
		formula->Add(new QString("!!"));
		
		auto *result = formula->Eval();
		CHECK_PTR_VOID(result);
		
		auto ba = result->toString().toLocal8Bit();
		mtl_info("Formula with strings: \"%s\"", ba.data());
	}
	
	
	util::Save(book);
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
	
	ods::AutoDelete<ods::Book*> ad(book);
	auto *spreadsheet = book->spreadsheet();
	auto *sheet = spreadsheet->GetSheet(0);
	int row_index = 2;
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
	
	ods::Formula *formula = cell->formula();
	const ods::FormulaNode *value = formula->Eval();
	if (value == nullptr || formula->has_error()) {
		if (value == nullptr)
			mtl_info("value = nullptr");
		else {
			auto ba = formula->error().toLocal8Bit();
			mtl_warn("Formula error: %s", ba.data());
		}
		return;
	}
	
	if (value->is_double()) {
		double num = value->as_double();
		mtl_info("Formula value: %.2f", num);
	} else if (value->is_none()) {
		mtl_warn("Value not set");
	} else {
		auto ba = value->toString().toLocal8Bit();
		mtl_warn("Unknown formula value type: %s", ba.data());
	}
	
	{
		auto *row = sheet->GetRow(++row_index);
		auto *cell = row->GetCell(0);
		CHECK_TRUE_VOID((cell->has_formula()));
		ods::Formula *f = cell->formula();
		auto *node = f->Eval();
		auto ba = node->toString().toLocal8Bit();
		mtl_info("NOW() is: %s", ba.data());
	}
	
	util::Save(book);
}

void
CreateFormulaFunctions()
{
	auto *book = ods::Book::New();
	ods::AutoDelete<ods::Book*> ad(book);
	auto *spreadsheet = book->spreadsheet();
	auto *sheet = spreadsheet->NewSheet("Formula Functions");
	int last_row = 0;
	{ // CONCATENATE()
// Summary: Concatenate the text strings
// Semantics: Concatenate each text value, in order, into a
// single text result.
		auto *row = sheet->NewRowAt(last_row++);
		auto *cell = row->NewCellAt(0);
		cell->SetString("Hello");
		auto *cell2 = row->NewCellAt(1);
		cell2->SetString("World");
		auto *formula_cell = row->NewCellAt(2);
		ods::Formula *formula = formula_cell->NewFormula();
		ods::Function *concatenate = ods::Function::New(ods::FunctionId::Concatenate);
		concatenate->AddArg(sheet->NewAddress(cell));
		concatenate->AddArg(new QString(" "));
		concatenate->AddArg(sheet->NewAddress(cell2));
		concatenate->AddArg(new QString("!"));
		formula->Add(concatenate);
		
		ods::FormulaNode *result = formula->Eval();
		CHECK_PTR_VOID(result);
		auto ba = result->toString().toLocal8Bit();
		mtl_info("CONCATENATE(): \"%s\"", ba.data());
	}
	{ // DATE(), NOW()
		auto *row = sheet->NewRowAt(last_row++);
		auto *cell = row->NewCellAt(0);
		auto *formula = cell->NewFormula();
		auto *fn = ods::Function::New(ods::FunctionId::Now);
		formula->Add(fn);
		ods::FormulaNode *node = formula->Eval();
		
		if (node->is_date_time()) {
			QDateTime *dt = node->as_date_time();
			auto ba = dt->toString().toLocal8Bit();
			mtl_info("NOW(): %s", ba.data());
		}
		
		cell = row->NewCellAt(1);
		formula = cell->NewFormula();
		fn = ods::Function::New(ods::FunctionId::Date);
		// y = 1969, m = 5, d = 30
		fn->AddArg(double(1969));
		fn->AddArg(double(5));
		fn->AddArg(double(30));
		formula->Add(fn);
		node = formula->Eval();
		CHECK_PTR_VOID(node);
		
		if (node->is_date()) {
			QDate *date = node->as_date();
			auto ba = date->toString().toLocal8Bit();
			mtl_info("DATE(): %s", ba.data());
		} else {
			mtl_warn("Node is not a date!");
		}
	}
	
	// Enclosed in multiple if(true/false){} blocks to selectively disable execution
	// while still compiling - for the purpose of easing development:
	if (false) { // SUM()
		auto *row = sheet->NewRowAt(last_row++);
		auto *cell0 = row->NewCellAt(0);
		cell0->SetDouble(28);
		auto *cell = row->NewCellAt(1);
		auto *formula = cell->NewFormula();
		auto *fn = ods::Function::New(ods::FunctionId::Sum);
		fn->AddArg(sheet->NewAddress(cell0));
		fn->AddArg(25);
		fn->AddArg(20);
		formula->Add(fn);
		ods::FormulaNode *node = formula->Eval();
		CHECK_PTR_VOID(node);
		auto ba = node->toString().toLocal8Bit();
		mtl_info("SUM(): %s", ba.data());
	}
	
	if (false) { // QUOTIENT(), MOD(), POWER()
		auto *row = sheet->NewRowAt(last_row++);
		auto *lhs = row->NewCellAt(0);
		lhs->SetDouble(-11.7);
		int last_col = 1;
		auto *rhs = row->NewCellAt(last_col++);
		rhs->SetDouble(3);
		auto *f_cell = row->NewCellAt(last_col++);
		auto *f = f_cell->NewFormula();
		auto *fn = ods::Function::New(ods::FunctionId::Quotient);
		fn->AddArg(sheet->NewAddress(lhs));
		fn->AddArg(sheet->NewAddress(rhs));
		f->Add(fn);
		auto *node = f->Eval();
		if (node == nullptr || !node->is_any_double()) {
			mtl_warn("QUOTIENT() failed");
		} else {
			i64 result = i64(node->as_any_double());
			mtl_info("QUOTIENT(): %ld", result);
		}
		
		last_col++;
		lhs = row->NewCellAt(last_col++);
		lhs->SetDouble(7.5);
		f_cell = row->NewCellAt(last_col++);
		f = f_cell->NewFormula();
		fn = ods::Function::New(ods::FunctionId::Mod);
		fn->AddArg(sheet->NewAddress(lhs));
		fn->AddArg(2.1);
		f->Add(fn);
		node = f->Eval();
		if (node == nullptr || !node->is_any_double()) {
			mtl_warn("MOD() failed");
		} else {
			double result = node->as_any_double();
			mtl_info("MOD(): %.2f", result);
		}
		
		last_col++;
		lhs = row->NewCellAt(last_col++);
		lhs->SetDouble(3.4);
		f_cell = row->NewCellAt(last_col++);
		f = f_cell->NewFormula();
		fn = ods::Function::New(ods::FunctionId::Power);
		f->Add(fn);
		fn->AddArg(sheet->NewAddress(lhs));
		fn->AddArg(4.4);
		node = f->Eval();
		if (node == nullptr || !node->is_any_double()) {
			mtl_warn("POWER() failed");
		} else {
			double result = node->as_any_double();
			mtl_info("POWER(): %.4f", result);
		}
	}
	
	if (true) { // IF()
		auto *row = sheet->NewRowAt(last_row++);
		CHECK_PTR_VOID(row);
		
		{ // use IF() with numbers:
			auto *f_cell = row->NewCellAt(3);
			CHECK_PTR_VOID(f_cell);
			auto *f = f_cell->NewFormula();
			auto *fn_if = ods::Function::New(ods::FunctionId::If);
			f->Add(fn_if);
			
			auto *cell0 = row->NewCellAt(0);
			cell0->SetDouble(25.0);
			auto *condition = new QVector<ods::FormulaNode*>();
			ods::Address *a = sheet->NewAddress(cell0);
			condition->append(ods::FormulaNode::Address(a));
			condition->append(ods::FormulaNode::Op(ods::Op::GreaterOrEqual));
			condition->append(ods::FormulaNode::Double(98));
			fn_if->AddArg(condition);
	
			auto *true_option = ods::FormulaNode::String(new QString("true!"));
			fn_if->AddArg(true_option);
			auto *false_option = ods::Function::New(ods::FunctionId::Sum);
			false_option->AddArg(49);
			false_option->AddArg(15);
			fn_if->AddArg(false_option);
			
			auto *node = f->Eval();
			
			if (node == nullptr) {
				mtl_warn("IF(numbers) failed");
			} else {
				auto ba = node->toString().toLocal8Bit();
				mtl_info("IF(numbers): %s", ba.data());
			}
		}
		
		{// Now use IF() with dates:
			int next_cell = 3;
			
			auto *cell1 = row->NewCellAt(next_cell++);
			cell1->SetDate(new QDate(1960, 7, 15));
			auto *cell2 = row->NewCellAt(next_cell++);
			cell2->SetDate(new QDate(1980, 1, 2));
			
			auto *f_cell = row->NewCellAt(next_cell++);
			CHECK_PTR_VOID(f_cell);
			auto *f = f_cell->NewFormula();
			auto *fn_if = ods::Function::New(ods::FunctionId::If);
			f->Add(fn_if);
			
			auto *condition = new QVector<ods::FormulaNode*>();
			auto *lhs = ods::FormulaNode::Address(sheet->NewAddress(cell1));
			condition->append(lhs);
			condition->append(ods::FormulaNode::Op(ods::Op::GreaterOrEqual));
			auto *rhs = ods::FormulaNode::Address(sheet->NewAddress(cell2));
			condition->append(rhs);
			fn_if->AddArg(condition);
			
			auto *true_option = ods::FormulaNode::String(new QString("true!"));
			fn_if->AddArg(true_option);
			auto *false_option = ods::FormulaNode::String(new QString("false!"));
			fn_if->AddArg(false_option);
			
			auto *node = f->Eval();
			
			if (node == nullptr) {
				mtl_warn("IF(dates) failed");
			} else {
				auto ba = node->toString().toLocal8Bit();
				mtl_info("IF(dates): %s", ba.data());
			}
		}
	}
	
	if (true) { // COUNT()
		auto *row = sheet->NewRowAt(last_row++);
		CHECK_PTR_VOID(row);
		int next_col = 0;
		auto *num_cell = row->NewCellAt(next_col++);
		num_cell->SetDouble(14);
		auto *str_cell = row->NewCellAt(next_col++);
		str_cell->SetString(QString("foo"));
		auto *empty_cell = row->NewCellAt(next_col++);
		
		{
			auto *fcell = row->NewCellAt(next_col++);
			auto *f = fcell->NewFormula();
			auto *fn = ods::Function::New(ods::FunctionId::Count);
			f->Add(fn);
			fn->AddArg(43.2);
			fn->AddArg(new QString("hi"));
			fn->AddArg(sheet->NewAddress(num_cell));
			fn->AddArg(sheet->NewAddress(str_cell));
			
			auto *node = f->Eval();
			if (node == nullptr || !node->is_double()) {
				mtl_info("COUNT() eval failed");
			} else {
				double d = node->as_double();
				mtl_info("COUNT(): %.0f", d);
			}
		}
		
		{
			auto *fcell = row->NewCellAt(next_col++);
			auto *f = fcell->NewFormula();
			auto *fn = ods::Function::New(ods::FunctionId::CountA);
			f->Add(fn);
			fn->AddArg(43.2);
			fn->AddArg(new QString("hi"));
			fn->AddArg(sheet->NewAddress(num_cell));
			fn->AddArg(sheet->NewAddress(empty_cell));
			fn->AddArg(sheet->NewAddress(str_cell));
			
			auto *node = f->Eval();
			if (node == nullptr || !node->is_double()) {
				mtl_info("COUNTA() eval failed");
			} else {
				double d = node->as_double();
				mtl_info("COUNTA(): %.0f", d);
			}
		}
		
		
	}
	
	util::Save(book);
}

void
ReadFormulaCustom()
{
	auto full_path = util::FindFile("FormulaCustom2.ods");
	
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
	CHECK_PTR_VOID(row);
	auto *cell = row->GetCell(0);
	CHECK_PTR_VOID(cell);
	
	if (!cell->has_formula()) {
		mtl_warn("The cell has no formula");
		return;
	}
	
	ods::Formula *formula = cell->formula();
	const ods::FormulaNode *node = formula->Eval();
	
	if (node == nullptr || formula->has_error()) {
		if (node == nullptr)
			mtl_info("value = nullptr");
		else {
			auto ba = formula->error().toLocal8Bit();
			mtl_warn("Formula error: %s", ba.data());
		}
		return;
	}
	
	auto ba = node->toString().toLocal8Bit();
	if (node->is_string()) {
		auto ba = node->as_string()->toLocal8Bit();
		mtl_info("Node is string: %s", ba.data());
	} else {
		auto ba = node->toString().toLocal8Bit();
		mtl_info("node is not string: %s", ba.data());
	}
	mtl_info("value: \"%s\"", ba.data());
	
	util::Save(book);
}

