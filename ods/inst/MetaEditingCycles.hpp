#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API MetaEditingCycles : public Abstract
{
public:
	MetaEditingCycles(Abstract *parent, Tag *tag = nullptr);
	MetaEditingCycles(const MetaEditingCycles &cloner);
	virtual ~MetaEditingCycles();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	void WriteData(QXmlStreamWriter &xml) override;

private:
	void Init(Tag *tag);
};

} // ods::inst::
