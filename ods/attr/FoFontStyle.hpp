#pragma once

#include <QFont>

#include "../err.hpp"
#include "../global.hxx"

namespace ods { // ods::
namespace attr { // ods::attr::

enum class FontStyle : uint8_t
{
	Invalid,
	Italic,
	Normal,
	Oblique
};

class ODS_API FoFontStyle
{
public:
	FoFontStyle();
	FoFontStyle(const FontStyle fs);
	FoFontStyle(const FoFontStyle &fs);
	virtual ~FoFontStyle();
	
	FoFontStyle*
	Clone() const;
	
	FontStyle
	font_style() const { return font_style_; }
	
	static FoFontStyle*
	FromString(const QString &s);
	
	bool
	is_italic() const { return font_style_ == FontStyle::Italic; }
	
	bool
	is_normal() const { return font_style_ == FontStyle::Normal; }
	
	bool
	is_oblique() const { return font_style_ == FontStyle::Italic; }
	
	bool
	is_valid() const { return font_style_ != FontStyle::Invalid; }
	
	void
	SetItalic() { font_style_ = FontStyle::Italic; }
	
	void
	SetNormal() { font_style_ = FontStyle::Normal; }
	
	void
	SetOblique() { font_style_ = FontStyle::Oblique; }
	
	void
	Set(FontStyle s) { font_style_ = s; }
	
	QString
	toString();

private:
	
	FontStyle font_style_ = FontStyle::Invalid;
};

} // ods::attr::
} // ods::
