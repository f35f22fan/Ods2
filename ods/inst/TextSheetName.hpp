#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API TextSheetName : public Abstract
{
public:
	TextSheetName(Abstract *parent, Tag *tag = nullptr);
	TextSheetName(const TextSheetName &cloner);
	virtual ~TextSheetName();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;

	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	void WriteData(QXmlStreamWriter &xml) override;
	
private:
	
	void Init(Tag *tag);
};

} // ods::inst::
