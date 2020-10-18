#pragma once

#include "types.hxx"

namespace ods {

const u8 CoveredBit = 1u << 0;
const u8 SelectedBit = 1u << 1;

struct DeleteRegion {
	int vec_index;
	int start;
	int count;
};

}
