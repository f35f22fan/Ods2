#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API MetaCreationDate : public Abstract
{
public:
	MetaCreationDate(Abstract *parent, Tag *tag = nullptr);
	MetaCreationDate(const MetaCreationDate &cloner);
	virtual ~MetaCreationDate();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	void WriteData(QXmlStreamWriter &xml) override;
private:
	
	void Init(Tag *tag);
};

} // ods::inst::
