#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API TextTitle : public Abstract
{
public:
	TextTitle(Abstract *parent, Tag *tag = nullptr);
	TextTitle(const TextTitle &cloner);
	virtual ~TextTitle();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	void WriteData(QXmlStreamWriter &xml) override;
private:
	
	void Init(Tag *tag);
	
};

} // ods::inst::
