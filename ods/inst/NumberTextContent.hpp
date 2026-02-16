#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API NumberTextContent : public Abstract
{
public:
	NumberTextContent(Abstract *parent, ods::Tag *tag = 0);
	NumberTextContent(const NumberTextContent &cloner);
	virtual ~NumberTextContent();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;

	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	void WriteData(QXmlStreamWriter &xml) override;
	
private:
};

} // ods::inst::
