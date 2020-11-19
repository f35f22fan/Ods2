#include "function.hh"

#include "Address.hpp"
#include "Cell.hpp"
#include "CellRef.hpp"
#include "Formula.hpp"
#include "FormulaNode.hpp"

namespace ods::function {

bool
EvalDeepestGroup(QVector<FormulaNode*> &input)
{
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
		if (value != nullptr)
			result = *value;
	} else if (cell->is_any_double()) {
		double d = *cell->as_double();
		result.SetDouble(d);
	} else if (cell->is_string()) {
		result.SetString(new QString(*cell->GetFirstString()));
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

		if (node->is_function()) {
			Function *f = node->as_function();
			ods::FormulaNode *value = f->Eval();
			delete f;
			CHECK_PTR(value);
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
	static const QVector<FunctionMeta> v = {
	FunctionMeta {"SUM", FunctionId::Sum},
	FunctionMeta {"MAX", FunctionId::Max},
	FunctionMeta {"MIN", FunctionId::Min},
	FunctionMeta {"PRODUCT", FunctionId::Product},
	FunctionMeta {"CONCATENATE", FunctionId::Concatenate},
	FunctionMeta {"DATE", FunctionId::Date},
	FunctionMeta {"NOW", FunctionId::Now},
	};
	return v;
}

void
NodeToStr(FormulaNode *node, QString &type_str, QString &node_str)
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

bool
ProcessIfInfixPlusOrMinus(QVector<FormulaNode*> &nodes,
	const int op_index)
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

//============Standard Formula Functions===============

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

FormulaNode* Date(const QVector<ods::FormulaNode*> &values)
{
	CHECK_TRUE_NULL((values.size() == 3));
	
	int arr[3] = {};
	
	for (int i = 0; i < 3; i++) {
		FormulaNode *node = values[i];
		CHECK_TRUE_NULL((node->is_double()));
		arr[i] = node->as_double();
	}
	
	auto *result = new ods::FormulaNode();
	QDate *d = new QDate(arr[0], arr[1], arr[2]);
	result->SetDate(d);
	return result;
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
	
#ifdef DEBUG_FORMULA_EVAL
	mtl_info("%.2f", max);
#endif
	auto *result = new ods::FormulaNode();
	result->SetDouble(max);
	return result;
}

FormulaNode* Min(const QVector<FormulaNode *> &values)
{
	double min;
	bool initialize = true;
	
	for (ods::FormulaNode *value: values) {
		CHECK_TRUE_NULL(value->is_any_double());
		if (initialize) {
			initialize = false;
			min = value->as_any_double();
		} else {
			double d = value->as_any_double();
			if (d < min)
				min = d;
		}
	}
#ifdef DEBUG_FORMULA_EVAL
	mtl_info("%.2f", min);
#endif
	auto *result = new ods::FormulaNode();
	result->SetDouble(min);
	return result;
}

FormulaNode* Now() {
	QDateTime now = QDateTime::currentDateTime();
	return FormulaNode::DateTime(new QDateTime(now));
}

FormulaNode* Product(const QVector<ods::FormulaNode*> &values)
{
	double d = 0;
	
	for (int i = 0; i < values.size(); i++) {
		ods::FormulaNode *node = values[i];
		
		if (!node->is_any_double()) {
			auto ba = node->toString().toLocal8Bit();
			mtl_info("Node is non double: \"%s\"", ba.data());
			return nullptr;
		}
		
		//CHECK_TRUE_NULL(node->is_any_double());
		
		if (i == 0) {
			d = node->as_any_double();
		} else {
			d *= node->as_any_double();
		}
	}
#ifdef DEBUG_FORMULA_EVAL
	mtl_info("%.2f", d);
#endif
	auto *result = new ods::FormulaNode();
	result->SetDouble(d);
	return result;
}

FormulaNode* Sum(const QVector<ods::FormulaNode*> &values)
{
	double d = 0;
	
	for (ods::FormulaNode *node: values) {
		
		if (!node->is_any_double()) {
			auto ba = node->toString().toLocal8Bit();
			mtl_info("Node is non double: \"%s\"", ba.data());
		}
		
		CHECK_TRUE_NULL(node->is_any_double());
		d += node->as_any_double();
	}
#ifdef DEBUG_FORMULA_EVAL
	mtl_info("%.2f", d);
#endif
	auto *result = new ods::FormulaNode();
	result->SetDouble(d);
	return result;
}

}
