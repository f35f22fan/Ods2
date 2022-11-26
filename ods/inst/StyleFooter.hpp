#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API StyleFooter : public Abstract
{
public:
	StyleFooter(Abstract *parent, Tag *tag = 0, ndff::Container *cntr = 0);
	StyleFooter(const StyleFooter &cloner);
	virtual ~StyleFooter();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;

	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	void WriteData(QXmlStreamWriter &xml) override;
	
private:
	void Init(ndff::Container *cntr);
	void Init(ods::Tag *tag);
	void Scan(ods::Tag *tag);
};

} // ods::inst::
