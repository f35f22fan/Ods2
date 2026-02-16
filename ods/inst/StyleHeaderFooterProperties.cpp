#include "StyleHeaderFooterProperties.hpp"

#include "StyleBackgroundImage.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods::inst {

StyleHeaderFooterProperties::StyleHeaderFooterProperties(Abstract *parent, ods::Tag *tag)
: Abstract(parent, parent->ns(), id::StyleHeaderFooterProperties)
{
	if (tag)
		Init(tag);
}

StyleHeaderFooterProperties::StyleHeaderFooterProperties(const StyleHeaderFooterProperties &cloner)
: Abstract(cloner)
{}

StyleHeaderFooterProperties::~StyleHeaderFooterProperties() {}

Abstract*
StyleHeaderFooterProperties::Clone(Abstract *parent) const
{
	auto *p = new StyleHeaderFooterProperties(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->fo_background_color_ = fo_background_color_;
	p->fo_border_ = fo_border_;
	p->fo_padding_ = fo_padding_;
	p->fo_min_height_ = fo_min_height_;
	p->fo_margin_left_ = fo_margin_left_;
	p->fo_margin_top_ = fo_margin_top_;
	p->fo_margin_right_ = fo_margin_right_;
	p->fo_margin_bottom_ = fo_margin_bottom_;
	p->CloneChildrenOf(this);
	
	return p;
}

void StyleHeaderFooterProperties::Init(ods::Tag *tag)
{
	tag->Copy(ns_->fo(), ns::kBackgroundColor, fo_background_color_);
	tag->Copy(ns_->fo(), ns::kBorder, fo_border_);
	tag->Copy(ns_->fo(), ns::kPadding, fo_padding_);
	tag->Copy(ns_->fo(), ns::kMinHeight, fo_min_height_);
	tag->Copy(ns_->fo(), ns::kMarginLeft, fo_margin_left_);
	tag->Copy(ns_->fo(), ns::kMarginTop, fo_margin_top_);
	tag->Copy(ns_->fo(), ns::kMarginRight, fo_margin_right_);
	tag->Copy(ns_->fo(), ns::kMarginBottom, fo_margin_bottom_);
	
	Scan(tag);
}

void StyleHeaderFooterProperties::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name(), ns::kBackgroundColor,
		ns::kBorder, ns::kPadding, ns::kMinHeight, ns::kMarginLeft,
		ns::kMarginTop, ns::kMarginRight, ns::kMarginBottom}, list);
}

void StyleHeaderFooterProperties::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->style(), list);
	
	if (!fo_background_color_.isEmpty() || !fo_border_.isEmpty() ||
		!fo_padding_.isEmpty() || !fo_min_height_.isEmpty() ||
		!fo_margin_left_.isEmpty() || !fo_margin_top_.isEmpty() ||
		!fo_margin_right_.isEmpty() || !fo_margin_bottom_.isEmpty())
	{
		Add(ns_->fo(), list);
	}
}

void StyleHeaderFooterProperties::Scan(Tag *tag)
{
	for (auto *x: tag->nodes())
	{
		if (AddText(x))
			continue;
		
		auto *next = x->as_tag();
		
		if (next->Is(ns_->style(), ns::kBackgroundImage)) {
			Append(new StyleBackgroundImage(this, next), TakeOwnership::Yes);
		} else {
			Scan(next);
		}
	}
}

void StyleHeaderFooterProperties::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->fo(), ns::kBackgroundColor, fo_background_color_);
	Write(xml, ns_->fo(), ns::kBorder, fo_border_);
	Write(xml, ns_->fo(), ns::kPadding, fo_padding_);
	Write(xml, ns_->fo(), ns::kMinHeight, fo_min_height_);
	Write(xml, ns_->fo(), ns::kMarginLeft, fo_margin_left_);
	Write(xml, ns_->fo(), ns::kMarginTop, fo_margin_top_);
	Write(xml, ns_->fo(), ns::kMarginRight, fo_margin_right_);
	Write(xml, ns_->fo(), ns::kMarginBottom, fo_margin_bottom_);
	WriteNodes(xml);
}

} // ods::inst::
