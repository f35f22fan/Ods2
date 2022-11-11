#pragma once

#include "Abstract.hpp"
#include "../currency.hxx"
#include "decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API NumberCurrencySymbol : public Abstract
{
public:
	NumberCurrencySymbol(Abstract *parent, ods::Tag *tag = 0, ndff::Container *cntr = 0);
	NumberCurrencySymbol(const NumberCurrencySymbol &cloner);
	virtual ~NumberCurrencySymbol();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	const QString&
	country() const { return number_country_; }
	
	void country(const QString &s) { number_country_ = s; }
	
	const QString&
	language() const { return number_language_; }
	
	void language(const QString &s) { number_language_ = s; }
	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	void SetSymbol(const ods::Currency &c);
	
	QString
	GetSymbol();
	
	void WriteData(QXmlStreamWriter &xml) override;
	void WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba) override;
private:
	
	void Init(ndff::Container *cntr);
	void Init(Tag *tag);
	
	// <number:currency-symbol number:language="en"
	// number:country="US">$</number:currency-symbol>
	
	QString number_language_;
	QString number_rfc_language_tag_;
	QString number_country_;
	QString number_script_;
};

} // ods::inst::
