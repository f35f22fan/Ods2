#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"
#include "../ndff/ndff.hh"

namespace ods::inst {

class ODS_API OfficeFontFaceDecls : public Abstract
{
public:
	OfficeFontFaceDecls(Abstract *parent, ods::Tag *tag = 0, ndff::Container *cntr = 0);
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
	
	void Init(ndff::Container *cntr);
	void Init(ods::Tag *tag);
	void InitDefault();
	void Scan(ods::Tag *parent);
	
};

} // ods::inst::
