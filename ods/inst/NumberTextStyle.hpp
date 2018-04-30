#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

class ODS_API NumberTextStyle : public Abstract
{
public:
	NumberTextStyle(Abstract *parent, ods::Tag *tag = nullptr);
	NumberTextStyle(const NumberTextStyle &cloner);
	virtual ~NumberTextStyle();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	void
	WriteData(QXmlStreamWriter &xml) override;
	
private:
	
	void Init(ods::Tag *tag);
	void Scan(ods::Tag *tag);
	
	QString style_name_;
};

} // ods::inst::
} // ods::
