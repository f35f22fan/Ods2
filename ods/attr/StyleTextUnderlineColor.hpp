#pragma once

#include "../err.hpp"
#include "../global.hxx"
#include "../Color.hpp"

namespace ods::attr {

enum class TextUnderlineColor : uint8_t
{
	Invalid,
	Color,
	FontColor
};

class ODS_API StyleTextUnderlineColor
{
public:
	StyleTextUnderlineColor();
	virtual ~StyleTextUnderlineColor();
	StyleTextUnderlineColor* Clone() const;
	
	static StyleTextUnderlineColor* FromString(const QString &s);
	
	const ods::Color& color() const { return color_; }
	bool is_color() const { return tuc_ == TextUnderlineColor::Color; }
	bool is_font_color() const { return tuc_ == TextUnderlineColor::FontColor; }
	bool is_valid() const { return tuc_ != TextUnderlineColor::Invalid; }
	void SetColor(const ods::Color &c) { tuc_ = TextUnderlineColor::Color; color_ = c; }
	void SetFontColor() { tuc_ = TextUnderlineColor::FontColor; }
	
	QString toString();

private:
	NO_ASSIGN_COPY_MOVE(StyleTextUnderlineColor);
	
	TextUnderlineColor tuc_ = TextUnderlineColor::Invalid;
	ods::Color color_ = {};
};

} // ods::attr::
