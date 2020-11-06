#pragma once

#include "global.hxx"
#include "types.hxx"

#include <QString>

namespace ods {

namespace op_str {
const QString Tilde = QLatin1String("~");
const QString Less = QLatin1String("<");
const QString LessOrEqual = QLatin1String("<=");
const QString Greater = QLatin1String(">");
const QString GreaterOrEqual = QLatin1String(">=");
const QString Ampersand = QLatin1String("&");
const QString Plus = QLatin1String("+");
const QString Minus = QLatin1String("-");
const QString Multiply = QLatin1String("*");
const QString Divide = QLatin1String("/");
const QString Xor = QLatin1String("^");
const QString Equals = QLatin1String("=");
const QString Starfish = QLatin1String("<>");
const QString Percent = QLatin1String("%");
const QString Exclamation = QLatin1String("!");
const QString Colon = QLatin1String(":");

const QString Regex = Tilde + QChar('|') + Minus
	+ QLatin1String("|\\") + Plus
	+ QChar('|') + Divide
	+ QLatin1String("|\\") + Multiply;
}

enum class Op : u8 {
	None,
	Tilde,
	Less,
	LessOrEqual,
	Greater,
	GreaterOrEqual,
	Ampersand,
	Plus,
	Minus,
	Multiply,
	Divide,
	Xor,
	Equals,
	Starfish,
	Percent,
	Exclamation,
	Colon
};

namespace op {
Op From(const QString &s);
Op From(const QStringRef &s);
QString ToString(const Op op);
}} // namespaces
