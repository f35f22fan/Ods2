#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

class ODS_API TextPageCount : public Abstract
{
public:
	TextPageCount(Abstract *parent, ods::Tag *tag = nullptr);
	TextPageCount(const TextPageCount &cloner);
	virtual ~TextPageCount();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;

	void
	WriteData(QXmlStreamWriter &xml) override;
	
private:
	
	void Init(ods::Tag *tag);
};

} // ods::inst::
} // ods::
