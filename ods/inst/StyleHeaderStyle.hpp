#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API StyleHeaderStyle : public Abstract
{
public:
	StyleHeaderStyle(Abstract *parent, ods::Tag *tag = nullptr);
	StyleHeaderStyle(const StyleHeaderStyle &cloner);
	virtual ~StyleHeaderStyle();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;

	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	void WriteData(QXmlStreamWriter &xml) override;
	void WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba) override;
	
private:
	
	void Init(ods::Tag *tag);
	void Scan(ods::Tag *tag);
	
	QString style_name_;
};

} // ods::inst::
