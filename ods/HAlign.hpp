#pragma once

#include "err.hpp"
#include "global.hxx"

namespace ods { // ods::

namespace halign { // ods::halign::

enum class Value : uint8_t
{
	None,
	Left,
	Center,
	Right
};

} // ods::halign::

class ODS_API HAlign
{
public:
	HAlign(const halign::Value value);
	virtual ~HAlign();
	
	HAlign*
	Clone() const;
	
	static HAlign*
	FromString(const QString &str);
	
	bool
	is_center() const { return value_ == halign::Value::Center; }
	
	bool
	is_left() const { return value_ == halign::Value::Left; }
	
	bool
	is_right() const { return value_ == halign::Value::Right; }
	
	bool
	is_valid() const { return value_ != halign::Value::None; }
	
	QString
	toString() const;
	
	halign::Value
	value() const { return value_; }
	
	void
	value(const halign::Value v) { value_ = v; }
	
private:
	NO_ASSIGN_COPY_MOVE(HAlign);
	
	static halign::Value
	ValueFromString(const QString &str);
	
	QString
	ValueToString() const;
	
	halign::Value value_ = halign::Value::None;
};

} // ods::
