#pragma once

#include "Value.hpp"
#include "../ods.hxx"

namespace ods { // ods::
namespace formula { // ods::formula::
namespace node { // ods::formula::node::

enum class Type : uint8_t
{
	None,
	CellRef,
	Op,
	Value,
	Grouping,
};

} // ods::formula::node::
} // ods::formula::
} // ods::