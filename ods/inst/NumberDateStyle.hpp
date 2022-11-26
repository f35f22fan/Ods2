#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API NumberDateStyle : public Abstract
{
public:
	NumberDateStyle(Abstract *parent, ods::Tag *tag = 0, ndff::Container *cntr = 0);
	NumberDateStyle(const NumberDateStyle &cloner);
	virtual ~NumberDateStyle();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	
	NumberDay* NewDay();
	NumberHours* NewHours();
	NumberMinutes* NewMinutes();
	NumberMonth* NewMonth();
	NumberSeconds* NewSeconds();
	NumberText* NewText(const QString &s);
	NumberYear* NewYear();
	
	virtual QString* style_name() override { return &style_name_; }
	void style_name(const QString &s) { style_name_ = s; }
	
	void WriteData(QXmlStreamWriter &xml) override;
	void WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba) override;
private:
	
	void Init(ndff::Container *cntr);
	void Init(Tag *tag);
	void Scan(Tag *tag);
	
	// <number:date-style style:name="N10036" number:language="ru"
	// number:country="RU" number:automatic-order="true">
	
	QString number_automatic_order_;
	QString number_country_;
	QString number_language_;
	QString style_name_;
};

} // ods::inst::
