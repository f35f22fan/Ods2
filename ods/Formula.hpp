#pragma once

#include "Cell.hpp"
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
	void Add(Function f);
	void Add(ods::Cell *cell, Sheet *sheet = nullptr);
	FormulaNode *Eval();
	
	bool evaluating() const { return bits_ & ods::EvaluatingBit; }
	
	static Formula*
	FromString(const QString &str, ods::Cell *cell);
	
	bool has_error() const { return !error_.isEmpty(); }
	bool ok() const { return error_.isEmpty(); }
	const QString& raw_str() const { return raw_str_; }
	void raw_str(const QString &s) { raw_str_ = s; }
	
	// This method segfaults if called from inside
	// the dtor of ~Formula():
	QString ToXmlString() const;
	FormulaNode* value() const { return value_; }
	
private:
	Formula(ods::Cell *cell);
	
	static Address *CellAddressOrRange(QStringRef s, int &skip, Sheet *default_sheet);
	bool ProcessFormulaString(QString s);
	
	static bool
	DecodeNext(QStringRef s, int &resume_at, QVector<FormulaNode *> &vec, Sheet *default_sheet,
		u8 &settings);
	
	void EvaluateNodes();
	void evaluating(const bool flag) {
		if (flag)
			bits_ |= ods::EvaluatingBit;
		else
			bits_ &= ~ods::EvaluatingBit;
	}
	
	QVector<FormulaNode*> nodes_;
	QString raw_str_;
	QString error_;
	FormulaNode *value_ = nullptr;
	ods::Cell *cell_ = nullptr;
	ods::Sheet *default_sheet_ = nullptr;
	u8 bits_ = 0;
	friend class ods::Cell;
	friend class ods::Function;
};

}
