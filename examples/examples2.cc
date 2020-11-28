#include "examples2.hh"

#include "util.hh"
#include <ods/ods>
#include <float.h>

#include <QFile>

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
		auto *function = formula2->Add(ods::FunctionId::Sum);
		auto *cell_range = sheet->NewReference(start_cell, end_cell);
		function->AddArg(cell_range);
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
		auto *function = formula->Add(ods::FunctionId::Product);
		ods::Cell *start_cell = cells[0];
		ods::Cell *end_cell = cells[cells.size() - 1];
		auto *cell_range = sheet->NewReference(start_cell, end_cell);
		function->AddArg(cell_range);
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
	ods::inst::OfficeSpreadsheet *spreadsheet = book->spreadsheet();
	ods::Sheet *sheet = spreadsheet->NewSheet("Formula Functions");
	int last_row = 0;
	
	// Enclosed in multiple if(true/false){} blocks to selectively disable execution
	// while still compiling - for the purpose of easing development:
	
	if (false) { // CONCATENATE()
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
		ods::Function *concatenate = formula->Add(ods::FunctionId::Concatenate);
		concatenate->AddArg(sheet->NewReference(cell));
		concatenate->AddArg(new QString(" "));
		concatenate->AddArg(sheet->NewReference(cell2));
		concatenate->AddArg(new QString("!"));
		
		ods::FormulaNode *result = formula->Eval();
		CHECK_PTR_VOID(result);
		auto ba = result->toString().toLocal8Bit();
		mtl_info("CONCATENATE(): \"%s\"", ba.data());
	}
	
	if (false) { // NOW(), TODAY(), DATE()
		auto *row = sheet->NewRowAt(last_row++);
		int col = 0;
		
		{ // NOW():
			auto *cell = row->NewCellAt(col++);
			auto *formula = cell->NewFormula();
			formula->Add(ods::FunctionId::Now);
			ods::FormulaNode *node = formula->Eval();
			
			if (node == nullptr) {
				mtl_info("NOW() Failed");
			} else {
				QDateTime *dt = node->as_date_time();
				auto ba = dt->toString().toLocal8Bit();
				mtl_info("NOW(): %s", ba.data());
			}
		}
		
		{ // TODAY():
			auto *cell = row->NewCellAt(col++);
			auto *formula = cell->NewFormula();
			formula->Add(ods::FunctionId::Today);
			ods::FormulaNode *node = formula->Eval();
			
			if (node == nullptr) {
				mtl_info("TODAY() Failed");
			} else {
				QDate *dt = node->as_date();
				auto ba = dt->toString().toLocal8Bit();
				mtl_info("TODAY(): %s", ba.data());
			}
		}
		
		{ // DATE():
			auto *cell = row->NewCellAt(col++);
			auto *formula = cell->NewFormula();
			auto *fn = formula->Add(ods::FunctionId::Date);
			// y = 1969, m = 5, d = 30
			fn->AddArg(double(1969));
			fn->AddArg(double(5));
			fn->AddArg(double(30));
			auto *node = formula->Eval();
			CHECK_PTR_VOID(node);
			
			if (node->is_date()) {
				QDate *date = node->as_date();
				auto ba = date->toString().toLocal8Bit();
				mtl_info("DATE(): %s", ba.data());
			} else {
				mtl_warn("Node is not a date!");
			}
		}
	}
	
	if (false) { // SUM(), ROUND(), ROUNDDOWN(), ROUNDUP()
		auto *row = sheet->NewRowAt(last_row++);
		int col = 0;
		
		for (; col < 4; col++) {
			double val = (col == 0) ? 3.141592653 : (col + 2);
			row->NewCellAt(col)->SetDouble(val);
		}
		
		if (true) { // SUM():
			auto *cell = row->NewCellAt(col++);
			auto *formula = cell->NewFormula();
			auto *fn = formula->Add(ods::FunctionId::Sum);
			fn->AddArg(sheet->NewReference(row->GetCell(0)));
			fn->AddArg(25);
			fn->AddArg(20);
			ods::FormulaNode *node = formula->Eval();
			
			if (node == nullptr) {
				mtl_info("SUM() Failed");
			} else {
				auto ba = node->toString().toLocal8Bit();
				mtl_info("SUM(): %s", ba.data());
			}
		}
		
		const double RoundNum = 3.55;//3.1415926535897;
		const int places = 1;
		
		if (true) { // ROUND():
			auto *cell = row->NewCellAt(col++);
			auto *formula = cell->NewFormula();
			auto *fn = formula->Add(ods::FunctionId::Round);
			// number or expression resulting in a number:
			//ods::Reference *a1 = sheet->NewReference(row->GetCell(0));
			fn->AddArg(RoundNum);
			// number or expression resulting in the number of places to round to:
			fn->AddArg(places); // =ROUND(5555;-1) = 5560
			
			auto *node = formula->Eval();
			if (node == nullptr) {
				mtl_info("ROUND() Failed");
			} else {
				double num = node->as_double();
				mtl_info("ROUND(): %.6f", num);
			}
		}
		
		if (true) { // ROUNDDOWN():
			auto *cell = row->NewCellAt(col++);
			auto *formula = cell->NewFormula();
			auto *fn = formula->Add(ods::FunctionId::RoundDown);
			fn->AddArg(RoundNum);
			fn->AddArg(places);
			
			auto *node = formula->Eval();
			if (node == nullptr) {
				mtl_info("ROUNDDOWN() Failed");
			} else {
				double num = node->as_double();
				mtl_info("ROUNDDOWN(): %.6f", num);
			}
		}
		
		if (true) { // ROUNDUP():
			auto *cell = row->NewCellAt(col++);
			auto *formula = cell->NewFormula();
			auto *fn = formula->Add(ods::FunctionId::RoundUp);
			fn->AddArg(RoundNum);
			fn->AddArg(places);
			
			auto *node = formula->Eval();
			if (node == nullptr) {
				mtl_info("ROUNDUP() Failed");
			} else {
				double num = node->as_double();
				mtl_info("ROUNDUP(): %.6f", num);
			}
		}
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
		auto *fn = f->Add(ods::FunctionId::Quotient);
		fn->AddArg(sheet->NewReference(lhs));
		fn->AddArg(sheet->NewReference(rhs));
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
		fn = f->Add(ods::FunctionId::Mod);
		fn->AddArg(sheet->NewReference(lhs));
		fn->AddArg(2.1);
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
		fn = f->Add(ods::FunctionId::Power);
		fn->AddArg(sheet->NewReference(lhs));
		fn->AddArg(4.4);
		node = f->Eval();
		if (node == nullptr || !node->is_any_double()) {
			mtl_warn("POWER() failed");
		} else {
			double result = node->as_any_double();
			mtl_info("POWER(): %.4f", result);
		}
	}
	
	if (false) { // IF()
		auto *row = sheet->NewRowAt(last_row++);
		CHECK_PTR_VOID(row);
		
		{ // use IF() with numbers:
			auto *f_cell = row->NewCellAt(3);
			CHECK_PTR_VOID(f_cell);
			auto *f = f_cell->NewFormula();
			auto *fn_if = f->Add(ods::FunctionId::If);
			
			auto *cell0 = row->NewCellAt(0);
			cell0->SetDouble(25.0);
			auto *condition = new QVector<ods::FormulaNode*>();
			ods::Reference *a = sheet->NewReference(cell0);
			condition->append(ods::FormulaNode::Reference(a));
			condition->append(ods::FormulaNode::Op(ods::Op::GreaterOrEqual));
			condition->append(ods::FormulaNode::Double(98));
			fn_if->AddArg(condition);
	
			auto *true_option = ods::FormulaNode::String(new QString("true!"));
			fn_if->AddArg(true_option);
			auto *false_option = fn_if->AddArg(ods::FunctionId::Sum);
			false_option->AddArg(49);
			false_option->AddArg(15);
			
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
			auto *fn_if = f->Add(ods::FunctionId::If);
			
			auto *condition = new QVector<ods::FormulaNode*>();
			auto *lhs = ods::FormulaNode::Reference(sheet->NewReference(cell1));
			condition->append(lhs);
			condition->append(ods::FormulaNode::Op(ods::Op::GreaterOrEqual));
			auto *rhs = ods::FormulaNode::Reference(sheet->NewReference(cell2));
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
	
	if (false) { // COUNT(), COUNTA()
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
			auto *fn = f->Add(ods::FunctionId::Count);
			fn->AddArg(43.2);
			fn->AddArg(new QString("hi"));
			fn->AddArg(sheet->NewReference(num_cell));
			fn->AddArg(sheet->NewReference(str_cell));
			
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
			auto *fn = f->Add(ods::FunctionId::CountA);
			fn->AddArg(43.2);
			fn->AddArg(new QString("hi"));
			fn->AddArg(sheet->NewReference(num_cell));
			fn->AddArg(sheet->NewReference(empty_cell));
			fn->AddArg(sheet->NewReference(str_cell));
			
			auto *node = f->Eval();
			if (node == nullptr || !node->is_double()) {
				mtl_info("COUNTA() eval failed");
			} else {
				double d = node->as_double();
				mtl_info("COUNTA(): %.0f", d);
			}
		}
	}
	
	if (false) { // SUMIF()
		auto *row = sheet->NewRowAt(last_row++);
		int col = 0;
		for (; col < 3; col++) {
			row->NewCellAt(col)->SetDouble(col + 2);
			row->NewCellAt(col + 3)->SetDouble(10 * (col + 1));
		}
		
		{ // SUMIF() when condition is a string
			col = 6;
			auto *fcell = row->NewCellAt(col++);
			auto *f = fcell->NewFormula();
	
			auto *fn = f->Add(ods::FunctionId::SumIf);
			auto *a = sheet->NewReference(row->GetCell(0), row->GetCell(2));
			fn->AddArg(a);
			fn->AddArg(">3");
			a = sheet->NewReference(row->GetCell(3), row->GetCell(5));
			fn->AddArg(a);
			
			auto *node = f->Eval();
			if (node == nullptr) {
				mtl_info("SUMIF() Eval() failed");
			} else if (node->is_none()) {
				mtl_info("SUMIF(): Empty. No addition of data of any type happened.");
			} else {
				auto ba = node->toString().toLocal8Bit();
				mtl_info("SUMIF(): \"%s\"", ba.data());
			}
		}
		
		{ // SUMIF() when condition is an expression (array of FormulaNode*)
			auto *fcell = row->NewCellAt(col++);
			auto *f = fcell->NewFormula();
			
			auto *fn = f->Add(ods::FunctionId::SumIf);
			auto *a = sheet->NewReference(row->GetCell(0), row->GetCell(2));
			// SUMIF() test range:
			fn->AddArg(a);
			
			auto *cond_vec = new QVector<ods::FormulaNode*>();
			a = sheet->NewReference(row->GetCell(2));
			cond_vec->append(ods::FormulaNode::Reference(a));
			cond_vec->append(ods::FormulaNode::Op(ods::Op::Minus));
			cond_vec->append(ods::FormulaNode::Double(2));
			// SUMIF() condition:
			fn->AddArg(cond_vec);
			
			a = sheet->NewReference(row->GetCell(3), row->GetCell(5));
			// SUMIF() sum range:
			fn->AddArg(a);
			
			auto *node = f->Eval();
			if (node == nullptr) {
				mtl_info("SUMIF() Eval() failed");
			} else if (node->is_none()) {
				mtl_info("SUMIF(): Empty. No addition of data of any type happened.");
			} else {
				auto ba = node->toString().toLocal8Bit();
				mtl_info("SUMIF(): \"%s\"", ba.data());
			}
		}
	}
	
	if (false) { // COUNTIF()
		auto *row = sheet->NewRowAt(last_row++);
		int col = 0;
		for (; col < 3; col++) {
			row->NewCellAt(col)->SetDouble(col + 2);
		}
		row->NewCellAt(col++);
		
		{ // COUNTIF() when condition is a string
			auto *fcell = row->NewCellAt(col++);
			auto *f = fcell->NewFormula();
	
			auto *fn = f->Add(ods::FunctionId::CountIf);
			auto *a = sheet->NewReference(row->GetCell(0), row->GetCell(3));
			fn->AddArg(a);
			fn->AddArg(">1");
			
			auto *node = f->Eval();
			if (node == nullptr) {
				mtl_info("COUNTIF() Eval() failed");
			} else {
				double count = node->as_double();
				mtl_info("COUNTIF(): \"%.f\"", count);
			}
		}
		
		{ // COUNTIF() when condition is an expression (array of FormulaNode*)
			auto *fcell = row->NewCellAt(col++);
			auto *f = fcell->NewFormula();
			
			auto *fn = f->Add(ods::FunctionId::CountIf);
			auto *a = sheet->NewReference(row->GetCell(0), row->GetCell(2));
			// COUNTIF() test range:
			fn->AddArg(a);
			
			auto *cond_vec = new QVector<ods::FormulaNode*>();
			a = sheet->NewReference(row->GetCell(2));
			cond_vec->append(ods::FormulaNode::Reference(a));
			cond_vec->append(ods::FormulaNode::Op(ods::Op::Minus));
			cond_vec->append(ods::FormulaNode::Double(2));
			// COUNTIF() condition:
			fn->AddArg(cond_vec);
			
			auto *node = f->Eval();
			if (node == nullptr) {
				mtl_info("COUNTIF() Eval() failed");
			} else {
				double count = node->as_double();
				mtl_info("COUNTIF(): \"%.f\"", count);
			}
		}
	}
	
	if (false) { // COUNTBLANK(), AVERAGE()
		auto *row = sheet->NewRowAt(last_row++);
		int col = 0;
		for (; col < 5; col++) {
			if (col % 2)
				row->NewCellAt(col)->SetDouble(col + 2);
			else
				row->NewCellAt(col);
		}
		
		{ // COUNTBLANK():
			auto *fcell = row->NewCellAt(col++);
			auto *f = fcell->NewFormula();
		
			auto *fn = f->Add(ods::FunctionId::CountBlank);
			auto *a = sheet->NewReference(row->GetCell(0), row->GetCell(3));
			fn->AddArg(a);
			
			auto *node = f->Eval();
			if (node == nullptr) {
				mtl_info("COUNTBLANK() Eval() failed");
			} else {
				double count = node->as_double();
				mtl_info("COUNTBLANK(): \"%.f\"", count);
			}
		}
		
		{ // AVERAGE():
			auto *fcell = row->NewCellAt(col++);
			auto *f = fcell->NewFormula();
		
			auto *fn = f->Add(ods::FunctionId::Average);
			auto *a = sheet->NewReference(row->GetCell(0), row->GetCell(3));
			fn->AddArg(a);
			
			auto *node = f->Eval();
			if (node == nullptr) {
				mtl_info("AVERAGE() Eval() failed");
			} else {
				double avg = node->as_double();
				mtl_info("AVERAGE(): \"%.f\"", avg);
			}
		}
	}
	
	if (false) { // DAY(), MONTH(), YEAR(), AND(), OR(), etc
		auto *row = sheet->NewRowAt(last_row++);
		int col = 0;
		row->NewCellAt(col++)->SetDate(new QDate(QDate::currentDate()));
		row->NewCellAt(col++)->SetString(QLatin1String("2009-11-24"));
		row->NewCellAt(col++);
		const char *date_str = "2001-08-14";
		
		{ // DAY():
			auto *fcell = row->NewCellAt(col++);
			auto *f = fcell->NewFormula();
			auto *fn = f->Add(ods::FunctionId::Day);
			fn->AddArg(date_str);
			
			auto *node = f->Eval();
			if (node == nullptr) {
				mtl_info("DAY() failed");
			} else {
				const int day = node->as_double();
				mtl_info("DAY: %d", day);
			}
		}
		
		{ // MONTH():
			auto *fcell = row->NewCellAt(col++);
			auto *f = fcell->NewFormula();
			auto *fn = f->Add(ods::FunctionId::Month);
			fn->AddArg(date_str);
			
			auto *node = f->Eval();
			if (node == nullptr) {
				mtl_info("MONTH() failed");
			} else {
				const int day = node->as_double();
				mtl_info("MONTH: %d", day);
			}
		}
		
		{ // YEAR():
			auto *fcell = row->NewCellAt(col++);
			auto *f = fcell->NewFormula();
			auto *fn = f->Add(ods::FunctionId::Year);
			fn->AddArg(date_str);
			
			auto *node = f->Eval();
			if (node == nullptr) {
				mtl_info("YEAR() failed");
			} else {
				const int day = node->as_double();
				mtl_info("YEAR: %d", day);
			}
		}
		
		{ // AND():
			auto *fcell = row->NewCellAt(col++);
			auto *f = fcell->NewFormula();
			auto *fn = f->Add(ods::FunctionId::And);
			auto *a = sheet->NewReference(row->GetCell(0), row->GetCell(1));
			fn->AddArg(ods::FormulaNode::Reference(a));
			auto *node = f->Eval();
			if (node == nullptr) {
				mtl_info("AND() eval() failed");
			} else {
				mtl_info("AND(): %s", node->as_bool() ? "true" : "false");
			}
		}
		
		{ // OR():
			auto *fcell = row->NewCellAt(col++);
			auto *f = fcell->NewFormula();
			auto *fn = f->Add(ods::FunctionId::Or);
			auto *a = sheet->NewReference(row->GetCell(2));
			fn->AddArg(ods::FormulaNode::Reference(a));
			auto *node = f->Eval();
			if (node == nullptr) {
				mtl_info("OR() eval() failed");
			} else {
				mtl_info("OR(): %s", node->as_bool() ? "true" : "false");
			}
		}
		
		{ // COLUMNS():
			auto *fcell = row->NewCellAt(col++);
			auto *f = fcell->NewFormula();
			auto *fn = f->Add(ods::FunctionId::Columns);
			auto *a = sheet->NewReference(row->GetCell(0), row->GetCell(5));
			fn->AddArg(ods::FormulaNode::Reference(a));
			auto *node = f->Eval();
			if (node == nullptr) {
				mtl_info("COLUMNS() eval() failed");
			} else {
				mtl_info("COLUMNS(): %d", int(node->as_double()));
			}
		}
		
		{ // TRUE():
			auto *fcell = row->NewCellAt(col++);
			auto *f = fcell->NewFormula();
			f->Add(ods::FunctionId::True);
			auto *node = f->Eval();
			if (node == nullptr) {
				mtl_info("TRUE() eval() failed");
			} else {
				mtl_info("TRUE(): %s", node->as_bool() ? "true" : "false");
			}
		}
		
		{ // FALSE():
			auto *fcell = row->NewCellAt(col++);
			auto *f = fcell->NewFormula();
			f->Add(ods::FunctionId::False);
			auto *node = f->Eval();
			if (node == nullptr) {
				mtl_info("FALSE() eval() failed");
			} else {
				mtl_info("FALSE(): %s", node->as_bool() ? "true" : "false");
			}
		}
		
		{ // NOT():
			auto *fcell = row->NewCellAt(col++);
			auto *f = fcell->NewFormula();
			auto *fn = f->Add(ods::FunctionId::Not);
			fn->AddArg(false);
			auto *node = f->Eval();
			if (node == nullptr) {
				mtl_info("NOT() eval() failed");
			} else {
				mtl_info("NOT(): %s", node->as_bool() ? "true" : "false");
			}
		}
	}
	
	if (false) { // INDIRECT(), OFFSET(), TEXT()
		auto *row = sheet->NewRowAt(last_row++);
		int col = 0;
		
		if (false) { // INDIRECT()
			
			row->NewCellAt(col++)->SetDate(new QDate(QDate::currentDate()));
			row->NewCellAt(col++)->SetString(QLatin1String("2009-11-24"));
			row->NewCellAt(col++);
			row->NewCellAt(col++)->SetString("RC[-3]");
			
			auto *fcell = row->NewCellAt(col++);
			auto *f = fcell->NewFormula();
			auto *fn = f->Add(ods::FunctionId::Indirect);
			
			auto *a = sheet->NewReference(row->GetCell(3));
			fn->AddArg(a);
			fn->AddArg(0);
			
			auto *node = f->Eval();
			if (node == nullptr) {
				mtl_info("INDIRECT() failed");
			} else if (node->is_error()) {
				mtl_print_error(node);
			} else {
				ods::Reference *address = node->as_reference();
				mtl_printq2("INDIRECT(): ", address->toString());
			}
		}
		
		if (false) { // OFFSET()
			const int first_cell = col;
			row->NewCellAt(col++)->SetString("1st cell");
			row->NewCellAt(col++)->SetString("2nd cell");
			auto *fcell = row->NewCellAt(col++);
			auto *f = fcell->NewFormula();
			auto *fn = f->Add(ods::FunctionId::Offset);
			// args: reference, row_offset, col_offset[, new_height, new_width]
			auto *ref = sheet->NewReference(row->GetCell(first_cell));
			fn->AddArg(ref);
			fn->AddArg(0);
			fn->AddArg(1);
			fn->AddArg(1);
			fn->AddArg(1);
			
			ods::FormulaNode *node = f->Eval();
			
			if (node == nullptr) {
				mtl_info("OFFSET() failed");
			} else if (node->is_error()) {
				mtl_print_error(node);
			} else {
				ods::Reference *r = node->as_reference();
				mtl_printq2("OFFSET(): ", r->toString());
				auto *refcell = r->cell();
				CHECK_PTR_VOID(refcell);
				auto *cell = refcell->GetCell();
				CHECK_PTR_VOID(cell);
				auto ba = cell->ValueToString().toLocal8Bit();
				mtl_info("cell value: %s", ba.data());
			}
		}
		
		if (false) { // TEXT()
			const int first_cell = col;
			row->NewCellAt(col++)->SetDouble(3.14159265);
			auto *dt = new QDateTime(QDateTime::currentDateTime());
			row->NewCellAt(col++)->SetDateTime(dt);
			row->NewCellAt(col++)->SetDate(new QDate(QDate::currentDate()));
			row->NewCellAt(col++)->SetTime(new ods::Time(6, 2, 34, 8));
			
			if (true) { // TEXT(number)
				auto *fcell = row->NewCellAt(col++);
				auto *f = fcell->NewFormula();
				auto *fn = f->Add(ods::FunctionId::Text);
				auto *a = sheet->NewReference(row->GetCell(first_cell));
				fn->AddArg(a);
				fn->AddArg("###.#");
				
				auto *node = f->Eval();
				if (node == nullptr) {
					mtl_info("TEXT() failed");
				} else if (node->is_error()) {
					mtl_print_error(node);
				} else {
					mtl_printq2("TEXT(number): ", node->toString());
				}
			}
			
			{ // TEXT(DateTime)
				auto *fcell = row->NewCellAt(col++);
				auto *f = fcell->NewFormula();
				auto *fn = f->Add(ods::FunctionId::Text);
				auto *a = sheet->NewReference(row->GetCell(first_cell + 1));
				fn->AddArg(a);
				fn->AddArg("mm/dd/YYYY hh:m:ss");
				
				auto *node = f->Eval();
				if (node == nullptr) {
					mtl_info("TEXT() failed");
				} else if (node->is_error()) {
					mtl_print_error(node);
				} else {
					mtl_printq2("TEXT(DateTime): ", node->toString());
				}
			}
			
			if (true) { // TEXT(Date)
				auto *fcell = row->NewCellAt(col++);
				auto *f = fcell->NewFormula();
				auto *fn = f->Add(ods::FunctionId::Text);
				auto *a = sheet->NewReference(row->GetCell(first_cell + 2));
				fn->AddArg(a);
				fn->AddArg("yy.m.dd");
				
				auto *node = f->Eval();
				if (node == nullptr) {
					mtl_info("TEXT() failed");
				} else if (node->is_error()) {
					mtl_print_error(node);
				} else {
					mtl_printq2("TEXT(Date): ", node->toString());
				}
			}
			
			if (true) { // TEXT(Time)
				auto *fcell = row->NewCellAt(col++);
				auto *f = fcell->NewFormula();
				auto *fn = f->Add(ods::FunctionId::Text);
				auto *a = sheet->NewReference(row->GetCell(first_cell + 3));
				fn->AddArg(a);
				fn->AddArg("hh:m:s.zzz");
				
				auto *node = f->Eval();
				if (node == nullptr) {
					mtl_info("TEXT() failed");
				} else if (node->is_error()) {
					mtl_print_error(node);
				} else {
					mtl_printq2("TEXT(Time): ", node->toString());
				}
			}
		}
	}
	
	if (true) { // MATCH()
		auto *row = sheet->NewRowAt(last_row++);
		int col = 0;
		const int first_cell = col;
		row->NewCellAt(col++)->SetDouble(3.14159265);
		for (int i = 0; i < 4; i++) {
			row->NewCellAt(col++)->SetDouble(double(i + 5));
		}
		const int last_cell = col - 1;
		
		auto *fcell = row->NewCellAt(col++);
		auto *f = fcell->NewFormula();
		auto *fn = f->Add(ods::FunctionId::Match);
		fn->AddArg(double(7));
		ods::Reference *range = sheet->NewReference(
			row->GetCell(first_cell), row->GetCell(last_cell));
		fn->AddArg(range);
		fn->AddArg(0);
		
		auto *node = f->Eval();
		if (node == nullptr) {
			mtl_info("MATCH() failed");
		} else if (node->is_error()) {
			mtl_print_error(node);
		} else {
			mtl_printq2("MATCH(): ", node->toString());
		}
	}
	util::Save(book);
}

