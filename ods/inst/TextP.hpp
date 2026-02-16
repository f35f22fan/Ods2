#pragma once

#include "Abstract.hpp"
#include "../decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API TextP : public Abstract
{
public:
	TextP(Abstract *parent, ods::Tag *tag = 0);
	TextP(const TextP &cloner);
	virtual ~TextP();
	
	void AppendString(const QString &s);
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	const QString *GetFirstString() const;
	
	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	
	void WriteData(QXmlStreamWriter &xml) override;

private:
	void Init(ods::Tag *tag);
	void Scan(ods::Tag *tag);
};

} // ods::inst::
