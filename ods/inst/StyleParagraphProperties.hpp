#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API StyleParagraphProperties : public ods::inst::Abstract
{
public:
	StyleParagraphProperties(Abstract *parent, ods::Tag *tag = 0, ndff::Container *cntr = 0);
	StyleParagraphProperties(const StyleParagraphProperties &cloner);
	
	virtual ~StyleParagraphProperties();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	
	ods::HAlign*
	text_align() const { return fo_text_align_; }
	
	void text_align(ods::HAlign *a);
	void WriteData(QXmlStreamWriter &xml) override;
	void WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba) override;
private:
	void Init(ndff::Container *cntr);
	void Init(ods::Tag *tag);
	
	ods::HAlign *fo_text_align_ = nullptr;
	Length *fo_margin_left_ = nullptr;
	QString style_tab_stop_distance_;
};

} // ods::inst::
