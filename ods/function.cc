#include "function.hh"

#include "Address.hpp"
#include "Cell.hpp"
#include "CellRef.hpp"
#include "Formula.hpp"
#include "FormulaNode.hpp"
#include "Time.hpp"
#include "inst/TableNamedRange.hpp"

#include <QDate>
#include <QDateTime>

#include <cmath>

namespace ods::function {

QVector<FormulaNode*>*
CloneVec(const QVector<FormulaNode*> &vec)
{
	auto *v = new QVector<FormulaNode*>();
	for (auto *item: vec) {
		v->append(item->Clone());
	}
	
	return v;
}

bool
CommonForSumIfLikeFunctions_BuildUp(const QVector<ods::FormulaNode*> &values,
	ods::Sheet *default_sheet,
	QVector<FormulaNode*> &test_range_vec,
	QVector<FormulaNode*> &cond_nodes,
	QVector<FormulaNode*> *sum_range_vec)
{
	CHECK_TRUE(values.size() == 3);
	
	if (sum_range_vec != nullptr)
	{
		FormulaNode *sum_range = values[2];
		sum_range_vec->append(sum_range->Clone());
		CHECK_TRUE(function::FlattenOutArgs(*sum_range_vec));
	}
	
	{
		FormulaNode *test_range = values[0];
		test_range_vec.append(test_range->Clone());
		// Now convert address ranges to arrays of values
		CHECK_TRUE(function::FlattenOutArgs(test_range_vec));
	}
	
	{
		FormulaNode *condition = values[1];
		if (condition->is_string()) {
			QString cond_str = *condition->as_string();
			CHECK_TRUE(Formula::ParseString(cond_str, cond_nodes,
				default_sheet, ods::TreatRemainderAsString));
#ifdef DEBUG_SUMIF_LIKE_FUNCTIONS
			PrintNodes(cond_nodes, "Before adding anything:");
#endif
			if (cond_nodes.size() == 1) {
				// Op::Equals is implied
				cond_nodes.prepend(FormulaNode::Op(Op::Equals));
			}
		} else {
#ifdef DEBUG_SUMIF_LIKE_FUNCTIONS
			mtl_print_node(condition, "Condition not a string:");
#endif
			cond_nodes.append(FormulaNode::Op(Op::Equals));
			cond_nodes.append(condition->Clone());
		}
	}
	
	return true;
}

FormulaNode*
CommonForSumIfLikeFunctions_Eval(const FormulaNode &test_node,
	QVector<FormulaNode*> &cond_nodes)
{
	QVector<ods::FormulaNode*> *expression = function::CloneVec(cond_nodes);
	ods::AutoDeleteVecP expression_advp(expression);
	expression->prepend(test_node.Clone());
#ifdef DEBUG_SUMIF_LIKE_FUNCTIONS
	function::PrintNodesInOneLine(*expression);
#endif
	auto *result = Formula::EvaluateNodes(*expression);
#ifdef DEBUG_SUMIF_LIKE_FUNCTIONS
		mtl_print_node(result, "Expression result: ");
#endif
	return result;
}

bool
EvalDeepestGroup(QVector<FormulaNode*> &input)
{
	// first replace named ranges (if any) with regular nodes
	CHECK_TRUE(ReplaceNamedRanges(input));
	
	int start = -1, count = -1;
	bool remove_braces = false;
	
	for (int i = 0; i < input.size(); i++) {
		const FormulaNode *node = input[i];
		
		if (node->is_brace()) {
			if (node->as_brace() == Brace::Open) {
				remove_braces = true;
				if (i > start)
					start = i;
			} else if (count == -1) {
				// "+ 1" to include the closing brace
				count = i - start + 1;
			}
		}
	}
	
	if (start == -1) {
		start = 0;
		count = input.size();
	}
	
	QVector<FormulaNode*> ret;
	int add_start = remove_braces ? start + 1 : start;
	int add_end = start + count;

	if (remove_braces)
		add_end--;
	
	for (int i = add_start; i < add_end; i++) {
		FormulaNode *node = input[i];

		if (node->is_function()) {
			Function *f = node->as_function();
			ods::FormulaNode *value = f->Eval();
			CHECK_PTR(value);
			ret.append(value);
			delete node;
		} else {
			ret.append(node);
		}
	}
	
	input.erase(input.begin() + start, input.begin() + start + count);
	CHECK_TRUE(EvalNodesByOpPrecedence(ret));
	CHECK_TRUE((ret.size() == 1));
	input.insert(input.begin() + start, ret[0]);
	return true;
}

bool
EvalNodesByOpPrecedence(QVector<FormulaNode*> &nodes)
{
	int iter = 0;
	while (nodes.size() > 1)
	{
		if (iter++ > 300) {
			mtl_trace();
			return false;
		}
		
#ifdef DEBUG_FORMULA_EVAL
		PrintNodesInOneLine(nodes, "EvalNodesByOpPrecedence() while() start");
#endif
		const int op_index = FindHighestPriorityOp(nodes);
		RET_IF_EQUAL(op_index, -1);
		RET_IF_EQUAL(op_index, nodes.size() - 1);
#ifdef DEBUG_FORMULA_EVAL
		mtl_info("op_index: %d", op_index);
#endif
		if (ProcessIfInfixPlusOrMinus(nodes, op_index))
			continue;
		
		FormulaNode *op_node = nodes[op_index];
		ods::Op op = op_node->as_op();
		FormulaNode *prev_node = nodes[op_index - 1];
		FormulaNode *next_node = nodes[op_index + 1];
		CHECK_TRUE(prev_node->Operation(op, next_node));
		delete next_node;
		delete op_node;
		// + 2 instead of + 1 because the last element is not included
		nodes.erase(nodes.begin() + op_index, nodes.begin() + op_index + 2);
	}
	return true;
}

bool
ExtractCellValue(ods::Cell *cell, ods::FormulaNode &result)
{
	CHECK_PTR(cell);
	
	if (cell->has_formula()) {
		ods::Formula *f = cell->formula();
		if (f->evaluating()) {
			mtl_trace("Cyclic formula reference!");
			return false;
		}
		ods::FormulaNode *value = f->Eval();
		CHECK_PTR(value);
		result = *value;
	} else if (cell->is_any_double()) {
		double d = *cell->as_double();
		result.SetDouble(d);
	} else if (cell->is_string()) {
		result.SetString(new QString(*cell->GetFirstString()));
	} else if (cell->is_date()) {
		result.SetDate(new QDate(*cell->as_date()));
	} else if (cell->is_date_time()) {
		result.SetDateTime(new QDateTime(*cell->as_date_time()));
	} else if (cell->is_time()) {
		result.SetTime(new ods::Time(*cell->as_time()));
	} else if (cell->is_empty()) {
		result.SetNone();
	} else {
		mtl_warn("Cell not a number");
		return false;
	}
	
	return true;
}

bool
ExtractAddressValues(ods::FormulaNode *node, QVector<ods::FormulaNode*> &result)
{
	auto *address = node->as_address();
	
	if (address->is_cell_range()) {
#ifdef DEBUG_FORMULA_EVAL
		auto ba = address->toString().toLocal8Bit();
		mtl_info("Cell Address Range: %s", ba.data());
#endif
		QVector<ods::Cell*> cells;
		CHECK_TRUE(address->GenCells(cells));
		
		for (auto *cell : cells) {
			auto *val = new ods::FormulaNode();
			if (ExtractCellValue(cell, *val)) {
				result.append(val);
			} else {
				delete val;
				mtl_trace();
				return false;
			}
		}
		return true;
	} else {
		auto *cell_ref = address->cell();
		ods::Cell *cell = cell_ref->GetCell();
		auto *val = new ods::FormulaNode();
		if (ExtractCellValue(cell, *val)) {
			result.append(val);
			return true;
		} else {
			delete val;
			return false;
		}
	}
	return true;
}

int
FindHighestPriorityOp(QVector<FormulaNode*> &vec)
{
	const int count = vec.size();
	int highest_index = -1;
	int highest_priority = -1;
	
	for (int i = 0; i < count; i++) {
		FormulaNode *node = vec[i];
		
		if (!node->is_op())
			continue;
		
		ods::Op op = node->as_op();
		const int priority = ods::priority(op);
		if (priority > highest_priority) {
			highest_priority = priority;
			highest_index = i;
		}
	}
	
	return highest_index;
}

const FunctionMeta*
FindFunctionMeta(const FunctionId id)
{
	for (const FunctionMeta &fi: GetSupportedFunctions()) {
		if (id == fi.id) {
			return &fi;
		}
	}
	
	return nullptr;
}

const FunctionMeta*
FindFunctionMeta(const QString &name)
{
	for (const FunctionMeta &fi: GetSupportedFunctions()) {
		if (name == fi.name) {
			return &fi;
		}
	}
	
	return nullptr;
}

bool
FlattenOutArgs(QVector<ods::FormulaNode*> &vec)
{
	for (int i = 0; i < vec.size(); i++) {
		ods::FormulaNode *node = vec[i];
		CHECK_PTR(node);
		
		if (node->is_function()) {
			Function *f = node->as_function();
			ods::FormulaNode *value = f->Eval();
			CHECK_PTR(value);
			delete node;
			vec[i] = value;
		} else if (node->is_address()) {
			QVector<FormulaNode*> ext;
			CHECK_TRUE(ExtractAddressValues(node, ext));
			
			for (int k = 0; k < ext.size(); k++) {
				vec.insert(i + k, ext[k]);
			}
			vec.remove(i + ext.size());
		} else { // must not be processed
		}
	}
	
	return true;
}

const QVector<FunctionMeta>&
GetSupportedFunctions() {
// A third param of 0 indicates that the function args shouldn't be
// "flattened out", e.g. an "address range" node shouldn't be replaced
// with an array of nodes containing the values of those cells.
	static const QVector<FunctionMeta> v = {
	FunctionMeta ("SUM", FunctionId::Sum),
	FunctionMeta ("MAX", FunctionId::Max),
	FunctionMeta ("MIN", FunctionId::Min),
	FunctionMeta ("SUMIF", FunctionId::SumIf, 0),
	FunctionMeta ("PRODUCT", FunctionId::Product),
	FunctionMeta ("CONCATENATE", FunctionId::Concatenate),
	FunctionMeta ("DATE", FunctionId::Date),
	FunctionMeta ("NOW", FunctionId::Now),
	FunctionMeta ("QUOTIENT", FunctionId::Quotient),
	FunctionMeta ("MOD", FunctionId::Mod),
	FunctionMeta ("POWER", FunctionId::Power),
	FunctionMeta ("IF", FunctionId::If),
	FunctionMeta ("COUNT", FunctionId::Count),
	FunctionMeta ("COUNTA", FunctionId::CountA),
	FunctionMeta ("COUNTBLANK", FunctionId::CountBlank),
	FunctionMeta ("COUNTIF", FunctionId::CountIf, 0),
	FunctionMeta ("AVERAGE", FunctionId::Average),
	FunctionMeta ("TODAY", FunctionId::Today),
	FunctionMeta ("ROUND", FunctionId::Round, 0),
	FunctionMeta ("ROUNDDOWN", FunctionId::RoundDown, 0),
	FunctionMeta ("ROUNDUP", FunctionId::RoundUp, 0),
	FunctionMeta ("DAY", FunctionId::Day),
	FunctionMeta ("MONTH", FunctionId::Month),
	FunctionMeta ("YEAR", FunctionId::Year),
	};
	return v;
}

bool IsNearlyEqual(double x, double y)
{
	const double epsilon = 1e-5;
	return std::abs(x - y) <= epsilon * std::abs(x);
	// see Knuth section 4.2.2 pages 217-218
}

void NodeToStr(FormulaNode *node, QString &type_str, QString &node_str)
{
	if (node->is_address()) {
		type_str = QLatin1String("Address");
		node_str = node->as_address()->toString();
	} else if (node->is_function()) {
		type_str = QLatin1String("Function");
		node_str = node->as_function()->toString();
	} else if (node->is_double()) {
		type_str = QLatin1String("Number");
		node_str = QString::number(node->as_double());
	} else if (node->is_op()) {
		type_str = QLatin1String("Op");
		node_str = ods::op::ToString(node->as_op());
	} else if (node->is_brace()) {
		type_str = QLatin1String("Brace");
		node_str = ods::ToString(node->as_brace());
	} else if (node->is_string()) {
		type_str = QLatin1String("String");
		node_str = *node->as_string();
	} else if (node->is_time()) {
		type_str = QLatin1String("Time");
		node_str = node->as_time()->toString();
	} else if (node->is_date()) {
		type_str = QLatin1String("Date");
		node_str = node->as_date()->toString(Qt::ISODate);
	} else if (node->is_date_time()) {
		type_str = QLatin1String("DateTime");
		node_str = node->as_date_time()->toString(Qt::ISODate);
	} else if (node->is_bool()) {
		type_str = QLatin1String("Boolean");
		node_str = node->as_bool() ? QLatin1String("true") : QLatin1String("false");
	} else if (node->is_named_range()) {
		type_str = QLatin1String("Named Range");
		node_str = node->toString();
	} else if (node->is_none()) {
		type_str = QLatin1String("None");
		node_str = QLatin1String("[!!]");
	}
}

void PrintNodes(const QVector<FormulaNode*> &nodes, const QString &msg)
{
	if (msg.isEmpty())
		mtl_info("Formula nodes (%d):", nodes.size());
	else {
		auto ba = msg.toLocal8Bit();
		mtl_info("%s", ba.data());
	}
	
	for (int i = 0; i < nodes.size(); i++) {
		FormulaNode *node = nodes[i];
		QString type_str, node_str;
		NodeToStr(node, type_str, node_str);
		auto ba = node_str.toLocal8Bit();
		auto type_ba = type_str.toLocal8Bit();
		printf("(%d) %s: \"%s\"\n", i+1, type_ba.data(), ba.data());
	}
}

void PrintNodesInOneLine(const QVector<FormulaNode *> &v, const char *msg)
{
	QString s;
	QString separator = QString(MTL_COLOR_GREEN) + "|" + MTL_COLOR_DEFAULT;
	for (ods::FormulaNode *node : v) {
		s.append(node->toString()).append(separator);
	}
	
	auto ba = s.toLocal8Bit();
	auto sep_ba = separator.toLocal8Bit();
	mtl_info("%s%s %s", sep_ba.data(), ba.data(), msg);
}

bool ProcessIfInfixPlusOrMinus(QVector<FormulaNode*> &nodes, const int op_index)
{
	FormulaNode *op_node = nodes[op_index];
	const ods::Op op = op_node->as_op();
	
	if (op != Op::Minus && op != Op::Plus) {
//		auto ba = ods::op::ToString(op).toLocal8Bit();
//		mtl_warn("Don't know what to do with %s", ba.data());
		return false;
	}
	
	if (nodes.size() <= 1)
		return false;

	bool is_infix = op_index == 0;
	if (!is_infix) {
		FormulaNode *prev_node = nodes[op_index - 1];
		if (prev_node->is_op())
			is_infix = true;
		else
			return false;
	}
	
	const int adj_node_index = op_index + 1;
	CHECK_TRUE((adj_node_index < nodes.size()));
	FormulaNode *node = nodes[adj_node_index];
	nodes.erase(nodes.begin() + op_index);
	delete op_node;
	
	if (op == Op::Minus) {
		CHECK_TRUE(node->ApplyMinus());
	} else if (op == Op::Plus) {
		// just ignore
	}
	
	return true;
}

bool
ReplaceNamedRanges(QVector<FormulaNode*> &input)
{
	for (int i = 0; i < input.size(); i++) {
		ods::FormulaNode *node = input[i];
		if (node->is_named_range()) {
			inst::TableNamedRange *nr = node->as_named_range();
			QVector<FormulaNode*> vec;
			const QString s = QChar('[') + nr->cell_range_address() + QChar(']');
			CHECK_TRUE(Formula::ParseString(s, vec, nr->GetSheet()));
			CHECK_TRUE((!vec.isEmpty()));
			for (int j = 0; j < vec.size(); j++) {
				input.insert(i + j, vec[j]);
			}
			input.remove(i + vec.size());
			delete node;
		}
	}
	
	return true;
}

double RoundUp(double value, int decimal_places) {
	const double multiplier = std::pow(10.0, decimal_places);
	return std::round(value * multiplier) / multiplier;
}

double CeilUp(double value, int decimal_places) {
	const double multiplier = std::pow(10.0, decimal_places);
	return std::ceil(value * multiplier) / multiplier;
}

double FloorUp(double value, int decimal_places) {
	const double multiplier = std::pow(10.0, decimal_places);
	return std::floor(value * multiplier) / multiplier;
}

//============Standard Formula Functions===============

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
	
