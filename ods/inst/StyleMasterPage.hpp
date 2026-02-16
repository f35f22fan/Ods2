#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API StyleMasterPage : public Abstract
{
public:
	StyleMasterPage(Abstract *parent, Tag *tag = 0);
	StyleMasterPage(const StyleMasterPage &cloner);
	virtual ~StyleMasterPage();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;

	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	void WriteData(QXmlStreamWriter &xml) override;
private:
	void Init(Tag *tag);
	void Scan(Tag *tag);
	
//<style:master-page style:name="Report"
//	style:page-layout-name="Mpm2">
	
	QString style_name_;
	QString style_page_layout_name_;
};

} // ods::inst::
