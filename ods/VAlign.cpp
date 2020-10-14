#include "VAlign.hpp"

#include "ns.hxx"

namespace ods { // ods::

VAlign::VAlign(const VAlignSide value)
{
	value_ = value;
}

VAlign::~VAlign() {}

VAlign*
VAlign::Clone() const
{
	return new VAlign(value_);
}

VAlign*
VAlign::FromString(const QString &str)
{
	if (str.isEmpty())
		return nullptr;
	
	VAlignSide v = ValueFromString(str);
	
	if (v == VAlignSide::None)
		return nullptr;
	
	return new VAlign(v);
}

QString
VAlign::toString() const
{
	return ValueToString();
}

VAlignSide
VAlign::ValueFromString(const QString &str)
{
	if (str == ods::ns::kBottom)
		return VAlignSide::Bottom;
	
	if (str == ods::ns::kMiddle)
		return VAlignSide::Middle;
	
	if (str == ods::ns::kTop)
		return VAlignSide::Top;
	
	return VAlignSide::None;
}

QString
VAlign::ValueToString() const
{
	if (value_ == VAlignSide::Bottom)
		return ods::ns::kBottom;
	
	if (value_ == VAlignSide::Middle)
		return ods::ns::kMiddle;
	
	if (value_ == VAlignSide::Top)
		return ods::ns::kTop;
	
	it_happened();
	return QLatin1String();
}

} // ods::
