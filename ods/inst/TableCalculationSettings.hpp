#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API TableCalculationSettings : public Abstract
{
public:
	TableCalculationSettings(Abstract *parent, Tag *tag = 0, ndff::Container *cntr = 0);
	TableCalculationSettings(const TableCalculationSettings &cloner);
	virtual ~TableCalculationSettings();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	void WriteData(QXmlStreamWriter &xml) override;
	void WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba) override;
private:
	void Init(ndff::Container *cntr);
	void Init(Tag *tag);
	void InitDefault();
	
	//<table:calculation-settings table:automatic-find-labels="false"
	//table:use-regular-expressions="false" table:use-wildcards="true"/>
	QString table_automatic_find_labels_;
	QString table_use_regular_expressions_;
	QString table_use_wildcards_;
};

} // ods::inst::
