#pragma once

#include "Abstract.hpp"
#include "decl.hxx"

#include "../err.hpp"

namespace ods::inst {

class ODS_API SvgDesc : public Abstract
{
public:
	SvgDesc(Abstract *parent, ods::Tag *tag = nullptr);
	SvgDesc(const SvgDesc &cloner);
	virtual ~SvgDesc();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	const QString *GetFirstString() const;
	const QString* GetString() const { return GetFirstString(); }
	
	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	
	void SetFirstString(const QString &s);
	void SetString(const QString &s) { SetFirstString(s); }
	
	void WriteData(QXmlStreamWriter &xml) override;

private:
	void Init(ods::Tag*);
};

} // ods::inst::
