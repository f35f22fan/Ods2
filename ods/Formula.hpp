#pragma once

#include "Cell.hpp"
#include "eval.hh"
#include "formula.hxx"
#include "FormulaNode.hpp"
#include "Function.hpp"
#include "global.hxx"
#include "op.hh"

#include <QVector>

namespace ods {

// This helps detect cyclic references inside formulas
const u8 EvaluatingBit = 1u << 0;

class ODS_API Formula {
public:
	virtual ~Formula();
	Formula* Clone();
	
	void Add(const double d);
	void Add(const ods::Op op);
	void AddCloseBrace();
	void AddOpenBrace();
	void Add(ods::Cell *cell);
	void Add(QString *s);
	void AddCellRange(Cell *start, Cell *end);
	ods::Function* Add(const ods::FunctionId id);
	
	ods::Sheet* default_sheet() const { return default_sheet_; }
	const QString& error() const { return error_; }
	FormulaNode *Eval();
	
	bool evaluating() const { return bits_ & ods::EvaluatingBit; }
	
	static FormulaNode*
	EvaluateNodes(QVector<FormulaNode*> &nodes);
	
	static Formula*
	FromString(const QString &str, ods::Cell *cell);
	
	bool has_error() const { return !error_.isEmpty(); }
	bool ok() const { return error_.isEmpty(); }
	
	static bool
	ParseString(const QString &s, QVector<FormulaNode*> &result,
		Sheet *default_sheet, const ParsingSettings ps = 0);
	
	void PrintNodes(const char *msg = "") { eval::PrintNodes(nodes_, msg); }
	void PrintNodesInOneLine(const char *msg = "") { eval::PrintNodesInOneLine(nodes_, msg); }
	
	void RemoveAllNodes();
	
	// This method segfaults if called from inside
	// the dtor of ~Formula():
	QString ToXmlString();
	
private:
	Formula(ods::Cell *cell);
	
	static Reference *CellAddressOrRange(QStringView s, int &skip, Sheet *default_sheet);
	bool ProcessFormulaString(QString s, QVector<FormulaNode *> &nodes);
	
	static bool
	ParseNext(QStringView s, int &resume_at, QVector<FormulaNode *> &vec, Sheet *default_sheet,
		u8 &settings);
	
	static inst::TableNamedRange*
	StartsWithNamedRange(QStringView s, ods::Sheet *sheet);
	
	
	void evaluating(const bool flag) {
		if (flag)
			bits_ |= ods::EvaluatingBit;
		else
			bits_ &= ~ods::EvaluatingBit;
	}
	
	void SaveOriginalNodes(QVector<FormulaNode*> &nodes);
	
	QVector<FormulaNode*> nodes_;
	QString str_to_evaluate_;
	QString error_;
	ods::Cell *cell_ = nullptr;
	ods::Sheet *default_sheet_ = nullptr;
	u8 bits_ = 0;
	friend class ods::Reference;
	friend class ods::Cell;
	friend class ods::Function;
};

}
