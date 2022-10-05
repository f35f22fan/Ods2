#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API OfficeAutomaticStyles : public Abstract
{
public:
	OfficeAutomaticStyles(Abstract *parent, ods::Tag *tag = nullptr);
	OfficeAutomaticStyles(const OfficeAutomaticStyles &cloner);
	virtual ~OfficeAutomaticStyles();
	
	Abstract*
	ByStyleName(const QString &name) const;
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	
	NumberBooleanStyle*
	NewNumberBooleanStyle();
	
	NumberCurrencyStyle*
	NewNumberCurrencyStyle();
	
	NumberDateStyle*
	NewNumberDateStyle();
	
	NumberPercentageStyle*
	NewNumberPercentageStyle();
	
	NumberTimeStyle*
	NewNumberTimeStyle();
	
	StyleStyle*
	NewStyleStyle(const style::Family f);
	
	void WriteData(QXmlStreamWriter &xml) override;
	
private:
	
	void Init(ods::Tag *tag);
	void Scan(ods::Tag *tag);
};

} // ods::inst::
