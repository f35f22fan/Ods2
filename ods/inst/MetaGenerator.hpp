#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

class ODS_API MetaGenerator : public Abstract
{
public:
	MetaGenerator(Abstract *parent, Tag *tag = nullptr);
	MetaGenerator(const MetaGenerator &cloner);
	virtual ~MetaGenerator();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	void
	WriteData(QXmlStreamWriter &xml) override;

private:
	
	void Init(Tag *tag);
};

} // ods::inst::
} // ods::
