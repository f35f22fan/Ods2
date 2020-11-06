#pragma once

#include "Cell.hpp"
#include "formula.hxx"
#include "FormulaNode.hpp"
#include "Function.hpp"
#include "global.hxx"
#include "op.hh"
#include "Value.hpp"

#include <QVector>

namespace ods {

// this helps detect cyclic references inside formulas.
const u8 EvaluatingBit = 1u << 0;

struct Group { // for getting the deepest group in parenthesis.
	int start;
	int count;
};

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
	
	const Value& Eval();
	
	static Formula*
	FromString(const QString &str, ods::Cell *cell);
	
	bool has_error() const { return value_.has_error(); }
	bool ok() const { return value_.ok(); }
	const QString& raw_str() const { return raw_str_; }
	void raw_str(const QString &s) { raw_str_ = s; }
	const ods::Value& value() const { return value_; }
	
private:
	Formula(ods::Cell *cell);
	
	static Address *CellAddressOrRange(QStringRef s, int &skip, Sheet *default_sheet);
	Group GetDeepestGroup() const;
	void PrintNodes() const;
	bool ProcessFormulaString(QString s);
	bool ProcessNext(QStringRef s, int &resume_at, QVector<FormulaNode> &vec);
	QString ToXmlString() const;
	
	void EvaluateNodes();
	bool evaluating() const { return bits_ & ods::EvaluatingBit; }
	void evaluating(const bool flag) {
		if (flag)
			bits_ |= ods::EvaluatingBit;
		else
			bits_ &= ~ods::EvaluatingBit;
	}
	
	QVector<FormulaNode> nodes_;
	QString raw_str_;
	ods::Value value_;
	ods::Cell *cell_ = nullptr;
	ods::Sheet *default_sheet_ = nullptr;
	u8 bits_ = 0;
	friend class ods::Cell;
	friend class ods::Function;
};

}
