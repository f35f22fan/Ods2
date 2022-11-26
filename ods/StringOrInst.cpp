#include "StringOrInst.hpp"

#include "err.hpp"

#include "inst/Abstract.hpp"

namespace ods {

StringOrInst::StringOrInst(QStringView s)
{
	s_ = s.toString();
	is_string_ = true;
}

StringOrInst::StringOrInst(inst::Abstract *inst, const TakeOwnership to):
	inst_(inst),
	owns_inst_(to == TakeOwnership::Yes ? Owns::Yes : Owns::No) {}

StringOrInst::~StringOrInst()
{
	DeleteData();
}

void StringOrInst::AppendString(const QString &s)
{
	s_.append(s);
}

StringOrInst*
StringOrInst::Clone() const
{
	if (is_string())
		return new StringOrInst(as_string());
	
	return new StringOrInst(as_inst()->Clone(), TakeOwnership::Yes);
}

void
StringOrInst::DeleteData()
{
	if (is_string())
	{
		s_.clear();
	} else if (inst_) {
		if (owns_inst_ == Owns::Yes)
			delete inst_;
		inst_ = nullptr;
		owns_inst_ = Owns::No;
	}
}

bool
StringOrInst::Is(const Id id1, const Id id2) const
{
	if (!is_inst())
		return false;
	
	return as_inst()->Is(id1, id2);
}

void StringOrInst::SetInst(inst::Abstract *a, const TakeOwnership to)
{
	DeleteData();
	inst_ = a,
	owns_inst_ = (to == TakeOwnership::Yes) ? Owns::Yes : Owns::No;
}

void
StringOrInst::SetString(QStringView s)
{
	DeleteData();
	s_ = s.toString();
	is_string_ = true;
}

} // ods::
