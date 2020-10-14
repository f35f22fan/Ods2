#include "HAlign.hpp"

#include "ns.hxx"

namespace ods { // ods::

HAlign::HAlign(const HAlignSide value)
{
	value_ = value;
}

HAlign::~HAlign() {}

HAlign*
HAlign::Clone() const
{
	return new HAlign(value_);
}

HAlign*
HAlign::FromString(const QString &str)
{
	if (str.isEmpty())
		return nullptr;
	
	HAlignSide v = ValueFromString(str);
	
	if (v == HAlignSide::None)
		return nullptr;
	
	return new HAlign(v);
}

QString
HAlign::toString() const
{
	return ValueToString();
}

HAlignSide
HAlign::ValueFromString(const QString &str)
{
	if (str == ods::ns::kStart)
		return HAlignSide::Left;
	
	if (str == ods::ns::kCenter)
		return HAlignSide::Center;
	
	if (str == ods::ns::kEnd)
		return HAlignSide::Right;
	
	return HAlignSide::None;
}

QString
HAlign::ValueToString() const
{
	if (value_ == HAlignSide::Left)
		return ods::ns::kStart;
	
	if (value_ == HAlignSide::Center)
		return ods::ns::kCenter;
	
	if (value_ == HAlignSide::Right)
		return ods::ns::kEnd;
	
	it_happened();
	return QLatin1String();
}

} // ods::
