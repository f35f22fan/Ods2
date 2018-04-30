#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

class ODS_API StyleFontFace : public Abstract
{
public:
	StyleFontFace(ods::inst::Abstract *parent, ods::Tag *tag = nullptr);
	StyleFontFace(const StyleFontFace &cloner);
	virtual ~StyleFontFace();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	static StyleFontFace*
	FromTag(ods::inst::Abstract*, ods::Tag*);
	
	bool
	IsFont(const QString &font_name) const;
	
	const QString&
	style_name() const { return style_name_; }
	
	const QString&
	font_family() const { return svg_font_family_; }
	
	void
	font_family_generic(const QString &s);
	
	void
	font_pitch(const QString &s);
	
	void
	Set(const QString &s);

	void
	WriteData(QXmlStreamWriter &xml) override;
	
private:
	
	void Init(ods::Tag *tag);
	
	QString svg_font_family_;
	QString style_name_;
	QString style_font_family_generic_;
	QString style_font_pitch_;
};

} // ods::inst::
} // ods::
