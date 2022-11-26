#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API MetaTemplate : public Abstract
{
public:
	MetaTemplate(Abstract *parent, Tag  *Tag = 0, ndff::Container *cntr = 0);
	MetaTemplate(const MetaTemplate &cloner);
	virtual ~MetaTemplate();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;

	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	void WriteData(QXmlStreamWriter &xml) override;
	void WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba) override;
private:
	void Init(ndff::Container *cntr);
	void Init(Tag *tag);
	
//<meta:template xlink:type="simple"
//	xlink:actuate="onRequest" xlink:title="CustomShit"
//	xlink:href="../../.config/libreoffice/4/user/template/CustomShit.ots"
//	meta:date="2018-01-10T17:12:48.543249213"/>
	
	QString xlink_type_;
	QString xlink_actuate_;
	QString xlink_title_;
	QString xlink_href_;
	QString meta_date_;
};

} // ods::inst::
