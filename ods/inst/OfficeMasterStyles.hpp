#pragma once

#include "Abstract.hpp"
#include "../decl.hxx"
#include "../err.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

class ODS_API OfficeMasterStyles : public Abstract
{
public:
	OfficeMasterStyles(Abstract *parent, Tag *tag = nullptr);
	OfficeMasterStyles(const OfficeMasterStyles &cloner);
	virtual ~OfficeMasterStyles();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	void
	WriteData(QXmlStreamWriter &xml) override;

private:
	
	void Init(Tag *tag);
	void Scan(Tag *tag);
};

} // ods::inst::
} // ods::
