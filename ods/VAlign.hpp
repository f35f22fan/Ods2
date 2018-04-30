#pragma once

#include "err.hpp"
#include "global.hxx"

namespace ods { // ods::

namespace valign { // ods::valign::

enum class Value : uint8_t
{
	None,
	Top,
	Middle,
	Bottom
};

} // ods::valign::

class ODS_API VAlign
{
public:
	VAlign(const valign::Value value);
	virtual ~VAlign();
	
	VAlign*
	Clone() const;
	
	static VAlign*
	FromString(const QString &str);
	
	bool
	is_bottom() const { return value_ == valign::Value::Bottom; }
	
	bool
	is_middle() const { return value_ == valign::Value::Middle; }
	
	bool
	is_top() const { return value_ == valign::Value::Top; }
	
	bool
	is_valid() const { return value_ != valign::Value::None; }
	
	QString
	toString() const;
	
	valign::Value
	value() const { return value_; }
	
	void
	value(const valign::Value v) { value_ = v; }
	
private:
	NO_ASSIGN_COPY_MOVE(VAlign);
	
	static valign::Value
	ValueFromString(const QString &str);
	
	QString
	ValueToString() const;
	
	valign::Value value_ = valign::Value::None;
};

} // ods::
