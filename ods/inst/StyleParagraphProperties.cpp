#include "StyleParagraphProperties.hpp"

#include "../HAlign.hpp"
#include "../Length.hpp"
#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods::inst {

StyleParagraphProperties::StyleParagraphProperties(Abstract *parent, ods::Tag *tag)
: Abstract(parent, parent->ns(), id::StyleParagraphProperties)
{
	if (tag)
		Init(tag);
}

StyleParagraphProperties::StyleParagraphProperties(const StyleParagraphProperties &cloner)
: Abstract(cloner)
{}

StyleParagraphProperties::~StyleParagraphProperties()
{
	delete fo_margin_left_;
	delete fo_text_align_;
}

Abstract*
StyleParagraphProperties::Clone(Abstract *parent) const
{
	auto *p = new StyleParagraphProperties(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->style_tab_stop_distance_ = style_tab_stop_distance_;
	
	if (fo_margin_left_ != nullptr)
		p->fo_margin_left_ = fo_margin_left_->Clone();
	
	if (fo_text_align_ != nullptr)
		p->fo_text_align_ = fo_text_align_->Clone();
	
	return p;
}

void StyleParagraphProperties::Init(ods::Tag *tag)
{
	tag->Copy(ns_->style(), ns::kTabStopDistance, style_tab_stop_distance_);
	
	QString str;
	tag->Copy(ns_->fo(), ns::kMarginLeft, str);
	fo_margin_left_ = ods::Length::FromString(str);
	
	tag->Copy(ns_->fo(), ns::kTextAlign, str);
	fo_text_align_ = HAlign::FromString(str);
}

void StyleParagraphProperties::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name(), ns::kTabStopDistance, ns::kMarginLeft,
		ns::kTextAlign}, list);
}

void StyleParagraphProperties::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->style(), list);
	
	if (fo_margin_left_ || fo_text_align_)
		Add(ns_->fo(), list);
}

void StyleParagraphProperties::text_align(ods::HAlign *a)
{
	delete fo_text_align_;
	fo_text_align_ = a ? a->Clone() : nullptr;
}

void StyleParagraphProperties::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->style(), ns::kTabStopDistance, style_tab_stop_distance_);
	
	if (fo_margin_left_ != nullptr)
		Write(xml, ns_->fo(), ns::kMarginLeft, fo_margin_left_->toString());
	
	if (fo_text_align_ != nullptr)
		Write(xml, ns_->fo(), ns::kTextAlign, fo_text_align_->toString());
}

} // ods::inst::
