#include "op.hh"

#include "err.hpp"

namespace ods::op {

Op
From(const QString &s) {
	return From(s.midRef(0));
}

Op
From(const QStringRef &s) {
	if (s == op_str::Tilde)
		return Op::Tilde;
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
	if (s == op_str::Xor)
		return Op::Xor;
	if (s == op_str::Equals)
		return Op::Equals;
	if (s == op_str::Starfish)
		return Op::Starfish;
	if (s == op_str::Percent)
		return Op::Percent;
	if (s == op_str::Exclamation)
		return Op::Exclamation;
	if (s == op_str::Colon)
		return Op::Colon;
	
	return Op::None;
}

QString
ToString(const Op op)
{
	if (op == Op::Tilde)
		return op_str::Tilde;
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
	if (op == Op::Xor)
		return op_str::Xor;
	if (op == Op::Equals)
		return op_str::Equals;
	if (op == Op::Starfish)
		return op_str::Starfish;
	if (op == Op::Percent)
		return op_str::Percent;
	if (op == Op::Exclamation)
		return op_str::Exclamation;
	if (op == Op::Colon)
		return op_str::Colon;
	
	mtl_trace();
	return QString();
}
}
