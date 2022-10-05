#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API OfficeFontFaceDecls : public Abstract
{
public:
	OfficeFontFaceDecls(Abstract *parent, ods::Tag *tag = nullptr);
	OfficeFontFaceDecls(const OfficeFontFaceDecls &cloner);
	virtual ~OfficeFontFaceDecls();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	StyleFontFace*
	GetFontFace(const QString &font_name, const AddIfNeeded ain);
	
	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	
	StyleFontFace*
	Register(const QString &font_name);
	
	void WriteData(QXmlStreamWriter &xml) override;
	
private:
	
	void Init(ods::Tag *tag);
	void InitDefault();
	void Scan(ods::Tag *parent);
	
};

} // ods::inst::
