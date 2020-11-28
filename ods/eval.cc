#include "eval.hh"

#include "Reference.hpp"
#include "Cell.hpp"
#include "CellRef.hpp"
#include "Formula.hpp"
#include "FormulaNode.hpp"
#include "Time.hpp"
#include "inst/TableNamedRange.hpp"

#include <QDate>
#include <QDateTime>

#include <cmath>

namespace ods::eval {

const QChar MonthMarker('Q');


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
		CHECK_TRUE(FlattenOutArgs(*sum_range_vec));
	}
	
	{
		FormulaNode *test_range = values[0];
		test_range_vec.append(test_range->Clone());
		// Now convert reference ranges to arrays of values
		CHECK_TRUE(FlattenOutArgs(test_range_vec));
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
	QVector<ods::FormulaNode*> *expression = CloneVec(cond_nodes);
	ods::AutoDeleteVecP expression_advp(expression);
	expression->prepend(test_node.Clone());
#ifdef DEBUG_SUMIF_LIKE_FUNCTIONS
	PrintNodesInOneLine(*expression);
#endif
	auto *result = Formula::EvaluateNodes(*expression);
#ifdef DEBUG_SUMIF_LIKE_FUNCTIONS
		mtl_print_node(result, "Expression result: ");
#endif
	return result;
}

//void
//AppendDateTimeParam(const QStringRef &ref, QString &result)
//{
//	if (ref ==)
//}

void
MarkMonth(QVector<QString> &tokens)
{
	const int count = tokens.size();
	
	for (int i = 0; i < count; i++) {
		QString &ref = tokens[i];
		const QChar c = ref[0];
		
		if (c != 'm' && c != 'M')
			continue;
		
		i8 minute_score = 0;
		i8 month_score = 0;
		
		if (i > 0) {
			const QChar past = tokens[i - 1].at(0).toLower();
			if (past == 's' || past == 'h')
				minute_score++;
			else if (past == 'd' || past == 'y')
				month_score++;
		}
		
		if (i < count - 1) {
			const QChar past = tokens[i + 1].at(0).toLower();
			if (past == 's' || past == 'h')
				minute_score++;
			else if (past == 'd' || past == 'y')
				month_score++;
		}
		
		if (month_score > minute_score) {
			const int sz = ref.size();
			ref = MonthMarker; // 'm'
			if (sz > 1) // "mm"
				ref.append(MonthMarker);
		}
	}
}

