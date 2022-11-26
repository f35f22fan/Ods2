#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API NumberBoolean : public Abstract
{
public:
	NumberBoolean(Abstract *parent, Tag *tag = 0, ndff::Container *cntr = 0);
	NumberBoolean(const NumberBoolean &cloner);
	virtual ~NumberBoolean();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	void WriteData(QXmlStreamWriter &xml) override;
	
private:
};

} // ods::inst::
