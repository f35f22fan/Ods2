#pragma once

#include "err.hpp"
#include "global.hxx"

namespace ods { // ods::

enum class HAlignSide : uint8_t
{
	None,
	Left,
	Center,
	Right
};

class ODS_API HAlign
{
public:
	HAlign(const HAlignSide value);
	virtual ~HAlign();
	
	HAlign*
	Clone() const;
	
	static HAlign*
	FromString(const QString &str);
	
	bool
	is_center() const { return value_ == HAlignSide::Center; }
	
	bool
	is_left() const { return value_ == HAlignSide::Left; }
	
	bool
	is_right() const { return value_ == HAlignSide::Right; }
	
	bool
	is_valid() const { return value_ != HAlignSide::None; }
	
	QString
	toString() const;
	
	HAlignSide
	value() const { return value_; }
	
	void
	value(const HAlignSide v) { value_ = v; }
	
private:
	NO_ASSIGN_COPY_MOVE(HAlign);
	
	static HAlignSide
	ValueFromString(const QString &str);
	
	QString
	ValueToString() const;
	
	HAlignSide value_ = HAlignSide::None;
};

} // ods::
