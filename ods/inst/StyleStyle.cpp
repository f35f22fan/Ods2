#include "StyleStyle.hpp"

#include "NumberBooleanStyle.hpp"
#include "NumberCurrencyStyle.hpp"
#include "NumberDateStyle.hpp"
#include "NumberPercentageStyle.hpp"
#include "NumberText.hpp"
#include "NumberTimeStyle.hpp"
#include "OfficeAutomaticStyles.hpp"
#include "OfficeDocumentContent.hpp"
#include "StyleParagraphProperties.hpp"
#include "StyleTableCellProperties.hpp"
#include "StyleTableColumnProperties.hpp"
#include "StyleTableProperties.hpp"
#include "StyleTableRowProperties.hpp"
#include "StyleTextProperties.hpp"

#include "../attr/Border.hpp"
#include "../attr/FoFontWeight.hpp"

#include "../Book.hpp"
#include "../LineStyle.hpp"
#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

StyleStyle::StyleStyle(Abstract *parent, ods::Tag *tag)
: Abstract (parent, parent->ns(), id::StyleStyle)
{
	if (tag != nullptr)
		Init(tag);
}

StyleStyle::StyleStyle(const StyleStyle &cloner)
: Abstract(cloner)
{
}

StyleStyle::~StyleStyle()
{}

