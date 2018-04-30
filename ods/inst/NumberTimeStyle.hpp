#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

class ODS_API NumberTimeStyle : public Abstract
{
public:
	NumberTimeStyle(Abstract *parent, ods::Tag *tag = nullptr);
	NumberTimeStyle(const NumberTimeStyle &cloner);
	virtual ~NumberTimeStyle();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	NumberDay*
	NewDay();
	
	NumberHours*
	NewHours();
	
	NumberMinutes*
	NewMinutes();
	
	NumberMonth*
	NewMonth();
	
	NumberSeconds*
	NewSeconds();
	
	NumberText*
	NewText(const QString &s);
	
	NumberYear*
	NewYear();
	
	virtual QString*
	style_name() override { return &style_name_; }
	
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
