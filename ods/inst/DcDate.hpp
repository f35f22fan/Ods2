#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

class ODS_API DcDate : public Abstract
{
public:
	DcDate(Abstract *parent, Tag *tag = nullptr);
	DcDate(const DcDate &cloner);
	virtual ~DcDate();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	void
	WriteData(QXmlStreamWriter &xml) override;

private:
	
	void Init(Tag *tag);
};

} // ods::inst::
} // ods::
