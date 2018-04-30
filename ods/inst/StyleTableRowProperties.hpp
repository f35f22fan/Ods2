#pragma once

#include "Abstract.hpp"
#include "decl.hxx"

#include "../err.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

class ODS_API StyleTableRowProperties : public Abstract
{
public:
	StyleTableRowProperties(ods::inst::Abstract *parent, ods::Tag *tag = nullptr);
	StyleTableRowProperties(const StyleTableRowProperties &cloner);
	virtual ~StyleTableRowProperties();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	void
	WriteData(QXmlStreamWriter &xml) override;
	
private:
	
	void Init(ods::Tag *tag);
	
	QString fo_break_before_;
	QString style_use_optimal_row_height_;
	QString style_row_height_;
};

} // ods::inst::
} // ods::
