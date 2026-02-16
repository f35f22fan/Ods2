#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API StylePageLayoutProperties : public Abstract
{
public:
	StylePageLayoutProperties(Abstract *parent, ods::Tag *tag = 0);
	StylePageLayoutProperties(const StylePageLayoutProperties &cloner);
	virtual ~StylePageLayoutProperties();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	void WriteData(QXmlStreamWriter &xml) override;
private:
	void Init(ods::Tag *tag);
	
	// style:writing-mode="lr-tb"
	QString style_writing_mode_;
};

} // ods::inst::
