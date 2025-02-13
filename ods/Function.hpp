#pragma once

#include "currency.hxx"
#include "eval.hh"
#include "function.hh"
#include "global.hxx"
#include "ods.hxx"
#include "types.hxx"

#include <QVector>

namespace ods {

class ODS_API Function {
public:
	virtual ~Function();

	static Function *TryNew(QStringView s, int &skip, Sheet *default_sheet);

	void AddArg(ods::FormulaNode *node);
	void AddArg(ods::Reference *a);
	void AddArg(ods::Cell *cell);
	void AddArgRange(ods::Cell *from, ods::Cell *till);
	void AddArg(double d);
	void AddArg(i32 n) { AddArg(double(n)); }
	ods::Function* AddArg(const ods::FunctionId id);
	void AddArg(ods::Currency *c);
	void AddArg(ods::Brace b);
	void AddArg(ods::Op op);
	void AddArg(QString s);
	void AddArg(const char *s) { return AddArg(QString(s)); }
	void AddArg(QVector<FormulaNode*> *subvec);
	Function* Clone();
	ods::Sheet* default_sheet() const;
	const FunctionMeta* meta() const { return meta_; }
	QString toString() const;
	QString toXmlString() const;
	bool valid() const { return meta_ != nullptr; }
	void PrintArgs(const QString &msg = QString()) const;
	
private:
	Function();
	Function(const Function &src);
	static Function* New(const FunctionId id);
	bool DeepCopy(ods::Function &dest, const ods::Function &src);
	FormulaNode* Eval();
	FormulaNode* ExecOpenFormulaFunction(QVector<FormulaNode *> &fn_args);
	
	// set by ods::Formula when added to it.
	ods::Formula *parent_formula_ = nullptr;
	const FunctionMeta *meta_ = nullptr;
	QVector<QVector<ods::FormulaNode*>*> *args_ = nullptr;
	
	// ods::Formula calls into private Function::Eval()
	friend class ods::Formula;
	friend class ods::FormulaNode;
	friend bool ods::eval::EvalDeepestGroup(QVector<ods::FormulaNode*>&);
	friend bool ods::eval::FlattenOutArgs(QVector<ods::FormulaNode*> &vec);
};



}
