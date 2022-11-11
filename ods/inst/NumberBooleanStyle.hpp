#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API NumberBooleanStyle : public Abstract
{
public:
	NumberBooleanStyle(Abstract *parent, Tag *tag = 0, ndff::Container *cntr = 0);
	NumberBooleanStyle(const NumberBooleanStyle &cloner);
	virtual ~NumberBooleanStyle();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	
	NumberBoolean*
	NewBoolean();
	
	NumberText*
	NewText(const QString &s);
	
	virtual QString*
	style_name() override { return &style_name_; }
	
	void style_name(const QString &s) { style_name_ = s; }
	void WriteData(QXmlStreamWriter &xml) override;
	void WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba) override;
private:
	
	void Init(ndff::Container *cntr);
	void Init(Tag *tag);
	void Scan(Tag *tag);
	
	QString number_country_;
	QString number_language_;
	QString number_rfc_language_tag_;
	QString number_script_;
	QString number_title_;
	QString number_transliteration_country_;
	QString number_transliteration_format_;
	QString number_translitaration_language_;
	QString number_transliteration_style_;
	QString style_display_name_;
	QString style_name_;
	QString style_volatile_;
};

} // ods::inst::
