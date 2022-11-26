#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API OfficeMeta : public Abstract
{
public:
	OfficeMeta(Abstract *parent, Tag *tag = 0, ndff::Container *cntr = 0);
	OfficeMeta(const OfficeMeta &cloner);
	virtual ~OfficeMeta();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	void WriteData(QXmlStreamWriter &xml) override;
	
private:
	void Init(ndff::Container *cntr);
	void Init(ods::Tag *tag);
	void InitDefault();
	void Scan(ods::Tag *tag);
};

} // ods::inst::
