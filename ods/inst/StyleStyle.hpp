#pragma once

#include "Abstract.hpp"
#include "decl.hxx"

#include "../attr/Border.hpp"
#include "../attr/FoFontStyle.hpp"

#include "../err.hpp"
#include "../HAlign.hpp"
#include "../line.hxx"
#include "../style.hh"
#include "../VAlign.hpp"

namespace ods::inst {

class ODS_API StyleStyle : public Abstract
{
public:
	StyleStyle(Abstract *parent, ods::Tag *tag = 0, ndff::Container *cntr = 0);
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
	
	NumberCurrencyStyle*
	FetchNumberCurrencyStyle();
	
	NumberPercentageStyle*
	FetchPercentageStyle();
	
	inst::StyleTableCellProperties*
	FetchStyleTableCellProperties();
	
	StyleTextProperties*
	FetchStyleTextProperties();
	
	StyleTableCellProperties*
	FetchTableCellProperties();
	
	inst::StyleTableColumnProperties*
	FetchTableColumnProperties();
	
	inst::StyleTableRowProperties*
	FetchTableRowProperties();
	
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
	
	StyleTableRowProperties*
	GetTableRowProperties() const;
	
	NumberTimeStyle*
	GetTimeStyle() const;
	
	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	
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
	
	StyleTableRowProperties*
	NewTableRowProperties();
	
	NumberTimeStyle*
	NewTimeStyle();
	
	StyleFontFace*
	QueryFontFace() const;
	
	void SetBackgroundColor(const QColor &color);
	void SetBoldText(const bool bold);
	
	// "u8 sides" defined in attr/Border.hpp
	void SetBorder(const ods::Length &width, const Color &color,
		const ods::line::Style &line_style, const u8 sides = ods::BorderAll);
	
	void SetFamily(const style::Family f);
	void SetHAlignment(const ods::HAlignSide place);
	void SetMasterPageName(const QString &s);
	void SetName(const QString &s);
	void SetParentStyle(StyleStyle *s);
	void SetParentStyleName(const QString &s);
	void SetPercentage(const int min_integer_digits, const int decimal_places);
	
	virtual QString*
	style_name() override { return &style_name_; }
	
	void style_name(const QString &s) { style_name_ = s; }
	
	virtual QString*
	parent_style_name() override { return &style_parent_style_name_; }
	
	void SetDataStyle(NumberCurrencyStyle *p);
	void SetFontStyle(const ods::attr::FontStyle font_style);
	void SetVAlignment(const VAlignSide place);
	void WriteData(QXmlStreamWriter &xml) override;
	void WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba) override;
private:

	void Init(ndff::Container *cntr);
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
