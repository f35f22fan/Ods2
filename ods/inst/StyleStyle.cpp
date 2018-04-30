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

#include "../Book.hpp"
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
	
	mtl_line();
	return nullptr;
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
StyleStyle::SetFamily(const style::Family f)
{
	style_family_ = f;
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
