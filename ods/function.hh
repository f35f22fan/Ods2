#pragma once

#include "decl.hxx"
#include "op.hh"

namespace ods {
// This lets the parsing function know it's parsing
// formula function params
const u8 ParsingFunctionParams = 1u << 1;
const u8 ReachedFunctionEnd = 1u << 2;
const u8 ReachedParamSeparator = 1u << 3;

enum class FunctionId : u16 {
	None,
	Max,
	Min,
	Sum,
};

struct FunctionMeta {
	const char *name;
	FunctionId id;
};

namespace function {
//bool Apply(ods::Value &lhs, const ods::Op op, const ods::Value &rhs);

bool
EvalNodesByOpPrecedence(QVector<FormulaNode*> &nodes);

bool
ExtractCellValue(ods::Cell *cell, FormulaNode &result);

bool
ExtractValue(ods::FormulaNode *node, QVector<FormulaNode *> &result);

int
FindHighestPriorityOp(QVector<FormulaNode*> &vec);

const
FunctionMeta* FindFunctionMeta(const QString &name);

bool
EvalDeepestGroup(QVector<FormulaNode*> &input);

const QVector<FunctionMeta>&
GetSupportedFunctions();

void PrintNodesInOneLine(const QVector<FormulaNode*> &v, const char *msg = "");
void PrintNodes(const QVector<FormulaNode*> &nodes);
bool ProcessIfInfixPlusOrMinus(QVector<FormulaNode*> &nodes, const int op_index);

// Formula functions:
FormulaNode* Sum(const QVector<FormulaNode *> &values, FormulaNode *result);
FormulaNode* Max(const QVector<FormulaNode *> &values, FormulaNode *result);
FormulaNode* Min(const QVector<FormulaNode*> &values, FormulaNode *result);
}}
