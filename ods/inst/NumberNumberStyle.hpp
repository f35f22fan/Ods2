#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

class ODS_API NumberNumberStyle : public Abstract
{
public:
	NumberNumberStyle(Abstract *parent, ods::Tag *tag = nullptr);
	NumberNumberStyle(const NumberNumberStyle &cloner);
	virtual ~NumberNumberStyle();
	
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
