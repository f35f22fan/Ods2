#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API NumberText : public Abstract
{
public:
	NumberText(Abstract *parent, ods::Tag *tag = 0, ndff::Container *cntr = 0);
	NumberText(const NumberText &cloner);
	virtual ~NumberText();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	const QString *GetFirstString() const;
	
	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	void SetFirstString(const QString &s);
	void WriteData(QXmlStreamWriter &xml) override;
	
private:
};

} // ods::inst::
