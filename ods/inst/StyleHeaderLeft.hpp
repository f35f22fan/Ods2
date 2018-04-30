#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

class ODS_API StyleHeaderLeft : public Abstract
{
public:
	StyleHeaderLeft(Abstract *parent, Tag *tag = nullptr);
	StyleHeaderLeft(const StyleHeaderLeft &cloner);
	virtual ~StyleHeaderLeft();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;

	void
	WriteData(QXmlStreamWriter &xml) override;
	
private:
	
	void Init(ods::Tag *tag);
	
	QString style_display_;
};

} // ods::inst::
} // ods::
