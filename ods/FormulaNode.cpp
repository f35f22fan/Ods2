#include "FormulaNode.hpp"

namespace ods {

FormulaNode::FormulaNode() {}
FormulaNode::~FormulaNode() {
	if (is_address())
		delete data_.address;
	else if (is_function())
		delete data_.function;
	type_ = Type::None;
}

FormulaNode::FormulaNode(const FormulaNode &c)
{
	type_ = c.type_;
	
	if (c.is_address()) {
		data_.address = c.data_.address->Clone();
	} else if (c.is_function())
		data_.function = c.data_.function->Clone();
	else if (c.is_number())
		data_.number = c.data_.number;
	else if (c.is_op())
		data_.op = c.data_.op;
	else if (c.is_brace())
		data_.brace = c.data_.brace;
	else {
		mtl_trace();
	}
}

FormulaNode
FormulaNode::Address(ods::Address *a)
{
	FormulaNode c;
	c.type_ = Type::Address;
	c.data_.address = a;
	return c;
}

FormulaNode
FormulaNode::Function(ods::Function *f)
{
	FormulaNode c;
	c.type_ = Type::Function;
	c.data_.function = f;
	return c;
}

FormulaNode FormulaNode::Number(const double d)
{
	FormulaNode c;
	c.type_ = Type::Number;
	c.data_.number = d;
	
	return c;
}

FormulaNode FormulaNode::Op(const ods::Op op)
{
	FormulaNode c;
	c.type_ = Type::Op;
	c.data_.op = op;
	
	return c;
}

FormulaNode
FormulaNode::Brace(ods::Brace p)
{
	FormulaNode c;
	c.type_ = Type::Brace;
	c.data_.brace = p;
	
	return c;
}

}
