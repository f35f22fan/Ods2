#pragma once

#include "Abstract.hpp"
#include "../decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API StyleFooterLeft : public Abstract
{
public:
	StyleFooterLeft(Abstract *parent, Tag *tag = 0);
	StyleFooterLeft(const StyleFooterLeft &cloner);
	virtual ~StyleFooterLeft();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	void WriteData(QXmlStreamWriter &xml) override;
	
private:
	void Init(Tag *tag);
	
	QString style_display_;
};

} // ods::inst::
