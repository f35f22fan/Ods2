#pragma once

#include "decl.hxx"
#include "op.hh"

//#define DEBUG_FORMULA_PARSING
//#define DEBUG_FORMULA_EVAL

namespace ods {
// This lets the parsing function know it's parsing
// formula function params
const u8 ParsingFunctionParams = 1u << 1;
const u8 ReachedFunctionEnd = 1u << 2;
const u8 ReachedParamSeparator = 1u << 3;
//const u8 AppendToPastArgument = 1u << 4;

enum class FunctionId : u16 {
	None,
	Concatenate,
	Max,
	Min,
	Sum,
	Product,
};

struct FunctionMeta {
	const char *name;
	FunctionId id;
};

namespace function {

bool
EvalNodesByOpPrecedence(QVector<FormulaNode*> &nodes);

bool
ExtractCellValue(ods::Cell *cell, FormulaNode &result);

bool
ExtractAddressValues(ods::FormulaNode *node, QVector<FormulaNode *> &result);

int
FindHighestPriorityOp(QVector<FormulaNode*> &vec);

const FunctionMeta*
FindFunctionMeta(const FunctionId id);

const FunctionMeta*
FindFunctionMeta(const QString &name);

bool
FlattenOutArgs(QVector<ods::FormulaNode*> &vec);

bool
EvalDeepestGroup(QVector<FormulaNode*> &input);

const QVector<FunctionMeta>&
GetSupportedFunctions();
void NodeToStr(FormulaNode *node, QString &type_str, QString &node_str);
void PrintNodesInOneLine(const QVector<FormulaNode*> &v, const char *msg = "");
void PrintNodes(const QVector<FormulaNode*> &nodes, const QString &msg = QString());
bool ProcessIfInfixPlusOrMinus(QVector<FormulaNode*> &nodes, const int op_index);

// Formula functions:
FormulaNode* Concatenate(const QVector<ods::FormulaNode*> &values);
FormulaNode* Max(const QVector<FormulaNode *> &values);
FormulaNode* Min(const QVector<FormulaNode*> &values);
FormulaNode* Product(const QVector<FormulaNode*> &values);
FormulaNode* Sum(const QVector<FormulaNode *> &values);
}}