	CHECK_TRUE_NULL(CommonForSumIfLikeFunctions_BuildUp(values, default_sheet,
		test_range_vec, cond_nodes, nullptr));
	
	PrintNodes(cond_nodes, "CountIf() Before for loop");
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
		
		FormulaNode *result = CommonForSumIfLikeFunctions_Eval(*test_node, cond_nodes);
		ods::AutoDelete result_ad(result);
		CHECK_TRUE_NULL((result != nullptr && result->is_bool()));
		const bool ok = result->as_bool();
		
		if (ok) {
			count++;
		}
	}
	
	return FormulaNode::Double(double(count));
}

FormulaNode* Date(const QVector<ods::FormulaNode*> &values)
{
	CHECK_TRUE_NULL((values.size() == 3));
	
	int arr[3] = {};
	
	for (int i = 0; i < 3; i++) {
		FormulaNode *node = values[i];
		CHECK_TRUE_NULL((node->is_double()));
		arr[i] = node->as_double();
	}
	
	QDate *d = new QDate(arr[0], arr[1], arr[2]);
	return FormulaNode::Date(d);
}

FormulaNode* DayMonthYear(const QVector<ods::FormulaNode*> &values, const DMY dmy)
{
	CHECK_TRUE_NULL((values.size() == 1));
	
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
		
		QVector<QStringRef> list = date_str->splitRef(sep);
		CHECK_TRUE_NULL((list.size() == 3));
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
	
	RET_IF_EQUAL_NULL(ret_val, -1);
	return FormulaNode::Double(ret_val);	
}

