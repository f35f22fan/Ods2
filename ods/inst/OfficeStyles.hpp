#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API OfficeStyles : public Abstract
{
public:
	OfficeStyles(ods::inst::Abstract *parent, ods::Tag *tag = 0, ndff::Container *cntr = 0);
	OfficeStyles(const OfficeStyles &cloner);
	virtual ~OfficeStyles();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	
	NumberCurrencyStyle*
	NewNumberCurrencyStyle();
	
	StyleStyle*
	NewStyleStyle(const style::Family f);
	
	void WriteData(QXmlStreamWriter &xml) override;
	
private:
	void Init(ndff::Container *cntr);
	void Init(ods::Tag *tag);
	void Scan(ods::Tag *tag);
};

} // ods::inst::
