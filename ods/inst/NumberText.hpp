#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API NumberText : public Abstract
{
public:
	NumberText(Abstract *parent, ods::Tag *tag = nullptr);
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
	
	void Init(ods::Tag *tag);
};

} // ods::inst::