FormulaNode* If(const QVector<FormulaNode*> &values)
{
	CHECK_EQUAL_NULL(values.size(), 3);
	FormulaNode *condition = values[0];
	FormulaNode *true_node = values[1];
	FormulaNode *false_node = values[2];
	
	// Treat as 'false' zero, empty string or bool=false,
	bool flag = true;
	
	if (condition->is_bool()) {
		flag = condition->as_bool();
	} else if (condition->is_any_double()) {
		const double d = condition->as_double();
		if (IsNearlyEqual(d, 0.0)) {
			flag = false;
		}
	} else if (condition->is_string()) {
		QString *s = condition->as_string();
		flag = !s->isEmpty();
	}

	return flag ? true_node->Clone() : false_node->Clone();
}

FormulaNode* Max(const QVector<ods::FormulaNode*> &values)
{
	double max;
	bool initialize = true;
	
	for (ods::FormulaNode *value: values) {
		CHECK_TRUE_NULL(value->is_double());
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
		CHECK_TRUE_NULL(value->is_any_double());
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
	CHECK_EQUAL_NULL(values.size(), 2);
	auto *lhs_node = values[0];
	auto *rhs_node = values[1];
	if (!lhs_node->is_any_double() || !rhs_node->is_any_double()) {
		mtl_trace("Arg is not a number");
		return nullptr;
	}
	double rhs = rhs_node->as_any_double();
	RET_IF_EQUAL_NULL(rhs, 0);
	double lhs = lhs_node->as_any_double();
	double n = std::remainder(lhs, rhs); // ret type can be a float point value
	return FormulaNode::Double(double(n));
}

FormulaNode* Now()
{
	QDateTime now = QDateTime::currentDateTime();
	return FormulaNode::DateTime(new QDateTime(now));
}

FormulaNode* Power(const QVector<FormulaNode*> &values)
{
	CHECK_EQUAL_NULL(values.size(), 2);
	auto *lhs_node = values[0];
	auto *rhs_node = values[1];
	if (!lhs_node->is_any_double() || !rhs_node->is_any_double()) {
		mtl_trace("Arg is not a number");
		return nullptr;
	}
	double rhs = rhs_node->as_any_double();
	RET_IF_EQUAL_NULL(rhs, 0);
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
	CHECK_EQUAL_NULL(values.size(), 2);
	auto *lhs_node = values[0];
	auto *rhs_node = values[1];
	if (!lhs_node->is_any_double() || !rhs_node->is_any_double()) {
		mtl_trace("Arg is not a number");
		return nullptr;
	}
	double rhs = rhs_node->as_any_double();
	RET_IF_EQUAL_NULL(rhs, 0);
	double lhs = lhs_node->as_any_double();
	i64 n = lhs / rhs;
	return FormulaNode::Double(double(n));
}

FormulaNode* RoundAnyWay(const QVector<FormulaNode*> &values, const RoundType round_type)
{ // table:formula="of:=ROUND([.F35]*[.F36];2)"
	RET_IF_EQUAL_NULL(values.size(), 0);
	double number = values[0]->as_any_double();
	int places = 0;
	
	if (values.size() == 2) {
		FormulaNode *node = values[1];
		CHECK_TRUE_NULL(node->ConvertFunctionOrAddressToValue());
		if (node->is_any_double())
			places = node->as_double();
	}
	
	if (places > 0) {
		switch (round_type) {
			case RoundType::Round: {number = RoundUp(number, places); break;}
			case RoundType::Ceil: {number = CeilUp(number, places); break;}
			case RoundType::Floor: {number = FloorUp(number, places); break;}
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
	
	CHECK_TRUE_NULL(CommonForSumIfLikeFunctions_BuildUp(values, default_sheet,
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
		FormulaNode *result = CommonForSumIfLikeFunctions_Eval(*test_node, cond_nodes);
		ods::AutoDelete result_ad(result);
		CHECK_TRUE_NULL((result != nullptr && result->is_bool()));
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

FormulaNode* Today() {
	auto *p = new QDate(QDate::currentDate());
	return FormulaNode::Date(p);
}
} // ods::
