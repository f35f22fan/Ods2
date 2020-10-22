#pragma once

#include "decl.hxx"
#include "err.hpp"
#include "formula.hxx"
#include "global.hxx"
#include "ods.hxx"
#include "value.hxx"

#include "formula/decl.hxx"

#include <QDateTime>

namespace ods { // ods::

class ODS_API Formula
{
public:
	virtual ~Formula();
	
	void
	Add(ods::Cell *cell, ods::Sheet *sheet = nullptr);
	
	void
	Add(const double d);
	
	void
	Add(const ods::Grouping g);
	
	void
	Add(const ods::Op op);
	
	Formula*
	Clone(ods::Cell *cell = nullptr) const;
	
	ods::value::Error
	error() const { return error_; }
	
	void
	error(const ods::value::Error e) { error_ = e; }
	
	void
	Eval(formula::Value &result);
	
	static Formula*
	FromString(const QString &s, ods::Cell *parent);
	
	bool
	has_error() const { return error_ != value::Error::None; }
	
	const QString&
	str() const { return str_; }
	
	QString
	toString() const;

private:
	NO_ASSIGN_COPY_MOVE(Formula);
	
	Formula();
	Formula(ods::Cell *cell);
	
	formula::CellRef*
	CreateCellRef(const QString &address);
	
	void
	CreateNodes(const QString &str, QVector<formula::Node*> &vec);
	
	bool
	EnclosedInParentheses(QVector<formula::Node*> &vec, const int start,
		int *real_start, int *real_end) const;
	
	void
	Eval3Nodes(QVector<formula::Node*> &vec, const int start, formula::Value &result);
	
	void
	EvalDeepest(QVector<formula::Node*> &vec, formula::Value &result);
	
	void
	PrintNodes(QVector<formula::Node*> &vec);
	
	void
	SeekInParentheses(QVector<formula::Node*> &vec, int &start, int &end, bool &has_parentheses);
	
	int
	SeekValue(const QString &str, const int start_char_index, formula::Value &value);
	
	QString str_;
	ods::Cell *cell_ = nullptr;
	value::Error error_ = value::Error::None;
	friend class ods::Cell;
	
	// ((C5+B5)/A5)*(C4+B4*A3)+B3-C3
	// ((10+0.5)/3)*(4.5+2.4*22.3)+8-6
};

} // ods::
