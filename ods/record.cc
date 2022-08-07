#include "record.hh"

namespace ods {

Record::Record() {}
Record::~Record() {}

Records::Records() {}
Records::~Records() {}

void Records::Add(const Record r)
{
	vec_.append(r);
}

}
