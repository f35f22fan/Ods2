#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

class ODS_API NumberPercentageStyle : public Abstract
{
public:
	NumberPercentageStyle(Abstract *parent, ods::Tag *tag = nullptr);
	NumberPercentageStyle(const NumberPercentageStyle &cloner);
	virtual ~NumberPercentageStyle();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	//==> Style Interface
	virtual QString*
	style_name() override { return &style_name_; }
	//<== Style Interface
	
	inst::NumberNumber*
	GetNumberStyle();
	
	inst::NumberNumber*
	NewNumberStyle();
	
	NumberText*
	NewNumberText();
	
	void
	style_name(const QString &s) { style_name_ = s; }
	
	void
	WriteData(QXmlStreamWriter &xml) override;
	
private:
	
	void Init(ods::Tag*);
	void Scan(ods::Tag*);
	
	QString style_name_;
};

} // ods::inst::
} // ods::
