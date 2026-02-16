#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API OfficeScripts : public Abstract
{
public:
	OfficeScripts(Abstract *parent, Tag *tag = 0);
	OfficeScripts(const OfficeScripts &cloner);
	virtual ~OfficeScripts();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;

	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	void WriteData(QXmlStreamWriter &xml) override;

private:
};

} // ods::inst::
