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
	
	virtual QString*
	data_style_name() { return nullptr; }
	
	inst::NumberCurrencySymbol*
	NewCurrencySymbol();
	
	inst::NumberNumber*
	NewNumber();
	
	virtual QString*
	style_name() override { return &style_name_; }
	
	void
	style_name(const QString &s) { style_name_ = s; }
	
	virtual QString*
	parent_style_name() { return nullptr; }
	
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
