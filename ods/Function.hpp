#pragma once

#include "currency.hxx"
#include "function.hh"
#include "global.hxx"
#include "ods.hxx"
#include "types.hxx"

#include <QVector>

namespace ods {

//"of:=SUM([.B1];20.9;-2.4 + 3 * MAX(2; 4);[.B2];[.C1:.C2];MIN([.A1];5)) * (-3 + 2)"
class ODS_API Function {
public:
	virtual ~Function();
	
	static Function* TryNew(QStringRef s, int &skip, Sheet *default_sheet);
	static Function* SUM() { return New(FunctionId::Sum); }
	static Function* PRODUCT() { return New(FunctionId::Product); }
	void AddArg(ods::FormulaNode *node);
	void AddArg(ods::Address *a);
	void AddArg(double d);
	void AddArg(ods::Function *f);
	void AddArg(ods::Currency *c);
	void AddArg(ods::Brace b);
	void AddArg(const QString &s);
	void AddArg(QVector<FormulaNode*> *subvec);
	Function* Clone();
	FormulaNode* Eval();
	const FunctionMeta*
	meta() const { return meta_; }
	static Function* New(const FunctionId id);
	QString toString() const;
	QString toXmlString() const;
	bool valid() const { return meta_ != nullptr; }
	
private:
	Function();
	Function(const Function &src);
	void DeepCopy(ods::Function &dest, const ods::Function &src);
	
	const FunctionMeta *meta_ = nullptr;
	QVector<QVector<ods::FormulaNode*>*> *args_;
};



}
