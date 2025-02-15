#include "function.hh"

#include "Reference.hpp"
#include "Cell.hpp"
#include "CellRef.hpp"
#include "Formula.hpp"
#include "FormulaError.hpp"
#include "FormulaNode.hpp"
#include "Row.hpp"
#include "Sheet.hpp"
#include "Time.hpp"

#include <QDate>
#include <QDateTime>

#include <cmath>
#include <QtMath>
#include <unordered_set>

namespace ods::function {

FormulaNode* Acos(const QVector<ods::FormulaNode*> &values)
{
	MTL_CHECK_NULL(values.size() == 1);
	auto *node = values[0];
	const double x = node->as_any_double();
	double ret = acos(x);
	return FormulaNode::Double(ret);
}

FormulaNode* Acot(const QVector<ods::FormulaNode*> &values)
{
	MTL_CHECK_NULL(values.size() == 1);
	auto *node = values[0];
	const double x = node->as_any_double();
	double ret = M_PI/2 - atan(x);
	return FormulaNode::Double(ret);
}

FormulaNode* Asin(const QVector<ods::FormulaNode*> &values)
{
	MTL_CHECK_NULL(values.size() == 1);
	auto *node = values[0];
	const double x = node->as_any_double();
	double ret = asin(x);
	return FormulaNode::Double(ret);
}

FormulaNode* Atan(const QVector<ods::FormulaNode*> &values)
{
	MTL_CHECK_NULL(values.size() == 1);
	auto *node = values[0];
	const double x = node->as_any_double();
	double ret = atan(x);
	return FormulaNode::Double(ret);
}

FormulaNode* Atan2(const QVector<ods::FormulaNode*> &values)
{
	MTL_CHECK_NULL(values.size() == 2);
	double ret = atan2(values[1]->as_any_double(), values[0]->as_any_double());
	return FormulaNode::Double(ret);
}

FormulaNode* Abs(const QVector<ods::FormulaNode*> &values)
{
	MTL_CHECK_NULL(values.size() == 1);
	auto *node = values[0];
	const double x = node->as_any_double();
	double ret = abs(x);
	return FormulaNode::Double(ret);
}

FormulaNode* And(const QVector<ods::FormulaNode*> &values)
{
	bool all_true = true;
	
	for (ods::FormulaNode *node: values) {
		if (!node->InterpretAsBoolean()) {
			all_true = false;
			break;
		}
	}
	
	return ods::FormulaNode::Bool(all_true);
}

FormulaNode* Average(const QVector<ods::FormulaNode*> &values)
{
	i32 count = 0;
	double total = 0;
	
	for (ods::FormulaNode *node: values) {
		if (node->is_any_double()) {
			total += node->as_any_double();
			count++;
		}
	}
	
	return ods::FormulaNode::Double(total / count);
}

FormulaNode* Bool(const bool flag) {
	return FormulaNode::Bool(flag);
}

FormulaNode* ColumnsRows(const QVector<ods::FormulaNode*> &values, const ColsOrRows arg)
{
//COLUMNS sums the number of columns in each range, irrespective
// of any duplication of columns. 
	i32 total = 0;
	const bool cols = (arg == ColsOrRows::Columns);
	
	for (ods::FormulaNode *node: values) {
		if (node->is_reference()) {
			ods::Reference *a = node->as_reference();
			auto *cell = a->cell();
			const i32 start = cols ? cell->col() : cell->row();
			if (a->is_cell_range()) {
				auto *end_cell = a->end_cell();
				i32 end = cols ? end_cell->col() : end_cell->row();
				total += std::abs(start - end);
			} else {
				total++;
			}
		}
	}
	
	return ods::FormulaNode::Double(total);
}

FormulaNode* Concatenate(const QVector<ods::FormulaNode*> &values)
{
	QString *s = new QString();
	
	for (ods::FormulaNode *node: values) {
		s->append(node->toString());
	}
	auto *result = new ods::FormulaNode();
	result->SetString(s);
	
	return result;
}

FormulaNode* Cos(const QVector<ods::FormulaNode*> &values)
{
	MTL_CHECK_NULL(values.size() == 1);
	auto *node = values[0];
	double n = qCos(node->as_any_double());
	return FormulaNode::Double(double(n));
}

FormulaNode* Cot(const QVector<ods::FormulaNode*> &values)
{
	MTL_CHECK_NULL(values.size() == 1);
	auto *node = values[0];
	const double x = node->as_any_double();
	double ret = cos(x) / sin(x);
	return FormulaNode::Double(ret);
}

FormulaNode* Count(const QVector<ods::FormulaNode*> &values)
{
	i64 total = 0;
	
	for (ods::FormulaNode *value: values) {
		if (value->is_any_double())
			total++;
	}
	
	return ods::FormulaNode::Double(total);
}

FormulaNode* CountA(const QVector<ods::FormulaNode*> &values)
{
	i64 total = 0;
	
	for (ods::FormulaNode *value: values) {
		if (!value->is_empty())
			total++;
	}
	
	return ods::FormulaNode::Double(total);
}

FormulaNode* CountBlank(const QVector<ods::FormulaNode*> &values)
{
	i64 total = 0;
	
	for (ods::FormulaNode *node: values) {
		if (node->is_empty())
			total++;
	}
	
	return ods::FormulaNode::Double(total);
}

FormulaNode* CountIf(const QVector<ods::FormulaNode*> &values, ods::Sheet *default_sheet)
{
	QVector<FormulaNode*> test_range_vec;
	ods::AutoDeleteVec test_range_adv(test_range_vec);
	
	QVector<FormulaNode*> cond_nodes;
	ods::AutoDeleteVec cond_nodes_adv(cond_nodes);
	
	MTL_CHECK_NULL(eval::CommonForSumIfLikeFunctions_BuildUp(values, default_sheet,
		test_range_vec, cond_nodes, nullptr));
	
	eval::PrintNodes(cond_nodes, "CountIf() Before for loop");
	i64 count = 0;
	const int range_size = test_range_vec.size();
	// Now for the testing and summing up!
	for (int i = 0; i < range_size; i++) {
		FormulaNode *test_node = test_range_vec[i];
		if (test_node->is_empty()) {
			// Blank (empty) cells in test_range are ignored (they never
			// satisfy the condition).
			continue;
		}
		
		FormulaNode *result = eval::CommonForSumIfLikeFunctions_Eval(*test_node, cond_nodes);
		ods::AutoDelete result_ad(result);
		MTL_CHECK_NULL((result != nullptr && result->is_bool()));
		const bool ok = result->as_bool();
		
		if (ok) {
			count++;
		}
	}
	
	return FormulaNode::Double(double(count));
}

FormulaNode* Date(const QVector<ods::FormulaNode*> &values)
{
	MTL_CHECK_NULL((values.size() == 3));
	
	int arr[3] = {};
	
	for (int i = 0; i < 3; i++) {
		FormulaNode *node = values[i];
		MTL_CHECK_NULL((node->is_double()));
		arr[i] = node->as_double();
	}
	
	QDate *d = new QDate(arr[0], arr[1], arr[2]);
	return FormulaNode::Date(d);
}

FormulaNode* DayMonthYear(const QVector<ods::FormulaNode*> &values, const DMY dmy)
{
	MTL_CHECK_NULL((values.size() == 1));
	
	FormulaNode *node = values[0];
	int ret_val = -1;
	
	if (node->is_date()) {
		QDate *date = node->as_date();
		if (dmy == DMY::Day)
			ret_val = date->day();
		else if (dmy == DMY::Month)
			ret_val = date->month();
		else if (dmy == DMY::Year)
			ret_val = date->year();
		else {
			mtl_trace();
			return nullptr;
		}
	} else if (node->is_date_time()) {
		QDate date = node->as_date_time()->date();
		if (dmy == DMY::Day)
			ret_val = date.day();
		else if (dmy == DMY::Month)
			ret_val = date.month();
		else if (dmy == DMY::Year)
			ret_val = date.year();
		else {
			mtl_trace();
			return nullptr;
		}
	} else if (node->is_string()) {
		// Expected formats: "2008-06-24" or "24-06-2008"
		QString *date_str = node->as_string();
#ifdef DEBUG_FORMULA_EVAL
		{
			auto ba = date_str->toLocal8Bit();
			mtl_info("Date: \"%s\"", ba.data());
		}
#endif
		QChar sep('.');
		
		if (date_str->indexOf('-') != -1)
			sep = QChar('-');
		else if (date_str->indexOf('/') != -1)
			sep = QChar('/');
		
		auto list = QStringView(*date_str).split(sep);
		MTL_CHECK_NULL((list.size() == 3));
		const bool year_first = (list[0].size() > 2);
		int index = -1;
		
		if (dmy == DMY::Day)
			index = year_first ? 2 : 0;
		else if (dmy == DMY::Month)
			index = 1;
		else if (dmy == DMY::Year)
			index = year_first ? 0 : 2;
		else {
			mtl_trace();
			return nullptr;
		}
		
		auto str = list[index];
		bool ok;
		ret_val = str.toInt(&ok);
		
		if (!ok) {
			auto ba = date_str->toLocal8Bit();
			mtl_trace("Invalid date: \"%s\"", ba.data());
			return nullptr;
		}
	}
	
	MTL_CHECK_NULL(ret_val != -1);
	return FormulaNode::Double(ret_val);	
}

FormulaNode* If(const QVector<FormulaNode*> &values)
{
	MTL_CHECK_NULL(values.size() == 3);
	FormulaNode *condition = values[0];
	FormulaNode *true_node = values[1];
	FormulaNode *false_node = values[2];
	
	// Treat as 'false' zero, empty string or bool=false,
	bool flag = true;
	
	if (condition->is_bool()) {
		flag = condition->as_bool();
	} else if (condition->is_any_double()) {
		const double d = condition->as_double();
		if (eval::IsNearlyEqual(d, 0.0)) {
			flag = false;
		}
	} else if (condition->is_string()) {
		QString *s = condition->as_string();
		flag = !s->isEmpty();
	}

	return flag ? true_node->Clone() : false_node->Clone();
}

FormulaNode* Indirect(const QVector<FormulaNode*> &values, ods::Formula *formula)
{
	MTL_CHECK_NULL(!values.isEmpty());
	//eval::PrintNodes(values, "function.cc::Indirect()");
	bool is_r1c1 = false;
	
	if (values.size() == 2) {
		auto *node = values[1];
		is_r1c1 = node->is_any_double() && (node->as_any_double() == 0);
	}
	
	FormulaNode *node = values[0];
	
	if (!node->is_string())
		return FormulaNode::Error(FormError::WrongArgType, "A string is needed.");
	
	const QString &addr_str = *node->as_string();
	Reference *a = nullptr;
	
	if (is_r1c1) {
		a = ods::Reference::R1C1From(addr_str, formula);
	} else {
		a = ods::Reference::From(addr_str, formula->default_sheet());
	}
	
	MTL_CHECK_NULL(a);
	return FormulaNode::Reference(a);
}

FormulaNode* Match(const QVector<ods::FormulaNode*> &values)
{
	MTL_CHECK_NULL((values.size() == 3));
//	eval::PrintNodes(values);
	
	FormulaNode *search_item = values[0];
	FormulaNode *search_range = values[1];
	FormulaNode *param_node = values[2];
	
	if (!search_item->is_any_double()) {
		mtl_warn("Match:() Currently only can search for numbers");
		return nullptr;
	}
	
	MTL_CHECK_NULL(search_range->is_reference());
	MTL_CHECK_NULL(param_node->is_any_double());
	
	int param = param_node->as_any_double();
	
	if (param != 0) {
		mtl_warn("Match(): currently only implemented searching for exact item");
		return nullptr;
	}
	
	double search_for = search_item->as_any_double();
	Reference *reference = search_range->as_reference();
	QVector<ods::Cell*> cells;
	MTL_CHECK_NULL(reference->GenCells(cells));
	
	for (ods::Cell *cell: cells) {
		if (!cell->is_any_double())
			continue;
		
		double d = *cell->as_double();
		
		if (eval::IsNearlyEqual(d, search_for)) {
			ods::Sheet *sheet = cell->row()->sheet();
			Reference *r = sheet->NewReference(cell);
			return FormulaNode::Reference(r);
		}
	}
	
	return nullptr;
}

FormulaNode* Max(const QVector<ods::FormulaNode*> &values)
{
	double max;
	bool initialize = true;
	
	for (ods::FormulaNode *value: values) {
		MTL_CHECK_NULL(value->is_double());
		if (initialize) {
			initialize = false;
			max = value->as_double();
		} else {
			double d = value->as_double();
			if (d > max)
				max = d;
		}
	}
	
	return FormulaNode::Double(max);
}

FormulaNode* Min(const QVector<FormulaNode *> &values)
{
	double min = 0;
	bool initialized = false;
	
	for (ods::FormulaNode *value: values) {
		MTL_CHECK_NULL(value->is_any_double());
		if (!initialized) {
			initialized = true;
			min = value->as_any_double();
		} else {
			double d = value->as_any_double();
			if (d < min)
				min = d;
		}
	}
	
	if (initialized)
		return FormulaNode::Double(min);
	return FormulaNode::Empty();
}

FormulaNode* Mod(const QVector<FormulaNode*> &values)
{
	MTL_CHECK_NULL(values.size() == 2);
	auto *lhs_node = values[0];
	auto *rhs_node = values[1];
	if (!lhs_node->is_any_double() || !rhs_node->is_any_double()) {
		mtl_trace("Arg is not a number");
		return nullptr;
	}
	double rhs = rhs_node->as_any_double();
	MTL_CHECK_NULL(rhs != 0);
	double lhs = lhs_node->as_any_double();
	double n = std::remainder(lhs, rhs); // ret type can be a float point value
	return FormulaNode::Double(double(n));
}

FormulaNode* Not(const QVector<ods::FormulaNode*> &values)
{
	MTL_CHECK_NULL(values.size() == 1);
	FormulaNode *node = values[0];
	bool flag = !node->InterpretAsBoolean();
	
	return ods::FormulaNode::Bool(flag);
}

FormulaNode* Now()
{
	QDateTime now = QDateTime::currentDateTime();
	return FormulaNode::DateTime(new QDateTime(now));
}

FormulaNode* Offset(const QVector<ods::FormulaNode*> &values)
{
//	eval::PrintNodes(values);
	MTL_CHECK_NULL(values.size() >= 3);
	FormulaNode *refr_node = values[0];
	MTL_CHECK_NULL(refr_node->is_reference());
	FormulaNode *row_offset = values[1];
	MTL_CHECK_NULL(row_offset->is_any_double());
	FormulaNode *col_offset = values[2];
	MTL_CHECK_NULL(col_offset->is_any_double());
	
	const int row_off = row_offset->as_any_double();
	const int col_off = col_offset->as_any_double();
	
	int new_h = -1, new_w = -1;
	if (values.size() >= 4) {
		FormulaNode *node = values[3];
		MTL_CHECK_NULL(node->is_any_double());
		new_h = node->as_any_double();
	}
	
	if (values.size() >= 5) {
		FormulaNode *node = values[4];
		MTL_CHECK_NULL(node->is_any_double());
		new_w = node->as_any_double();
	}
	
	ods::Reference *refr = refr_node->as_reference();
	ods::Reference *new_refr = refr->Offset(row_off, col_off, new_h, new_w);
	MTL_CHECK_NULL(new_refr);
	
	return FormulaNode::Reference(new_refr);
}

FormulaNode* Or(const QVector<ods::FormulaNode*> &values)
{
	bool is_any_true = false;
	
	for (ods::FormulaNode *node: values) {
		if (node->InterpretAsBoolean()) {
			is_any_true = true;
			break;
		}
	}
	
	return ods::FormulaNode::Bool(is_any_true);
}

FormulaNode* Power(const QVector<FormulaNode*> &values)
{
	MTL_CHECK_NULL(values.size() == 2);
	auto *lhs_node = values[0];
	auto *rhs_node = values[1];
	if (!lhs_node->is_any_double() || !rhs_node->is_any_double()) {
		mtl_trace("Arg is not a number");
		return nullptr;
	}
	double rhs = rhs_node->as_any_double();
	MTL_CHECK_NULL(rhs != 0);
	double lhs = lhs_node->as_any_double();
	double n = std::pow(lhs, rhs); // ret type can be a float point value
	return FormulaNode::Double(double(n));
}

FormulaNode* Product(const QVector<ods::FormulaNode*> &values)
{
	double d = 0;
	
	for (int i = 0; i < values.size(); i++) {
		ods::FormulaNode *node = values[i];
		
		if (!node->is_any_double())
			continue;
		
		const double n = node->as_any_double();
		
		if (i == 0)
			d = n;
		else
			d *= n;
	}

	return FormulaNode::Double(d);
}

FormulaNode* Quotient(const QVector<FormulaNode*> &values)
{
	MTL_CHECK_NULL(values.size() == 2);
	auto *lhs_node = values[0];
	auto *rhs_node = values[1];
	if (!lhs_node->is_any_double() || !rhs_node->is_any_double()) {
		mtl_trace("Arg is not a number");
		return nullptr;
	}
	double rhs = rhs_node->as_any_double();
	MTL_CHECK_NULL(rhs != 0);
	double lhs = lhs_node->as_any_double();
	i64 n = lhs / rhs;
	return FormulaNode::Double(double(n));
}

FormulaNode* RoundAnyWay(const QVector<FormulaNode*> &values, const RoundType round_type)
{ // table:formula="of:=ROUND([.F35]*[.F36];2)"
	MTL_CHECK_NULL(values.size() != 0);
	double number = values[0]->as_any_double();
	int places = 0;
	
	if (values.size() == 2) {
		FormulaNode *node = values[1];
		MTL_CHECK_NULL(node->ConvertFunctionOrAddressToValue());
		if (node->is_any_double())
			places = node->as_double();
	}
	
	if (places > 0) {
		switch (round_type) {
			case RoundType::Round: {number = eval::RoundUp(number, places); break;}
			case RoundType::Ceil: {number = eval::CeilUp(number, places); break;}
			case RoundType::Floor: {number = eval::FloorUp(number, places); break;}
			default: {
				mtl_trace();
				return nullptr;
			}
		}
	} else if (places == 0) {
		number = i64(number); // round to the first integer
	} else { // places < 0
		// =ROUND(5555;-1) = 5560
		const double n = pow(10, -places);
		double d = round(number / n);
		number = i64(d) * n;
	}
	/*
=ROUND(10.1) 	10		If b is not specified, round to the nearest integer.
=ROUND(0.5) 	1		0.5 rounds up.
=ROUND(1/3;0) 	0		Round to the nearest integer.
=ROUND(1/3;1) 	0.3 	Round to one decimal place.
=ROUND(1/3;2) 	0.33 	Round to two decimal places.
=ROUND(1/3;2.9) 0.33 	If b is not an integer, it is truncated.
=ROUND(5555;-1) 5560 	Round to the nearest 10.
=ROUND(-1.1) 	-1	 	Negative number rounded to the nearest integer
=ROUND(-1.5) 	-2	 	Negative number rounded to the nearest integer 
	*/
	return FormulaNode::Double(number);
}

FormulaNode* Sin(const QVector<ods::FormulaNode*> &values)
{
	MTL_CHECK_NULL(values.size() == 1);
	auto *node = values[0];
	double n = qSin(node->as_any_double());
	return FormulaNode::Double(double(n));
}

FormulaNode* Sum(const QVector<ods::FormulaNode*> &values)
{
	double d = 0;
	
	for (ods::FormulaNode *node: values) {
		
		if (node->is_any_double())
			d += node->as_any_double();
	}
	
	return FormulaNode::Double(d);
}

FormulaNode* SumIf(const QVector<ods::FormulaNode*> &values, ods::Sheet *default_sheet)
{ // =SUMIF(E17:E33,"=x",F17:F33)
// table:formula="of:=SUMIF([.E17:.E33];&quot;=x&quot;;[.F17:.F33])"
	
	QVector<FormulaNode*> test_range_vec;
	ods::AutoDeleteVec test_range_adv(test_range_vec);
	
	QVector<FormulaNode*> cond_nodes;
	ods::AutoDeleteVec cond_nodes_adv(cond_nodes);
	
	QVector<FormulaNode*> sum_range_vec;
	ods::AutoDeleteVec sum_range_vec_adv(sum_range_vec);
	
	MTL_CHECK_NULL(eval::CommonForSumIfLikeFunctions_BuildUp(values, default_sheet,
		test_range_vec, cond_nodes, &sum_range_vec));
	
	ods::FormulaNode sumup;
	const int count = std::min(sum_range_vec.size(), test_range_vec.size());
	// Now for the testing and summing up!
	for (int i = 0; i < count; i++) {
		FormulaNode *test_node = test_range_vec[i];
		if (test_node->is_empty()) {
			// Blank (empty) cells in test_range are ignored (they never
			// satisfy the condition).
			return nullptr;
		}
		FormulaNode *result = eval::CommonForSumIfLikeFunctions_Eval(*test_node, cond_nodes);
		ods::AutoDelete result_ad(result);
		MTL_CHECK_NULL((result != nullptr && result->is_bool()));
		const bool do_sumup = result->as_bool();
		
		if (do_sumup) {
			ods::FormulaNode *sum_node = sum_range_vec[i];
			
			if (sumup.is_none()) {
				sumup.AdoptDefaultValueFrom(*sum_node);
			}
			
			sumup.Operation(ods::Op::Plus, sum_node);
		}
	}
	
	return sumup.Clone();
}

FormulaNode* Tan(const QVector<ods::FormulaNode*> &values)
{
	MTL_CHECK_NULL(values.size() == 1);
	auto *node = values[0];
	const double x = node->as_any_double();
	double ret = sin(x) / cos(x);
	return FormulaNode::Double(ret);
}

FormulaNode* Text(const QVector<FormulaNode *> &values)
{ //TEXT([.B7];"mm/dd/yyyy")
//	eval::PrintNodes(values, "from function::Text()");
	
	MTL_CHECK_NULL((values.size() == 2));
	
	FormulaNode *num_node = values[0];
	MTL_CHECK_NULL(num_node->HasNumberTrait());
	FormulaNode *format_node = values[1];
	MTL_CHECK_NULL(format_node->is_string());
	
	QString format_str = *format_node->as_string();
	QString result;
	
	if (num_node->is_any_double()) {
		const double d = num_node->as_any_double();
		eval::FormatNumber(format_str, d, result);
	} else {
		if (num_node->is_date_time()) {
			QDateTime *dt = num_node->as_date_time();
			QDate date = dt->date();
			QTime time = dt->time();
			MTL_CHECK_NULL(eval::FormatAsDateTime(format_str, &date, &time, result));
		} else if (num_node->is_date()) {
			QDate *date = num_node->as_date();
			MTL_CHECK_NULL(eval::FormatAsDateTime(format_str, date, nullptr, result));
		} else if (num_node->is_time()) {
			ods::Time *t = num_node->as_time();
			QTime time_arg(t->hours(), t->minutes(), t->seconds(), t->ms());
			MTL_CHECK_NULL(eval::FormatAsDateTime(format_str, nullptr, &time_arg, result));
		} else {
			mtl_it_happened();
			return nullptr;
		}
	}
	
	return FormulaNode::String(new QString(result));
}

FormulaNode* Today() {
	auto *p = new QDate(QDate::currentDate());
	return FormulaNode::Date(p);
}
} // ods::
