#pragma once

#include "formula.hxx"

namespace ods::eval {

QVector<FormulaNode*>*
CloneVec(const QVector<FormulaNode*> &vec);

bool
CommonForSumIfLikeFunctions_BuildUp(const QVector<ods::FormulaNode*> &values,
	ods::Sheet *default_sheet,
	QVector<FormulaNode*> &test_range_vec,
	QVector<FormulaNode*> &cond_nodes,
	QVector<FormulaNode*> *sum_range_vec);

FormulaNode*
CommonForSumIfLikeFunctions_Eval(const FormulaNode &test_node,
	QVector<FormulaNode*> &cond_nodes);

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

const QVector<FunctionMeta>& GetSupportedFunctions();
bool IsNearlyEqual(double x, double y);
void NodeToStr(FormulaNode *node, QString &type_str, QString &node_str);
void PrintNodesInOneLine(const QVector<FormulaNode*> &v, const char *msg = "");
void PrintNodes(const QVector<FormulaNode*> &nodes, const QString &msg = QString());
bool ProcessIfInfixPlusOrMinus(QVector<FormulaNode*> &nodes, const int op_index);
bool ReplaceNamedRanges(QVector<FormulaNode*> &input);
double CeilUp(double value, int decimal_places);
double FloorUp(double value, int decimal_places);
double RoundUp(double value, int decimal_places);


} // ods::+eval::
