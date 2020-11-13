#pragma once

#include "function.hh"
#include "global.hxx"
#include "types.hxx"

#include <QVector>

//#define DEBUG_FORMULA

namespace ods {

//"of:=SUM([.B1];20.9;-2.4 + 3 * MAX(2; 4);[.B2];[.C1:.C2];MIN([.A1];5)) * (-3 + 2)"
class ODS_API Function {
public:
	Function();
	Function(const Function &src);
	virtual ~Function();
	
	static Function* TryNew(QStringRef s, int &skip, Sheet *default_sheet);
	
	Function* Clone();
	
	bool Eval();
	
	const FunctionMeta*
	meta() const { return meta_; }
	
	QString
	toString() const;
	
	QString
	toXmlString() const;
	
	bool valid() const { return meta_ != nullptr; }
	
	FormulaNode* value() const { return value_; }
	
private:
	void DeepCopy(ods::Function &dest, const ods::Function &src);
	
	const FunctionMeta *meta_ = nullptr;
	QVector<QVector<ods::FormulaNode*>*> *params_;
	ods::FormulaNode *value_ = nullptr;
};



}
