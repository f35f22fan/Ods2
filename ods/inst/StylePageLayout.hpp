#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

class ODS_API StylePageLayout : public Abstract
{
public:
	StylePageLayout(Abstract *parent, ods::Tag *tag = nullptr);
	StylePageLayout(const StylePageLayout &cloner);
	virtual ~StylePageLayout();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	void
	WriteData(QXmlStreamWriter &xml) override;
	
private:
	
	void Init(ods::Tag *tag);
	void Scan(ods::Tag *tag);
	
	QString style_name_;
};

} // ods::inst::
} // ods::

