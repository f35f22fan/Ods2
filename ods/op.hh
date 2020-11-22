#pragma once

#include "global.hxx"
#include "types.hxx"

#include <QString>

namespace ods {
namespace op_str {
const QString RefConcat = QLatin1String("~");
const QString Less = QLatin1String("<");
const QString LessOrEqual = QLatin1String("<=");
const QString Greater = QLatin1String(">");
const QString GreaterOrEqual = QLatin1String(">=");
const QString Ampersand = QLatin1String("&");
const QString Plus = QLatin1String("+");
const QString Minus = QLatin1String("-");
const QString Multiply = QLatin1String("*");
const QString Divide = QLatin1String("/");
const QString Exponent = QLatin1String("^");
const QString Equals = QLatin1String("=");
const QString NotEquals = QLatin1String("<>");
const QString Percent = QLatin1String("%");
const QString RefIntersection = QLatin1String("!");
const QString Colon = QLatin1String(":");

const QChar sep('|');
const QString sep_slash = QLatin1String("|\\");

// 2-digit ops must follow before 1-digit ones otherwise parsing
// won't work properly, e.g. ">=" must be before "="
const QString Regex = 
	GreaterOrEqual + sep +
	LessOrEqual + sep +
	NotEquals + sep +
	RefConcat + sep +
	Minus + sep_slash +
	Plus + sep +
	Divide + sep_slash +
	Multiply + sep +
	Ampersand + sep +
	Equals + sep +
	Less + sep +
	Greater + sep_slash +
	Exponent + sep_slash +
	Percent + sep_slash +
	RefIntersection + sep +
	Colon;
}

enum class Op : u8 {
	None,
	RefConcat,
	Less,
	LessOrEqual,
	Greater,
	GreaterOrEqual,
	Ampersand,
	Plus,
	Minus,
	Multiply,
	Divide,
	Exponent,
	Equals,
	NotEquals,
	Percent,
	RefIntersection,
	Colon
};

namespace op {
Op From(const QString &s);
Op From(const QStringRef &s);
QString ToString(const Op op);
} // ods::op::

int priority(const ods::Op op);

} // ods::