Abstract*
StyleStyle::Clone(Abstract *parent) const
{
	auto *p = new StyleStyle(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->style_data_style_name_ = style_data_style_name_;
	p->style_family_ = style_family_;
	p->style_display_name_ = style_display_name_;
	p->style_master_page_name_ = style_master_page_name_;
	p->style_name_ = style_name_;
	p->style_parent_style_name_ = style_parent_style_name_;
	return p;
}

StyleStyle*
StyleStyle::DeriveCellStyle()
{
	auto *p = book_->NewCellStyle();
	p->SetParentStyle(this);
	return p;
}

NumberCurrencyStyle*
StyleStyle::FetchNumberCurrencyStyle()
{
	auto *tcp = (ods::inst::NumberCurrencyStyle*)
		Get(ods::Id::NumberCurrencyStyle);
	
	if (tcp == nullptr)
		tcp = NewCurrencyStyle();
	
	return tcp;
}

inst::StyleTableCellProperties*
StyleStyle::FetchStyleTableCellProperties()
{
	auto *p = (StyleTableCellProperties*)Get(Id::StyleTableCellProperties);
	
	if (p == nullptr) {
		p = new StyleTableCellProperties(this);
		Append(p);
	}
	
	return p;
}

StyleTextProperties*
StyleStyle::FetchStyleTextProperties()
{
	auto *p = (StyleTextProperties*)Get(Id::StyleTextProperties);
	
	if (p == nullptr) {
		p = new StyleTextProperties(this);
		Append(p);
	}
	
	return p;
}

StyleTableCellProperties*
StyleStyle::FetchTableCellProperties()
{
	auto *tcp = (ods::inst::StyleTableCellProperties*)
		Get(ods::Id::StyleTableCellProperties);
	
	if (tcp == nullptr)
		tcp = NewTableCellProperties();
	
	return tcp;
}

NumberBooleanStyle*
StyleStyle::GetBooleanStyle() const
{
	auto *ds = GetDataStyle();
	
	if (ds == nullptr)
		return nullptr;
	
	if (ds->Is(Id::NumberBooleanStyle))
		return (inst::NumberBooleanStyle*) ds;
	
	return nullptr;
}

Abstract*
StyleStyle::GetDataStyle() const
{
	if (style_data_style_name_.isEmpty())
		return nullptr;
	
	return GetAnyStyle(style_data_style_name_);
}

NumberCurrencyStyle*
StyleStyle::GetCurrencyStyle() const
{
	auto *ds = GetDataStyle();
	
	if (ds == nullptr)
		return nullptr;
	
	if (ds->Is(Id::NumberCurrencyStyle))
		return (inst::NumberCurrencyStyle*) ds;
	
	return nullptr;
}

NumberDateStyle*
StyleStyle::GetDateStyle() const
{
	auto *ds = GetDataStyle();
	
	if (ds == nullptr)
		return nullptr;
	
	if (ds->Is(Id::NumberDateStyle))
		return (inst::NumberDateStyle*) ds;
	
	return nullptr;
}

NumberPercentageStyle*
StyleStyle::GetPercentageStyle() const
{
	auto *ds = GetDataStyle();
	
	if (ds == nullptr)
	{
		mtl_line();
		return nullptr;
	}
	
	if (ds->Is(Id::NumberPercentageStyle))
		return (inst::NumberPercentageStyle*) ds;
	
	return nullptr;
}

StyleTableRowProperties*
StyleStyle::GetTableRowProperties() const
{
	auto *p = Get(Id::StyleTableRowProperties);
	
	if (p == nullptr)
		return nullptr;
	
	return (inst::StyleTableRowProperties*)p;
}

NumberTimeStyle*
StyleStyle::GetTimeStyle() const
{
	auto *ds = GetDataStyle();
	
	if (ds == nullptr)
		return nullptr;
	
	if (ds->Is(Id::NumberTimeStyle))
		return (inst::NumberTimeStyle*) ds;
	
	return nullptr;
}

void
StyleStyle::Init(ods::Tag *tag)
{
	tag->Copy(ns_->style(), ods::ns::kDataStyleName, style_data_style_name_);
	
	QString str;
	tag->Copy(ns_->style(), ods::ns::kFamily, str);
	style_family_ = style::FamilyFromString(str);
	
	tag->Copy(ns_->style(), ods::ns::kMasterPageName, style_master_page_name_);
	tag->Copy(ns_->style(), ods::ns::kName, style_name_);
	tag->Copy(ns_->style(), ods::ns::kParentStyleName, style_parent_style_name_);
	tag->Copy(ns_->style(), ods::ns::kDisplayName, style_display_name_);
	
	Scan(tag);
}

NumberBooleanStyle*
StyleStyle::NewBooleanStyle()
{
	auto *dc = book_->document_content();
	auto *aus = dc->automatic_styles();
	auto *p = aus->NewNumberBooleanStyle();
	style_data_style_name_ = *p->style_name();
	return p;
}

NumberCurrencyStyle*
StyleStyle::NewCurrencyStyle()
{
	auto *dc = book_->document_content();
	auto *aus = dc->automatic_styles();
	auto *p = aus->NewNumberCurrencyStyle();
	style_data_style_name_ = *p->style_name();
	return p;
}

NumberDateStyle*
StyleStyle::NewDateStyle()
{
	auto *dc = book_->document_content();
	auto *aus = dc->automatic_styles();
	auto *p = aus->NewNumberDateStyle();
	style_data_style_name_ = *p->style_name();
	return p;
}

inst::StyleParagraphProperties*
StyleStyle::NewParagraphProperties()
{
	auto *p = new StyleParagraphProperties(this);
	Append(p);
	return p;
}

NumberPercentageStyle*
StyleStyle::NewPercentageStyle()
{
	auto *dc = book_->document_content();
	auto *aus = dc->automatic_styles();
	auto *p = aus->NewNumberPercentageStyle();
	style_data_style_name_ = *p->style_name();
	return p;
}

StyleTableCellProperties*
StyleStyle::NewTableCellProperties()
{
	auto *p = new StyleTableCellProperties(this);
	Append(p);
	return p;
}

StyleTableColumnProperties*
StyleStyle::NewTableColumnProperties()
{
	auto *tcp = new StyleTableColumnProperties(this);
	Append(tcp);
	return tcp;
}

StyleTableRowProperties*
StyleStyle::NewTableRowProperties()
{
	auto *p = new StyleTableRowProperties(this);
	Append(p);
	return p;
}

NumberTimeStyle*
StyleStyle::NewTimeStyle()
{
	auto *dc = book_->document_content();
	auto *aus = dc->automatic_styles();
	auto *p = aus->NewNumberTimeStyle();
	style_data_style_name_ = *p->style_name();
	return p;
}

StyleFontFace*
StyleStyle::QueryFontFace() const
{
	auto *tp = (inst::StyleTextProperties*)Get(Id::StyleTextProperties);
	
	if (tp == nullptr)
		return nullptr;
	
	return tp->font_face();
}

void
StyleStyle::Scan(ods::Tag *tag)
{
	for (auto *x: tag->nodes())
	{
		if (AddText(x))
			continue;

		auto *next = x->as_tag();
		
		if (next->Is(ns_->style(), ods::ns::kTextProperties))
		{
			Append(new StyleTextProperties(this, next));
		} else if (next->Is(ns_->style(), ods::ns::kTableCellProperties)) {
			Append(new StyleTableCellProperties(this, next));
		} else if (next->Is(ns_->style(), ods::ns::kTableColumnProperties)) {
			Append(new StyleTableColumnProperties(this, next));
		} else if (next->Is(ns_->style(), ods::ns::kTableProperties)) {
			Append(new StyleTableProperties(this, next));
		} else if (next->Is(ns_->style(), ods::ns::kTableRowProperties)) {
			Append(new StyleTableRowProperties(this, next));
		} else if (next->Is(ns_->style(), ods::ns::kParagraphProperties)) {
			Append(new StyleParagraphProperties(this, next));
		} else {
			Scan(next);
		}
	}
}

void
StyleStyle::SetBackgroundColor(const QColor &color)
{
	// set background color:
	auto *stcp = FetchStyleTableCellProperties();
	stcp->SetBackgroundColor(color);
}

void
StyleStyle::SetBoldText(const bool bold)
{
	ods::inst::StyleTextProperties *tp = FetchStyleTextProperties();

	// set font weight:
	auto *font_weight = new ods::attr::FoFontWeight();
	
	if (bold)
		font_weight->SetBold();
	else
		font_weight->SetNormal();
	
	tp->SetFontWeight(font_weight);
}

void
StyleStyle::SetBorder(const ods::Length &width, const QColor &color,
	const ods::line::Style &line_style, const u8 sides)
{
	auto *tcp = FetchTableCellProperties();
	ods::attr::Border border;
	border.width(&width);
	border.color(&color);
	ods::LineStyle line_style_class;
	line_style_class.Set(line_style);
	border.line_style(&line_style_class);
	
	if (sides == ods::BorderAll) {
		tcp->border(&border);
		return;
	}
	
	if (sides & ods::BorderLeft)
		tcp->border_left(&border);
	if (sides & ods::BorderRight)
		tcp->border_right(&border);
	if (sides & ods::BorderTop)
		tcp->border_top(&border);
	if (sides & ods::BorderBottom)
		tcp->border_bottom(&border);
}

void
StyleStyle::SetDataStyle(NumberCurrencyStyle *p) {
	style_data_style_name_ = *p->style_name();
}

void
StyleStyle::SetFamily(const style::Family f)
{
	style_family_ = f;
}

void
StyleStyle::SetFontStyle(const ods::attr::FontStyle font_style)
{
	ods::inst::StyleTextProperties *tp = FetchStyleTextProperties();
	auto *fo_font_style = new ods::attr::FoFontStyle(font_style);
	tp->SetFontStyle(fo_font_style);
}

void
StyleStyle::SetHAlignment(const HAlignSide place)
{
	auto *spp = (ods::inst::StyleParagraphProperties*)
		Get(ods::Id::StyleParagraphProperties);
	
	if (spp == nullptr)
		spp = NewParagraphProperties();
	
	ods::HAlign ha(place);
	spp->text_align(&ha);
}

void
StyleStyle::SetVAlignment(const VAlignSide place)
{
	auto *tcp = (ods::inst::StyleTableCellProperties*)
		Get(ods::Id::StyleTableCellProperties);
	
	if (tcp == nullptr)
		tcp = NewTableCellProperties();
	
	ods::VAlign va(place);
	tcp->vertical_align(&va);
}

void
StyleStyle::SetMasterPageName(const QString &s)
{
	style_master_page_name_ = s;
}

void
StyleStyle::SetName(const QString &s)
{
	style_name_ = s;
}

void
StyleStyle::SetParentStyle(StyleStyle *s)
{
	SetParentStyleName(*s->style_name());
}

void
StyleStyle::SetParentStyleName(const QString &s)
{
	style_parent_style_name_ = s;
}

void
StyleStyle::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->style(), ods::ns::kDataStyleName, style_data_style_name_);
	
	if (style_family_ != style::Family::None)
	{
		auto *str = style::FamilyToString(style_family_);
		Write(xml, ns_->style(), ods::ns::kFamily, str);
	}
	
	Write(xml, ns_->style(), ods::ns::kMasterPageName, style_master_page_name_);
	Write(xml, ns_->style(), ods::ns::kName, style_name_);
	Write(xml, ns_->style(), ods::ns::kParentStyleName, style_parent_style_name_);
	Write(xml, ns_->style(), ods::ns::kDisplayName, style_display_name_);
	
	WriteNodes(xml);
}

} // ods::inst::
} // ods::