bool
FormatAsDateTime(const QString &format_str, QDate *date_arg, QTime *time_arg, QString &result)
{ // "mm/dd/yyyy"
//	mtl_printq2("Original String: ", format_str);
	int token_start = -1;
	const QChar dot('.');
	const QChar fwslash('/');
	const QChar colon(':');
	QVector<QString> tokens;
	struct Sep {
		int at;
		QChar c;
	};
	
	QVector<Sep> separators;
	const auto str_size = format_str.size();
	
	for (int i = 0; i < str_size; i++) {
		QChar c = format_str.at(i);
		if (c == ' ' || c == '\t') {
			if (!tokens.isEmpty()) {
				separators.append(Sep {tokens.size() + 1, c} );
				int token_size = i - token_start;
				auto sub = format_str.mid(token_start, token_size);
				//mtl_printq(sub);
				tokens.append(sub);
			}
			token_start = i + 1;
			continue;
		}
		
		if (token_start == -1) {
			token_start = i;
			continue;
		}
		
		const bool at_end = (i == (str_size - 1));
		const bool is_sep = (c == dot || c == fwslash || c == colon);
		
		if (is_sep || at_end) {
			if (is_sep)
				separators.append(Sep {tokens.size() + 1, c});
			
			if (at_end)
				i++;
			
			if (token_start < (i + 1)) {
				int token_size = i - token_start;
				auto sub = format_str.mid(token_start, token_size);
				tokens.append(sub);
			}
			token_start = i + 1;
		}
	}
	
	MarkMonth(tokens);
	
	for (int i = separators.size() - 1; i >= 0; i--) {
		Sep &sep = separators[i];
		tokens.insert(sep.at, QString(sep.c));
	}
	
	for (auto &token : tokens) {
		const QChar char0 = token[0].toLower();
		const int token_size = token.size();
		
		if (char0 == 'y') {
			if (date_arg != nullptr) {
				QString year_str = QString::number(date_arg->year());
				if (token_size > year_str.size()) {
					const int diff = token_size - year_str.size();
					for (int i = 0; i < diff; i++) {
						result.append('0');
					}
				} else if (token_size < year_str.size()) {
					year_str = year_str.left(token_size);
				}
				result.append(year_str);
			}
		} else if (char0 == MonthMarker.toLower()) {
			if (date_arg != nullptr) {
				const int month = date_arg->month();
				if (token_size == 2 && month <= 9)
					result.append('0');
				result.append(QString::number(month));
			}
		} else if (char0 == 'd') {
			if (date_arg != nullptr) {
				const int day = date_arg->day();
				if (token_size == 2 && day <= 9)
					result.append('0');
				result.append(QString::number(day));
			}
		} else if (char0 == 'h') {
			if (time_arg != nullptr) {
				const int hour = time_arg->hour();
				if (token_size == 2 && hour <= 9)
					result.append('0');
				result.append(QString::number(hour));
			}
		} else if (char0 == 'm') {
			if (time_arg != nullptr) {
				const int minute = time_arg->minute();
				if (token_size == 2 && minute <= 9)
					result.append('0');
				result.append(QString::number(minute));
			}
		} else if (char0 == 's') {
			if (time_arg != nullptr) {
				const int second = time_arg->second();
				if (token_size == 2 && second <= 9)
					result.append('0');
				result.append(QString::number(second));
			}
		} else if (char0 == 'z') {
			if (time_arg != nullptr) {
				const int ms = time_arg->msec();
				QString ms_str = QString::number(ms);
				const int diff = token_size - ms_str.size();
				for (int i = 0; i < diff; i++) {
					result.append('0');
				}
				result.append(ms_str);
			}
		} else {
			result.append(token);
		}
	}
	
	return true;
}

void
FormatNumber(const QString &format_str, const double d, QString &result)
{
	int dot = format_str.indexOf('.');
	if (dot == -1) {
		result = QString::number(i64(d));
		const int int_part = format_str.size();
		
		while (result.size() < int_part) {
			result.prepend('0');
		}
		
		return;
	}
	const int int_part = dot;
	const int decimals = format_str.size() - dot - 1;
	result = QString::number(d, 'f', decimals);
	int int_size = result.indexOf('.');
	
	if (int_size == -1)
		int_size = result.size();
	
	for (int i = int_size; i < int_part; i++) {
		result.prepend('0');
	}
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
	auto *reference = node->as_reference();
	
	if (reference->is_cell_range()) {
#ifdef DEBUG_FORMULA_EVAL
		auto ba = reference->toString().toLocal8Bit();
		mtl_info("Cell Reference Range: %s", ba.data());
#endif
		QVector<ods::Cell*> cells;
		CHECK_TRUE(reference->GenCells(cells));
		
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
		auto *cell_ref = reference->cell();
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
		} else if (node->is_reference()) {
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
// "flattened out", e.g. an "reference range" node shouldn't be replaced
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
	FunctionMeta ("AND", FunctionId::And),
	FunctionMeta ("OR", FunctionId::Or),
	FunctionMeta ("COLUMNS", FunctionId::Columns, 0),
	FunctionMeta ("ROWS", FunctionId::Rows, 0),
	FunctionMeta ("TRUE", FunctionId::True),
	FunctionMeta ("FALSE", FunctionId::False),
	FunctionMeta ("NOT", FunctionId::Not),
	FunctionMeta ("INDIRECT", FunctionId::Indirect),
	FunctionMeta ("OFFSET", FunctionId::Offset, 0),
	FunctionMeta ("TEXT", FunctionId::Text),
	FunctionMeta ("MATCH", FunctionId::Match, 0),
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
	if (node->is_reference()) {
		type_str = QLatin1String("Reference");
		node_str = node->as_reference()->toString();
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
	} else if (node->is_error()) {
		type_str = QLatin1String("Formula Error");
		node_str = node->toString();
	} else if (node->is_none()) {
		type_str = QLatin1String("Empty");
		node_str = QString();
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

bool ReplaceNamedRanges(QVector<FormulaNode*> &input)
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

}
