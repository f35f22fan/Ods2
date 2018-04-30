#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

class ODS_API DcTitle : public Abstract
{
public:
	DcTitle(Abstract *parent, Tag *tag = nullptr);
	DcTitle(const DcTitle &cloner);
	virtual ~DcTitle();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	void
	WriteData(QXmlStreamWriter &xml) override;

private:
	
	void Init(Tag *tag);
};

} // ods::inst::
} // ods::
