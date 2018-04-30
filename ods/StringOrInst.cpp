#include "StringOrInst.hpp"

#include "err.hpp"

#include "inst/Abstract.hpp"

namespace ods {

StringOrInst::StringOrInst(const QString &s)
{
	type_ = StringOrInstType::String;
	data_ = new QString(s);
}

StringOrInst::StringOrInst(inst::Abstract *inst) : data_(inst),
type_(StringOrInstType::Inst) {}

StringOrInst::~StringOrInst()
{
	DeleteData();
}

void
StringOrInst::AppendString(const QString &s)
{
	if (!is_string())
		return;
	
	if (data_ == nullptr)
		data_ = new QString(s);
	else
		as_string()->append(s);
}

StringOrInst*
StringOrInst::Clone() const
{
	if (is_string())
		return new StringOrInst(*as_string());
	
	return new StringOrInst(as_inst()->Clone());
}

void
StringOrInst::DeleteData()
{
	if (is_string())
		delete as_string();
	else
		delete as_inst();
	
	type_ = StringOrInstType::None;
	data_ = nullptr;
}

bool
StringOrInst::Is(const Id id1, const Id id2) const
{
	if (!is_inst())
		return false;
	
	if (data_ == nullptr)
	{
		it_happened();
		return false;
	}
	
	return as_inst()->Is(id1, id2);
}

void
StringOrInst::SetString(const QString &s)
{
	if (!is_string())
		mtl_warn();
	
	DeleteData();
	type_ = StringOrInstType::String;
	data_ = new QString(s);
}

} // ods::