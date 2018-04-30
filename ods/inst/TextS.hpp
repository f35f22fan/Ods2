#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

class ODS_API TextS : public Abstract
{
public:
	TextS(Abstract *parent, Tag *tag = nullptr);
	TextS(const TextS &cloner);
	virtual ~TextS();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;

	void
	WriteData(QXmlStreamWriter &xml) override;
	
private:
	
	void Init(Tag *tag);
};

} // ods::inst::
} // ods::
