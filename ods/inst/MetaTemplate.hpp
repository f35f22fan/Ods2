#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

class ODS_API MetaTemplate : public Abstract
{
public:
	MetaTemplate(Abstract *parent, Tag  *Tag = nullptr);
	MetaTemplate(const MetaTemplate &cloner);
	virtual ~MetaTemplate();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;

	void
	WriteData(QXmlStreamWriter &xml) override;
	
private:

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
} // ods::
