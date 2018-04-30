#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

class ODS_API NumberTextContent : public Abstract
{
public:
	NumberTextContent(Abstract *parent, ods::Tag *tag = nullptr);
	NumberTextContent(const NumberTextContent &cloner);
	virtual ~NumberTextContent();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	void
	WriteData(QXmlStreamWriter &xml) override;
	
private:
	
	void Init(ods::Tag *tag);
};

} // ods::inst::
} // ods::
