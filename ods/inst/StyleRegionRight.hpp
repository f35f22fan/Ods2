#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

class ODS_API StyleRegionRight : public Abstract
{
public:
	StyleRegionRight(Abstract *parent, Tag *tag = nullptr);
	StyleRegionRight(const StyleRegionRight &cloner);
	virtual ~StyleRegionRight();
	
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
