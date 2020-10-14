#include "StyleTableRowProperties.hpp"

#include "../Length.hpp"
#include "../Ns.hpp"
#include "../ns.hxx"
#include "../str.hxx"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

StyleTableRowProperties::StyleTableRowProperties(Abstract *parent, ods::Tag *tag)
: Abstract(parent, parent->ns(), id::StyleTableRowProperties)
{
	if (tag != nullptr)
		Init(tag);
}

StyleTableRowProperties::StyleTableRowProperties(const StyleTableRowProperties &cloner)
: Abstract(cloner)
{}

StyleTableRowProperties::~StyleTableRowProperties()
{
	delete style_row_height_;
	style_row_height_ = nullptr;
}

Abstract*
StyleTableRowProperties::Clone(Abstract *parent) const
{
	auto *p = new StyleTableRowProperties(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->fo_break_before_ = fo_break_before_;
	p->style_use_optimal_row_height_ = style_use_optimal_row_height_;
	p->style_row_height_ = style_row_height_;

	return p;
}

void
StyleTableRowProperties::Init(ods::Tag *tag)
{
	tag->Copy(ns_->fo(), ods::ns::kBreakBefore, fo_break_before_);
	tag->Copy(ns_->style(), ods::ns::kUseOptimalRowHeight, style_use_optimal_row_height_);
	tag->Copy(ns_->style(), ods::ns::kRowHeight, &style_row_height_);
	ScanString(tag);
}

void
StyleTableRowProperties::SetOptimal(Length *size)
{
	style_use_optimal_row_height_ = ods::Bool::True;
	style_row_height_ = size;
}

void
StyleTableRowProperties::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->fo(), ods::ns::kBreakBefore, fo_break_before_);
	Write(xml, ns_->style(), ods::ns::kUseOptimalRowHeight, style_use_optimal_row_height_);
	Write(xml, ns_->style(), ods::ns::kRowHeight, style_row_height_);
	WriteNodes(xml);
}

} // ods::inst::
} // ods::
