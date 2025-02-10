#pragma once

#include "types.hxx"

namespace ods::line {

enum class Style : u8
{
	Invalid,
	None,
	Dash,
	Dashed,
	DotDash,
	DotDotDash,
	Dotted,
	Double,
	LongDash,
	Solid,
	Wave
};

}
