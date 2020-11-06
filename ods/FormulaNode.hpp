#pragma once

#include "Address.hpp"
#include "err.hpp"
#include "Function.hpp"
#include "formula.hxx"
#include "op.hh"
#include "ods.hxx"

namespace ods {

class FormulaNode {
public:
	FormulaNode();
	FormulaNode(const FormulaNode &c);
	virtual ~FormulaNode();
	
	ods::Address* as_address() const { return data_.address; }
	ods::Function* as_function() const { return data_.function; }
	double as_number() const { return data_.number; }
	ods::Op as_op() const { return data_.op; }
	ods::Brace as_brace() const { return data_.brace; }
	
	static FormulaNode Address(ods::Address *a);
	static FormulaNode Function(ods::Function *f);
	static FormulaNode Number(const double d);
	static FormulaNode Op(const ods::Op op);
	static FormulaNode Brace(ods::Brace p);
	
	bool is_address() const { return type_ == Type::Address; }
	bool is_function() const { return type_ == Type::Function; }
	bool is_number() const { return type_ == Type::Number; }
	bool is_none() const { return type_ == Type::None; }
	bool is_op() const { return type_ == Type::Op; }
	bool is_brace() const { return type_ == Type::Brace; }
private:
	
	union Data {
		ods::Address *address;
		ods::Function *function;
		double number;
		ods::Op op;
		ods::Brace brace;
		Data() { memset(this, 0, sizeof(Data)); }
		~Data() {}
	};
	
	enum class Type : u8 {
		None,
		Address,
		Function,
		Op,
		Number,
		Brace,
	};
	
	Type type_ = Type::None;
	Data data_;
};

}
