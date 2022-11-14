#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API TextTime : public Abstract
{
public:
	TextTime(Abstract *parent, ods::Tag *tag = 0, ndff::Container *cntr = 0);
	TextTime(const TextTime &cloner);
	virtual ~TextTime();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	void WriteData(QXmlStreamWriter &xml) override;
	void WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba) override;
private:
	
	/*
<text:time style:data-style-name="N2"
	text:time-value="18:57:07.989456592">00:00:00</text:time>
	*/
	void Init(ndff::Container *cntr);
	void Init(ods::Tag *tag);
	
	QString style_data_style_name_;
	QString text_time_value_;
};

} // ods::inst::
