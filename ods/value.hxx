#pragma once

#include "types.hxx"

namespace ods { // ods::

enum class ValueType: u8
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

}
