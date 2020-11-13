#include "function.hh"

#include "Address.hpp"
#include "Cell.hpp"
#include "CellRef.hpp"
#include "Formula.hpp"
#include "FormulaNode.hpp"

namespace ods::function {

/* bool
Apply(ods::Value &lhs, const ods::Op op, const ods::Value &rhs)
{
	if (lhs.is_double() && rhs.is_double()) {
		double d1 = *lhs.as_double();
		double d2 = *rhs.as_double();
		double result;
		if (op == Op::Plus) {
			result = d1 + d2;
		} else if (op == Op::Minus) {
			result = d1 - d2;
		} else if (op == Op::Multiply) {
			result = d1 * d2;
		} else if (op == Op::Divide) {
			result = d1 / d2;
		} else {
			mtl_trace("Operation not implemented");
			return false;
		}
		lhs.SetDouble(result);
		return true;
	}
	mtl_trace("Both sides not numbers");
	return false;
} */

bool
EvalDeepestGroup(QVector<FormulaNode*> &input)
{
mtl_line();
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
mtl_line();
			ods::FormulaNode *value = f->Eval();
			CHECK_PTR(value);
			ret.append(value);
			delete node;
		} else {
			ret.append(input[i]);
		}
	}
	
	input.erase(input.begin() + start, input.begin() + start + count);
	PrintNodes(input);
	
	CHECK_TRUE(EvalNodesByOpPrecedence(ret));
	CHECK_TRUE((ret.size() == 1));
	auto *node = ret[0];
	input.insert(input.begin() + start, node);
	auto ba = node->toCompactString().toLocal8Bit();
	mtl_line("===============RESULT: %s", ba.data());
	return true;
}

bool
EvalNodesByOpPrecedence(QVector<FormulaNode*> &nodes)
{
	int iter = 0;
	while (nodes.size() > 1)
	{
		if (iter++ > 10)
			break;
		mtl_line("nodes count: %d", nodes.size());
		PrintNodesInOneLine(nodes, "EvalNodesByOpPrecedence() while() start");
		const int op_index = FindHighestPriorityOp(nodes);
		RET_IF_EQUAL(op_index, -1);
		RET_IF_EQUAL(op_index, nodes.size() - 1);
		mtl_line("op_index: %d", op_index);
		
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
	PrintNodesInOneLine(nodes, "End of EvalNodesByOpPrecedence() func end");
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
	} else {
		mtl_warn("Cell not a number");
		return false;
	}
	
	return true;
}

bool
ExtractValue(ods::FormulaNode *node, QVector<ods::FormulaNode*> &result)
{
	if (node->is_address()) {
		auto *address = node->as_address();
		
		if (address->is_cell_range()) {
			auto ba = address->toString().toLocal8Bit();
			mtl_line("Cell Address Range: %s", ba.data());
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
	} else if (node->is_double()) {
		double d = node->as_double();
		auto *v = new ods::FormulaNode();
		v->SetDouble(d);
		result.append(v);
	} else if (node->is_function()) {
		ods::Function *f = node->as_function();
		ods::FormulaNode *value = f->Eval();
		CHECK_PTR(value);
		if (value->is_none()) {
			mtl_trace();
			return false;
		}
		result.append(value->Clone());
	} else {
		mtl_trace();
		return false;
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
FindFunctionMeta(const QString &name)
{
	for (const FunctionMeta &fi: GetSupportedFunctions()) {
		if (name == fi.name) {
			return &fi;
		}
	}
	
	return nullptr;
}

const QVector<FunctionMeta>&
GetSupportedFunctions() {
	static QVector<FunctionMeta> v = {
		FunctionMeta {"SUM", FunctionId::Sum},
		FunctionMeta {"MAX", FunctionId::Max},
		FunctionMeta {"MIN", FunctionId::Min},
	};
	return v;
}

bool
ProcessIfInfixPlusOrMinus(QVector<FormulaNode*> &nodes,
	const int op_index)
{
	FormulaNode *op_node = nodes[op_index];
	const ods::Op op = op_node->as_op();
	
	if (op != Op::Minus && op != Op::Plus) {
		auto ba = ods::op::ToString(op).toLocal8Bit();
		mtl_warn("Don't know what to do with %s", ba.data());
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

void PrintNodesInOneLine(const QVector<FormulaNode *> &v, const char *msg)
{
	QString s;
	QString separator = QString(MTL_COLOR_GREEN) + "|" + MTL_COLOR_DEFAULT;
	for (ods::FormulaNode *node : v) {
		s.append(node->toCompactString()).append(separator);
	}
	
	auto ba = s.toLocal8Bit();
	auto sep_ba = separator.toLocal8Bit();
	mtl_line("%s%s %s", sep_ba.data(), ba.data(), msg);
}

void PrintNodes(const QVector<FormulaNode*> &nodes)
{
	mtl_line("Formula nodes:");
	
	for (int i = 0; i < nodes.size(); i++) {
		const FormulaNode *node = nodes[i];
		QString s;
		QString type;
		if (node->is_address()) {
			type = "Address";
			s = node->as_address()->toString();
		} else if (node->is_function()) {
			type = "Function";
			s = node->as_function()->toString();
		} else if (node->is_double()) {
			type = "Number";
			s = QString::number(node->as_double());
		} else if (node->is_op()) {
			type = "Op";
			s = ods::op::ToString(node->as_op());
		} else if (node->is_brace()) {
			type = "Brace";
			s = ods::ToString(node->as_brace());
		} else if (node->is_none()) {
			type = "None";
			s = "[!!]";
		}
		
		auto ba = s.toLocal8Bit();
		auto type_ba = type.toLocal8Bit();
		printf("(%d) %s: \"%s\"\n", i+1, type_ba.data(), ba.data());
	}
}

//===================Formula Functions==============================

FormulaNode* Max(const QVector<ods::FormulaNode*> &values, ods::FormulaNode *result)
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
	
	mtl_line("%.2f", max);
	result->SetDouble(max);
	return result;
}

FormulaNode* Min(const QVector<FormulaNode *> &values, FormulaNode *result)
{
	double min;
	bool initialize = true;
	
	for (ods::FormulaNode *value: values) {
		CHECK_TRUE_NULL(value->is_double());
		if (initialize) {
			initialize = false;
			min = value->as_double();
		} else {
			double d = value->as_double();
			if (d < min)
				min = d;
		}
	}
	
	mtl_line("%.2f", min);
	result->SetDouble(min);
	return result;
}

FormulaNode* Sum(const QVector<ods::FormulaNode*> &values, ods::FormulaNode *result)
{
	double d = 0;
	
	for (ods::FormulaNode *value: values) {
		CHECK_TRUE_NULL(value->is_double());
		d += value->as_double();
	}
	
	mtl_line("%.2f", d);
	result->SetDouble(d);
	return result;
}
}
