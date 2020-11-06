#pragma once

#include "global.hxx"
#include "types.hxx"
#include "Value.hpp"

#include <QVector>

namespace ods {

typedef int(*FunctionPtr)(QStringRef s, int start_at);

enum class FunctionId : u16 {
	None,
	Min,
	Sum,
};

struct FunctionMeta {
	const char *name;
	FunctionId id;
	FunctionPtr ptr;
};

const FunctionMeta* FindFunctionMeta(const QString &name);
const QVector<FunctionMeta>& GetSupportedFunctions();

//"of:=SUM([.B1];20.9;-2.4 + 3 * MAX(2; 4);[.B2];[.C1:.C2];MIN([.A1];5)) * (-3 + 2)"
class ODS_API Function {
public:
	Function();
	Function(const Function &src);
	virtual ~Function();
	
	// returns num of chars to skip, -1 on error.
	static int
	TryNew(QStringRef s, Formula *host, Function &retval);
	
	Function* Clone();
	
	const ods::Value& Eval();
	
	const FunctionMeta*
	meta() const { return meta_; }
	
	QString
	toString() const;
	
	bool valid() const { return meta_ != nullptr; }
	
private:
	void DeepCopy(ods::Function &dest, const ods::Function &src);
	
	void Sum();
	
	const FunctionMeta *meta_ = nullptr;
	QVector<ods::FormulaNode> params_;
	ods::Value value_;
};



}
