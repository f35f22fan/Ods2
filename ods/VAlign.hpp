#pragma once

#include "err.hpp"
#include "global.hxx"
#include "types.hxx"

namespace ods { // ods::

enum class VAlignSide : u8
{
	None,
	Top,
	Middle,
	Bottom
};

namespace valign { // ods::valign::

} // ods::valign::

class ODS_API VAlign
{
public:
	VAlign(const VAlignSide value);
	virtual ~VAlign();
	
	VAlign*
	Clone() const;
	
	static VAlign*
	FromString(const QString &str);
	
	bool
	is_bottom() const { return value_ == VAlignSide::Bottom; }
	
	bool
	is_middle() const { return value_ == VAlignSide::Middle; }
	
	bool
	is_top() const { return value_ == VAlignSide::Top; }
	
	bool
	is_valid() const { return value_ != VAlignSide::None; }
	
	QString
	toString() const;
	
	VAlignSide
	value() const { return value_; }
	
	void
	value(const VAlignSide v) { value_ = v; }
	
private:
	NO_ASSIGN_COPY_MOVE(VAlign);
	
	static VAlignSide
	ValueFromString(const QString &str);
	
	QString
	ValueToString() const;
	
	VAlignSide value_ = VAlignSide::None;
};

} // ods::
