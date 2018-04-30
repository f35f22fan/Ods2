#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

class ODS_API TextTime : public Abstract
{
public:
	TextTime(Abstract *parent, ods::Tag *tag = nullptr);
	TextTime(const TextTime &cloner);
	virtual ~TextTime();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	void
	WriteData(QXmlStreamWriter &xml) override;

private:
	
	/*
<text:time style:data-style-name="N2"
	text:time-value="18:57:07.989456592">00:00:00</text:time>
	*/
	void Init(ods::Tag *tag);
	
	QString style_data_style_name_;
	QString text_time_value_;
};

} // ods::inst::
} // ods::
