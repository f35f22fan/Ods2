#pragma once

#include "currency.hxx"
#include "function.hh"
#include "global.hxx"
#include "ods.hxx"
#include "types.hxx"

#include <QVector>

namespace ods {

class ODS_API Function {
public:
	virtual ~Function();
	
	static Function* TryNew(QStringRef s, int &skip, Sheet *default_sheet);
	
	static Function* CONCATENATE() { return New(FunctionId::Concatenate); }
	static Function* MAX() { return New(FunctionId::Max); }
	static Function* MIN() { return New(FunctionId::Min); }
	static Function* SUM() { return New(FunctionId::Sum); }
	static Function* PRODUCT() { return New(FunctionId::Product); }
	
	void AddArg(ods::FormulaNode *node);
	void AddArg(ods::Address *a);
	void AddArg(double d);
	void AddArg(ods::Function *f);
	void AddArg(ods::Currency *c);
	void AddArg(ods::Brace b);
	void AddArg(QString *s);
	void AddArg(QVector<FormulaNode*> *subvec);
	Function* Clone();
	FormulaNode* Eval();
	const FunctionMeta*
	meta() const { return meta_; }
	static Function* New(const FunctionId id);
	QString toString() const;
	QString toXmlString() const;
	bool valid() const { return meta_ != nullptr; }
	void PrintArgs(const QString &msg = QString());
	
private:
	Function();
	Function(const Function &src);
	void DeepCopy(ods::Function &dest, const ods::Function &src);
	
	const FunctionMeta *meta_ = nullptr;
	QVector<QVector<ods::FormulaNode*>*> *args_ = nullptr;
};



}
