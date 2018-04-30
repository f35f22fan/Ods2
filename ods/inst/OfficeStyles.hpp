#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

class ODS_API OfficeStyles : public Abstract
{
public:
	OfficeStyles(ods::inst::Abstract *parent, ods::Tag *tag = nullptr);
	OfficeStyles(const OfficeStyles &cloner);
	virtual ~OfficeStyles();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	void
	WriteData(QXmlStreamWriter &xml) override;
	
private:
	
	void Init(ods::Tag *tag);
	void Scan(ods::Tag *tag);
};

} // ods::inst::
} // ods::
