#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API DcTitle : public Abstract
{
public:
	DcTitle(Abstract *parent, Tag *tag, ndff::Container *cntr = 0);
	DcTitle(const DcTitle &cloner);
	virtual ~DcTitle();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	void WriteData(QXmlStreamWriter &xml) override;

private:
};

} // ods::inst::
