#pragma once

#include "Abstract.hpp"
#include "../decl.hxx"
#include "../err.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

class ODS_API StyleFooterLeft : public Abstract
{
public:
	StyleFooterLeft(Abstract *parent, Tag *tag = nullptr);
	StyleFooterLeft(const StyleFooterLeft &cloner);
	virtual ~StyleFooterLeft();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;

	void
	WriteData(QXmlStreamWriter &xml) override;
	
private:
	
	void Init(Tag *tag);
	
	QString style_display_;
};

} // ods::inst::
} // ods::
