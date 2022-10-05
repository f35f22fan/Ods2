#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API TextPageNumber : public Abstract
{
public:
	TextPageNumber(Abstract *parent, Tag *tag = nullptr);
	TextPageNumber(const TextPageNumber &cloner);
	virtual ~TextPageNumber();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	void WriteData(QXmlStreamWriter &xml) override;

private:
	
	void Init(ods::Tag *tag);
};

} // ods::inst::
