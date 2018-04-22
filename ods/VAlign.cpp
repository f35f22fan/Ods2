#include "VAlign.hpp"

#include "ns.hxx"

namespace ods { // ods::

VAlign::VAlign(const valign::Value value)
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
	
	valign::Value v = ValueFromString(str);
	
	if (v == valign::Value::None)
		return nullptr;
	
	return new VAlign(v);
}

QString
VAlign::toString() const
{
	return ValueToString();
}

valign::Value
VAlign::ValueFromString(const QString &str)
{
	if (str == ods::ns::kBottom)
		return valign::Value::Bottom;
	
	if (str == ods::ns::kMiddle)
		return valign::Value::Middle;
	
	if (str == ods::ns::kTop)
		return valign::Value::Top;
	
	return valign::Value::None;
}

QString
VAlign::ValueToString() const
{
	if (value_ == valign::Value::Bottom)
		return ods::ns::kBottom;
	
	if (value_ == valign::Value::Middle)
		return ods::ns::kMiddle;
	
	if (value_ == valign::Value::Top)
		return ods::ns::kTop;
	
	it_happened();
	return QLatin1String();
}

} // ods::
