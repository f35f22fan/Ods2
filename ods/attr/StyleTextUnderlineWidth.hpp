#pragma once

#include "../err.hpp"
#include "../global.hxx"

namespace ods { // ods::
namespace attr { // ods::attr::

enum class TextUnderlineWidth : uint8_t
{
	Invalid,
	Auto,
	Bold,
	Percent,
	PositiveInteger,
	PositiveLength
};

class ODS_API StyleTextUnderlineWidth
{
public:
	StyleTextUnderlineWidth();
	virtual ~StyleTextUnderlineWidth();
	
	StyleTextUnderlineWidth*
	Clone() const;
	
	static StyleTextUnderlineWidth*
	FromString(const QString &s);
	
	bool
	is_auto() const { return tuw_ == TextUnderlineWidth::Auto; }
	
	bool
	is_bold() const { return tuw_ == TextUnderlineWidth::Bold; }
	
	bool
	is_percent() const { return tuw_ == TextUnderlineWidth::Percent; }
	
	bool
	is_positive_integer() const { return tuw_ == TextUnderlineWidth::PositiveInteger; }
	
	bool
	is_positive_length() const { return tuw_ == TextUnderlineWidth::PositiveLength; }
	
	bool
	is_valid() const { return tuw_ != TextUnderlineWidth::Invalid; }
	
	void
	SetAuto() { tuw_ = TextUnderlineWidth::Auto; }
	
	void
	SetBold() { tuw_ = TextUnderlineWidth::Bold; }
	
	void
	SetPercent(const float n)
	{
		data_.percent = n;
		tuw_ = TextUnderlineWidth::Percent;
	}
	
	void
	SetPositiveInteger(const int n)
	{
		data_.positive_integer = n;
		tuw_ = TextUnderlineWidth::PositiveInteger;
	}
	
	void
	SetPositiveLength(const int n)
	{
		data_.positive_length = n;
		tuw_ = TextUnderlineWidth::PositiveLength;
	}
	
	QString
	toString();

private:
	NO_ASSIGN_COPY_MOVE(StyleTextUnderlineWidth);
	
	TextUnderlineWidth tuw_ = TextUnderlineWidth::Invalid;
	
	union Data {
		float percent;
		int positive_integer;
		int positive_length;
	};
	
	Data data_;
};

} // ods::attr::
} // ods::
