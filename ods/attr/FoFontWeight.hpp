#pragma once

#include "../err.hpp"
#include "../global.hxx"

namespace ods { // ods::
namespace attr { // ods::attr::

enum class FontWeight : uint8_t
{
	Invalid,
	Normal,
	Bold,
	N100,
	N200,
	N300,
	N400,
	N500,
	N600,
	N700,
	N800,
	N900
};

class ODS_API FoFontWeight
{
public:
	FoFontWeight();
	FoFontWeight(const FontWeight fw);
	virtual ~FoFontWeight();
	
	FoFontWeight*
	Clone() const;
	
	static FoFontWeight*
	FromString(const QString &s);
	
	FontWeight
	font_weight() const { return font_weight_; }
	
	bool
	is_bold() const { return font_weight_ == FontWeight::Bold; }
	
	bool
	is_normal() const { return font_weight_ == FontWeight::Normal; }
	
	bool
	is_valid() const { return font_weight_ != FontWeight::Invalid; }
	
	void
	Set(const FontWeight fw) { font_weight_ = fw; }
	
	void
	SetBold() { font_weight_ = FontWeight::Bold; }
	
	void
	SetNormal() { font_weight_ = FontWeight::Normal; }
	
	QString
	toString();

private:
	
	FontWeight font_weight_ = FontWeight::Invalid;
};

} // ods::attr::
} // ods::
