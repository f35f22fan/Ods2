#include "op.hh"

#include "err.hpp"

namespace ods {
int priority(const ods::Op op)
{
	// Greatest number has highest priority.
	switch (op) {
	case Op::Ampersand: // this op's priority is just a guess!
	case Op::Equals:
	case Op::NotEquals:
	case Op::Less:
	case Op::LessOrEqual:
	case Op::Greater:
	case Op::GreaterOrEqual: return 4;
	case Op::Minus:
	case Op::Plus: return 5;
	case Op::Multiply:
	case Op::Percent:
	case Op::Divide: return 6;
	case Op::Exponent: return 7;
    default: {
        mtl_trace();
        return -1;
    }
	}
}

namespace op {

Op
From(QStringView s) {
	if (s == op_str::RefConcat)
		return Op::RefConcat;
	if (s == op_str::Less)
		return Op::Less;
	if (s == op_str::LessOrEqual)
		return Op::LessOrEqual;
	if (s == op_str::Greater)
		return Op::Greater;
	if (s == op_str::GreaterOrEqual)
		return Op::GreaterOrEqual;
	if (s == op_str::Ampersand)
		return Op::Ampersand;
	if (s == op_str::Plus)
		return Op::Plus;
	if (s == op_str::Minus)
		return Op::Minus;
	if (s == op_str::Multiply)
		return Op::Multiply;
	if (s == op_str::Divide)
		return Op::Divide;
	if (s == op_str::Exponent)
		return Op::Exponent;
	if (s == op_str::Equals)
		return Op::Equals;
	if (s == op_str::NotEquals)
		return Op::NotEquals;
	if (s == op_str::Percent)
		return Op::Percent;
	if (s == op_str::RefIntersection)
		return Op::RefIntersection;
	if (s == op_str::Colon)
		return Op::Colon;
	
	return Op::None;
}

QString
ToString(const Op op)
{
	if (op == Op::RefConcat)
		return op_str::RefConcat;
	if (op == Op::Less)
		return op_str::Less;
	if (op == Op::LessOrEqual)
		return op_str::LessOrEqual;
	if (op == Op::Greater)
		return op_str::Greater;
	if (op == Op::GreaterOrEqual)
		return op_str::GreaterOrEqual;
	if (op == Op::Ampersand)
		return op_str::Ampersand;
	if (op == Op::Plus)
		return op_str::Plus;
	if (op == Op::Minus)
		return op_str::Minus;
	if (op == Op::Multiply)
		return op_str::Multiply;
	if (op == Op::Divide)
		return op_str::Divide;
	if (op == Op::Exponent)
		return op_str::Exponent;
	if (op == Op::Equals)
		return op_str::Equals;
	if (op == Op::NotEquals)
		return op_str::NotEquals;
	if (op == Op::Percent)
		return op_str::Percent;
	if (op == Op::RefIntersection)
		return op_str::RefIntersection;
	if (op == Op::Colon)
		return op_str::Colon;
	
	mtl_trace();
	return QString();
}

}}
