#pragma once

#include "Abstract.hpp"
#include "decl.hxx"

#include "../err.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

class ODS_API NumberCurrencyStyle : public Abstract
{
public:
	NumberCurrencyStyle(Abstract *parent, Tag *tag = nullptr);
	NumberCurrencyStyle(const NumberCurrencyStyle &cloner);
	virtual ~NumberCurrencyStyle();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	QString*
	data_style_name() override { return nullptr; }
	
	NumberCurrencySymbol*
	FetchCurrencySymbol();
	
	NumberNumber*
	FetchNumber();
	
	inst::NumberCurrencySymbol*
	NewCurrencySymbol();
	
	inst::NumberNumber*
	NewNumber();

	inst::NumberText*
	NewText();
	
	virtual QString*
	style_name() override { return &style_name_; }
	
	void
	style_name(const QString &s) { style_name_ = s; }
	
	QString*
	parent_style_name() override { return nullptr; }
	
	void
	WriteData(QXmlStreamWriter &xml) override;
	
private:
	
	void Init(ods::Tag *tag);
	void Scan(ods::Tag *tag);
	
/*
<number:currency-symbol number:language="en" number:country="US">$</number:currency-symbol>

<number:number number:decimal-places="2" loext:min-decimal-places="2"
	number:min-integer-digits="1" number:grouping="true"/>
*/
	
	QString style_name_;
	QString style_volatile_;
};

} // ods::inst::
} // ods::