void
ReadCellRange()
{
	QString full_path = "/home/fox/Documents/NamedRange.ods";
	
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
	auto *sheet = spreadsheet->GetSheet(1);
	
	auto *row = sheet->GetRow(3);
	CHECK_PTR_VOID(row);
	auto *fcell = row->GetCell(0);
	CHECK_PTR_VOID(fcell);
	auto *f = fcell->formula();
	CHECK_PTR_VOID(f);
	auto *node = f->Eval();
	
	if (node == nullptr) {
		mtl_info("Named Range formula failed");
	} else {
		mtl_printq2("Formula Eval(): ", node->toString());
	}
	
	util::Save(book);
}

bool
IsFunctionImplemented(const QStringRef &s) {
	const QVector<ods::FunctionMeta> &vec = ods::eval::GetSupportedFunctions();
	
	for (const auto &next: vec) {
		if (s == next.name) {
			return true;
		}
	}
	
	return false;
}

bool CompareVec(const QStringRef &lhs, const QStringRef &rhs) {
	return lhs < rhs;
}

void GenerateFunctionsListForGitHub()
{
	QFile file("/media/data/Documents/ListOfAllFormulaFunctions.txt");
	
	CHECK_TRUE_VOID(file.open(QIODevice::ReadOnly | QIODevice::Text));
	QTextStream in(&file);
	QString all = in.readAll();
	QVector<QStringRef> list1 = all.splitRef(',');
	
	const int count = list1.size();
	const int columns = 4;
	
	const QString impl_str = QLatin1String(":ballot_box_with_check:");
	const QString not_impl_str = QLatin1String(":black_square_button:");
	QString heap = QLatin1String("Column 1 | Column 2 | Column 3 | Column 4\n");
	heap.append("-------- | ---------- | -------- | ---------\n");
	
	QVector<QStringRef> vec;
	
	for (const auto &item: list1) {
		vec.append(item.trimmed());
	}
	int implemented = 0;
	
	std::sort(vec.begin(), vec.end(), CompareVec);
	
	for (int i = 0; i < count; i++) {
		const QStringRef &name = vec[i];
		
		if (IsFunctionImplemented(name)) {
			implemented++;
			heap.append(impl_str);
		} else {
			heap.append(not_impl_str);
		}
		heap.append(' ');
		heap.append(name.toString().toLower());
		heap.append(QLatin1String("()"));
		int col = i % columns;
		
		if (col == columns - 1) {
			heap.append('\n');
		} else {
			heap.append(QLatin1String(" | "));
		}
	}
	
	QString prepend = QLatin1String("##### ") + QString::number(implemented) +
		QLatin1String(" out of ") + QString::number(count) +
		QLatin1String(" functions implemented\n\n");
	
	heap.prepend(prepend);
	
	const QString full_path = QDir::homePath() + "/out.txt";
	QFile out_file(full_path);
	out_file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);
	QTextStream out(&out_file);
	out << heap;
	// optional, as QFile destructor will already do it:
	out_file.close(); 
	
}
