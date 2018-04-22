#include "StyleTableRowProperties.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
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

StyleTableRowProperties::~StyleTableRowProperties() {}

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
	tag->Copy(ns_->style(), ods::ns::kRowHeight, style_row_height_);
	ScanString(tag);
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
