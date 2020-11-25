#pragma once

#include "decl.hxx"
#include "op.hh"

//#define DEBUG_FORMULA_PARSING
//#define DEBUG_FORMULA_EVAL
//#define DEBUG_SUMIF_LIKE_FUNCTIONS
namespace ods {
// This lets the parsing function know it's parsing
// formula function params
const u8 ParsingFunctionParams = 1u << 1;
const u8 ReachedFunctionEnd = 1u << 2;
const u8 ReachedParamSeparator = 1u << 3;


// used by ods::Formula::ParseString()
typedef u8 ParsingSettings;
const u8 TreatRemainderAsString = 1u << 0;

enum class RoundType : u8 {
	Ceil,
	Floor,
	Round
};

enum class DMY {
	Day,
	Month,
	Year
};

enum class FunctionId : u16 {
	None,
	Average,
	Concatenate,
	Count,
	CountA,
	CountBlank,
	CountIf,
	Date,
	Day,
	If,
	Max,
	Min,
	Mod,
	Month,
	Now,
	Power,
	Quotient,
	Round,
	RoundUp,
	RoundDown,
	Sum,
	SumIf,
	Product,
	Today,
	Year,
};

namespace function {
const u32 FlattenOutParamsBit = 1u << 0;
const u32 DefaultSettings = FlattenOutParamsBit;
}

struct FunctionMeta {
	const char *name;
	FunctionId id;
	u32 settings; // possible value: function::FlattenOutParamsBit
	
	FunctionMeta(const char *n, const FunctionId fi, const u32 bits = ods::function::DefaultSettings) {
		name = n;
		id = fi;
		settings = bits;
	}
};

namespace function {
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

// Formula functions:
FormulaNode* Average(const QVector<ods::FormulaNode*> &values);
FormulaNode* Concatenate(const QVector<ods::FormulaNode*> &values);
FormulaNode* Count(const QVector<ods::FormulaNode*> &values);
FormulaNode* CountA(const QVector<ods::FormulaNode*> &values);
FormulaNode* CountBlank(const QVector<ods::FormulaNode*> &values);
FormulaNode* CountIf(const QVector<FormulaNode *> &values, Sheet *default_sheet);
FormulaNode* Date(const QVector<ods::FormulaNode*> &values);
FormulaNode* DayMonthYear(const QVector<ods::FormulaNode*> &values, const DMY dmy);
FormulaNode* If(const QVector<ods::FormulaNode*> &values);
FormulaNode* Max(const QVector<FormulaNode *> &values);
FormulaNode* Min(const QVector<FormulaNode*> &values);
FormulaNode* Mod(const QVector<FormulaNode*> &values);
FormulaNode* Now();
FormulaNode* Power(const QVector<FormulaNode*> &values);
FormulaNode* Product(const QVector<FormulaNode*> &values);
FormulaNode* Quotient(const QVector<FormulaNode*> &values);
FormulaNode* RoundAnyWay(const QVector<FormulaNode*> &values, const RoundType round_type);
FormulaNode* Sum(const QVector<FormulaNode *> &values);
FormulaNode* SumIf(const QVector<FormulaNode *> &values, Sheet *default_sheet);
FormulaNode* Today();
} // function::

}
