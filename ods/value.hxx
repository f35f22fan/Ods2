#pragma once

namespace ods { // ods::
namespace value { // ods::value::

enum class Error : uint8_t
{
	None,
	BadEvaluation,
	BadFormulaString,
};

enum class Type : uint8_t
{
	Bool = 1,
	Currency,
	Date,
	Double,
	Duration,
	None, // default
	Percentage,
	String, // string support implemented as TextP
};

}} // ods:::value::