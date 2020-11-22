#pragma once

#include "decl.hxx"
#include "op.hh"

//#define DEBUG_FORMULA_PARSING
//#define DEBUG_FORMULA_EVAL
#define DEBUG_SUMIF_LIKE_FUNCTIONS
namespace ods {
// This lets the parsing function know it's parsing
// formula function params
const u8 ParsingFunctionParams = 1u << 1;
const u8 ReachedFunctionEnd = 1u << 2;
const u8 ReachedParamSeparator = 1u << 3;


// used by ods::Formula::ParseString()
typedef u8 ParsingSettings;
const u8 TreatRemainderAsString = 1u << 0;

enum class FunctionId : u16 {
	None,
	Concatenate,
	Count,
	CountA,
	Date,
	If,
	Max,
	Min,
	Mod,
	Now,
	Power,
	Quotient,
	Sum,
	SumIf,
	Product,
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

// Formula functions:
FormulaNode* Concatenate(const QVector<ods::FormulaNode*> &values);
FormulaNode* Count(const QVector<ods::FormulaNode*> &values);
FormulaNode* CountA(const QVector<ods::FormulaNode*> &values);
FormulaNode* Date(const QVector<ods::FormulaNode*> &values);
FormulaNode* If(const QVector<ods::FormulaNode*> &values);
FormulaNode* Max(const QVector<FormulaNode *> &values);
FormulaNode* Min(const QVector<FormulaNode*> &values);
FormulaNode* Mod(const QVector<FormulaNode*> &values);
FormulaNode* Now();
FormulaNode* Power(const QVector<FormulaNode*> &values);
FormulaNode* Product(const QVector<FormulaNode*> &values);
FormulaNode* Quotient(const QVector<FormulaNode*> &values);
FormulaNode* Sum(const QVector<FormulaNode *> &values);
FormulaNode* SumIf(const QVector<FormulaNode *> &values, Sheet *default_sheet);

} // function::

}
