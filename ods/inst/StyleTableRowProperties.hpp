#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"
#include "../ods.hxx"

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
	SetOptimal(Length *size);
	
	void
	WriteData(QXmlStreamWriter &xml) override;
	
private:
	
	void Init(ods::Tag *tag);
	
	QString fo_break_before_;
	ods::Bool style_use_optimal_row_height_ = ods::Bool::None;
	Length *style_row_height_ = nullptr;
};

} // ods::inst::
} // ods::
