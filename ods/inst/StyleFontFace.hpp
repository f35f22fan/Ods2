#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods::inst {

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
	
	bool IsFont(const QString &font_name) const;
	
	const QString&
	font_family() const { return svg_font_family_; }
	
	void font_family_generic(const QString &s);
	void font_pitch(const QString &s);
	
	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	
	void Set(const QString &s);

	QString* style_name() override { return &style_name_; }
	
	void WriteData(QXmlStreamWriter &xml) override;
	void WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba) override;
private:
	
	void Init(ods::Tag *tag);
	
	QString svg_font_family_;
	QString style_name_;
	QString style_font_family_generic_;
	QString style_font_pitch_;
};

} // ods::inst::
