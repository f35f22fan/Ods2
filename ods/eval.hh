#pragma once

#include "formula.hxx"
#include "global.hxx"

#include <QDateTime>

namespace ods::eval {

ODS_API QVector<FormulaNode*>*
CloneVec(const QVector<FormulaNode*> &vec);

ODS_API bool
CommonForSumIfLikeFunctions_BuildUp(const QVector<ods::FormulaNode*> &values,
	ods::Sheet *default_sheet,
	QVector<FormulaNode*> &test_range_vec,
	QVector<FormulaNode*> &cond_nodes,
	QVector<FormulaNode*> *sum_range_vec);

ODS_API FormulaNode*
CommonForSumIfLikeFunctions_Eval(const FormulaNode &test_node,
	QVector<FormulaNode*> &cond_nodes);

ODS_API bool
EvalDeepestGroup(QVector<FormulaNode*> &input);

ODS_API bool
EvalNodesByOpPrecedence(QVector<FormulaNode*> &nodes);

ODS_API bool
ExtractCellValue(ods::Cell *cell, FormulaNode &result);

ODS_API bool
ExtractAddressValues(ods::FormulaNode *node, QVector<FormulaNode *> &result);

ODS_API int
FindHighestPriorityOp(QVector<FormulaNode*> &vec);

ODS_API const FunctionMeta*
FindFunctionMeta(const FunctionId id);

ODS_API const FunctionMeta*
FindFunctionMeta(const QString &name);

ODS_API bool
FlattenOutArgs(QVector<ods::FormulaNode*> &vec);

ODS_API bool
FormatAsDateTime(const QString &format_str, QDate *date_arg, QTime *time_arg, QString &result);

ODS_API void FormatNumber(const QString &format_str, const double d, QString &result);

ODS_API const QVector<FunctionMeta>& GetSupportedFunctions();
ODS_API bool IsNearlyEqual(double x, double y);
ODS_API void NodeToStr(FormulaNode *node, QString &type_str, QString &node_str);
ODS_API void PrintNodesInOneLine(const QVector<FormulaNode*> &v, const char *msg = "");
ODS_API void PrintNodes(const QVector<FormulaNode*> &nodes, const QString &msg = QString());
ODS_API bool ProcessIfInfixPlusOrMinus(QVector<FormulaNode*> &nodes, const int op_index);
ODS_API bool ReplaceNamedRanges(QVector<FormulaNode*> &input);
ODS_API double CeilUp(double value, int decimal_places);
ODS_API double FloorUp(double value, int decimal_places);
ODS_API double RoundUp(double value, int decimal_places);


} // ods::+eval::
