#pragma once

#include "Abstract.hpp"
#include "decl.hxx"

#include "../err.hpp"
#include "../style.hh"

namespace ods { // ods::
namespace inst { // ods::inst::

class ODS_API StyleStyle : public Abstract
{
public:
	StyleStyle(Abstract *parent, ods::Tag *tag = nullptr);
	StyleStyle(const StyleStyle &cloner);
	virtual ~StyleStyle();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	virtual QString*
	data_style_name() override { return &style_data_style_name_; }
	
	StyleStyle*
	DeriveCellStyle();
	
	style::Family
	family() const { return style_family_; }
	
	NumberBooleanStyle*
	GetBooleanStyle() const;
	
	NumberCurrencyStyle*
	GetCurrencyStyle() const;
	
	inst::Abstract*
	GetDataStyle() const;
	
	NumberDateStyle*
	GetDateStyle() const;
	
	NumberPercentageStyle*
	GetPercentageStyle() const;
	
	NumberTimeStyle*
	GetTimeStyle() const;
	
	const QString&
	master_page_name() const { return style_master_page_name_; }
	
	NumberBooleanStyle*
	NewBooleanStyle();
	
	NumberCurrencyStyle*
	NewCurrencyStyle();
	
	NumberDateStyle*
	NewDateStyle();
	
	StyleParagraphProperties*
	NewParagraphProperties();
	
	NumberPercentageStyle*
	NewPercentageStyle();
	
	StyleTableCellProperties*
	NewTableCellProperties();
	
	StyleTableColumnProperties*
	NewTableColumnProperties();
	
	NumberTimeStyle*
	NewTimeStyle();
	
	StyleFontFace*
	QueryFontFace() const;
	
	void
	SetFamily(const style::Family f);
	
	void
	SetMasterPageName(const QString &s);
	
	void
	SetName(const QString &s);
	
	void
	SetParentStyle(StyleStyle *s);
	
	void
	SetParentStyleName(const QString &s);
	
	virtual QString*
	style_name() override { return &style_name_; }
	
	void
	style_name(const QString &s) { style_name_ = s; }
	
	virtual QString*
	parent_style_name() override { return &style_parent_style_name_; }
	
	void
	WriteData(QXmlStreamWriter &xml) override;
	
private:

	void Init(ods::Tag*);
	void Scan(ods::Tag*);

	QString style_data_style_name_;
	style::Family style_family_ = style::Family::None;
	QString style_display_name_;
	QString style_master_page_name_;
	QString style_name_;
	QString style_parent_style_name_;
};

} // ods::inst::
} // ods::
