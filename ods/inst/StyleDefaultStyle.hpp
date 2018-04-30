#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

class ODS_API StyleDefaultStyle : public Abstract
{
public:
	StyleDefaultStyle(Abstract *parent, ods::Tag *tag = nullptr);
	StyleDefaultStyle(const StyleDefaultStyle &cloner);
	virtual ~StyleDefaultStyle();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	void
	WriteData(QXmlStreamWriter &xml) override;
	
private:
	
	void Init(ods::Tag *tag);
	void Scan(ods::Tag *tag);
	
	QString style_family_;
};

} // ods::inst::
} // ods::
