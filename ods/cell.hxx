#pragma once

#include "types.hxx"

namespace ods {

const u8 CoveredBit = 1u << 0;
const u8 SelectedBit = 1u << 1;

struct DeleteRegion {
	int vec_index = -1;
	int start = -1;
	int count = -1;
};

}
