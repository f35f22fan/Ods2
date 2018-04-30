#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

class ODS_API StyleBackgroundImage : public Abstract
{
public:
	StyleBackgroundImage(Abstract *parent, Tag *tag = nullptr);
	StyleBackgroundImage(const StyleBackgroundImage &cloner);
	virtual ~StyleBackgroundImage();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;

	void
	WriteData(QXmlStreamWriter &xml) override;
	
private:
	
	void Init(Tag *tag);
};

} // ods::inst::
} // ods::
