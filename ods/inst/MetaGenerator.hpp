#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API MetaGenerator: public Abstract
{
public:
	MetaGenerator(Abstract *parent, Tag *tag = 0, ndff::Container *cntr = 0);
	MetaGenerator(const MetaGenerator &cloner);
	virtual ~MetaGenerator();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	void WriteData(QXmlStreamWriter &xml) override;

private:
};

} // ods::inst::
