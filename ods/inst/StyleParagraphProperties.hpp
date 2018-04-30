#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

class ODS_API StyleParagraphProperties : public ods::inst::Abstract
{
public:
	StyleParagraphProperties(Abstract *parent, ods::Tag *tag = nullptr);
	StyleParagraphProperties(const StyleParagraphProperties &cloner);
	
	virtual ~StyleParagraphProperties();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	ods::HAlign*
	text_align() const { return fo_text_align_; }
	
	void
	text_align(ods::HAlign *a);
	
	void
	WriteData(QXmlStreamWriter &xml) override;
	
private:
	
	void Init(ods::Tag *tag);
	
	ods::HAlign *fo_text_align_ = nullptr;
	Length *fo_margin_left_ = nullptr;
	QString style_tab_stop_distance_;
};

} // ods::inst::
} // ods::
