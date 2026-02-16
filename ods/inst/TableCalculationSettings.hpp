#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API TableCalculationSettings : public Abstract
{
public:
	TableCalculationSettings(Abstract *parent, Tag *tag = 0);
	TableCalculationSettings(const TableCalculationSettings &cloner);
	virtual ~TableCalculationSettings();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	void WriteData(QXmlStreamWriter &xml) override;
private:
	void Init(Tag *tag);
	void InitDefault();
	
	//<table:calculation-settings table:automatic-find-labels="false"
	//table:use-regular-expressions="false" table:use-wildcards="true"/>
	QString table_automatic_find_labels_;
	QString table_use_regular_expressions_;
	QString table_use_wildcards_;
};

} // ods::inst::
