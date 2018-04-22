#include "HAlign.hpp"

#include "ns.hxx"

namespace ods { // ods::

HAlign::HAlign(const halign::Value value)
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
	
	halign::Value v = ValueFromString(str);
	
	if (v == halign::Value::None)
		return nullptr;
	
	return new HAlign(v);
}

QString
HAlign::toString() const
{
	return ValueToString();
}

halign::Value
HAlign::ValueFromString(const QString &str)
{
	if (str == ods::ns::kStart)
		return halign::Value::Left;
	
	if (str == ods::ns::kCenter)
		return halign::Value::Center;
	
	if (str == ods::ns::kEnd)
		return halign::Value::Right;
	
	return halign::Value::None;
}

QString
HAlign::ValueToString() const
{
	if (value_ == halign::Value::Left)
		return ods::ns::kStart;
	
	if (value_ == halign::Value::Center)
		return ods::ns::kCenter;
	
	if (value_ == halign::Value::Right)
		return ods::ns::kEnd;
	
	it_happened();
	return QLatin1String();
}

} // ods::
