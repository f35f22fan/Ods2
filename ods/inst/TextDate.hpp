#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

class ODS_API TextDate : public Abstract
{
public:
	TextDate(Abstract *parent, Tag *tag = nullptr);
	TextDate(const TextDate &cloner);
	virtual ~TextDate();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;

	void
	WriteData(QXmlStreamWriter &xml) override;
	
private:
	
	void Init(Tag *tag);
	
	QString style_data_style_name_;
	QString text_date_value_;
	
//<text:date style:data-style-name="N2"
// text:date-value="2018-01-12">00/00/0000</text:date>
};

} // ods::inst::
} // ods::
