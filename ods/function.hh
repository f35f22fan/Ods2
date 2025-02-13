#pragma once

#include "decl.hxx"
#include "formula.hxx"
#include "op.hh"

namespace ods::function {
FormulaNode* And(const QVector<ods::FormulaNode*> &values);
FormulaNode* Average(const QVector<ods::FormulaNode*> &values);
FormulaNode* Bool(const bool flag);
FormulaNode* ColumnsRows(const QVector<ods::FormulaNode*> &values, const ColsOrRows arg);
FormulaNode* Concatenate(const QVector<ods::FormulaNode*> &values);
FormulaNode* Count(const QVector<ods::FormulaNode*> &values);
FormulaNode* CountA(const QVector<ods::FormulaNode*> &values);
FormulaNode* CountBlank(const QVector<ods::FormulaNode*> &values);
FormulaNode* CountIf(const QVector<FormulaNode *> &values, Sheet *default_sheet);
FormulaNode* Cos(const QVector<FormulaNode*> &values);
FormulaNode* Cot(const QVector<FormulaNode*> &values);
FormulaNode* Date(const QVector<ods::FormulaNode*> &values);
FormulaNode* DayMonthYear(const QVector<ods::FormulaNode*> &values, const DMY dmy);
FormulaNode* If(const QVector<ods::FormulaNode*> &values);
FormulaNode* Indirect(const QVector<FormulaNode*> &values, ods::Formula *formula);
FormulaNode* Match(const QVector<ods::FormulaNode*> &values);
FormulaNode* Max(const QVector<FormulaNode *> &values);
FormulaNode* Min(const QVector<FormulaNode*> &values);
FormulaNode* Mod(const QVector<FormulaNode*> &values);
FormulaNode* Not(const QVector<FormulaNode*> &values);
FormulaNode* Now();
FormulaNode* Offset(const QVector<FormulaNode*> &values);
FormulaNode* Or(const QVector<FormulaNode*> &values);
FormulaNode* Power(const QVector<FormulaNode*> &values);
FormulaNode* Product(const QVector<FormulaNode*> &values);
FormulaNode* Quotient(const QVector<FormulaNode*> &values);
FormulaNode* RoundAnyWay(const QVector<FormulaNode*> &values, const RoundType round_type);
FormulaNode* Sin(const QVector<FormulaNode*> &values);
FormulaNode* Sum(const QVector<FormulaNode*> &values);
FormulaNode* SumIf(const QVector<FormulaNode*> &values, Sheet *default_sheet);
FormulaNode* Tan(const QVector<FormulaNode*> &values);
FormulaNode* Text(const QVector<FormulaNode*> &values);
FormulaNode* Today();
} // ods::+function::

