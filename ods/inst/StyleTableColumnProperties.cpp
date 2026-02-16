#include "StyleTableColumnProperties.hpp"

#include "../Length.hpp"
#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"
#include "../attr/VisualBreak.hpp"

namespace ods::inst {

StyleTableColumnProperties::StyleTableColumnProperties(Abstract *parent,
ods::Tag *tag) : Abstract(parent, parent->ns(), ods::id::StyleTableColumnProperties)
{
	if (tag)
		Init(tag);
}

StyleTableColumnProperties::StyleTableColumnProperties(const StyleTableColumnProperties &cloner)
: Abstract(cloner)
{}

StyleTableColumnProperties::~StyleTableColumnProperties()
{
	delete style_column_width_;
	style_column_width_ = nullptr;
	
	delete visual_break_;
	visual_break_ = nullptr;
}

Abstract*
StyleTableColumnProperties::Clone(Abstract *parent) const
{
	auto *p = new StyleTableColumnProperties(*this);
	
	if (parent)
		p->parent(parent);
	
	if (visual_break_) {
		p->visual_break_ = visual_break_->Clone();
	}
	
	if (style_column_width_)
		p->style_column_width_ = style_column_width_->Clone();
	
	p->CloneChildrenOf(this);
	
	return p;
}

void StyleTableColumnProperties::Init(ods::Tag *tag)
{
	QString input_str;
	{
		tag->Copy(ns_->fo(), ns::kBreakBefore, input_str);
		visual_break_ = attr::VisualBreak::FromString(input_str);
		
		if (!visual_break_) {
			tag->Copy(ns_->fo(), ns::kBreakAfter, input_str);
			visual_break_ = attr::VisualBreak::FromString(input_str);
		}
		input_str.clear();
	}
	
	tag->Copy(ns_->style(), ns::kColumnWidth, input_str);
	style_column_width_ = Length::FromString(input_str);
	
	ReadStrings(tag);
}

void StyleTableColumnProperties::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name(), ns::kBreakBefore, ns::kBreakAfter, ns::kColumnWidth}, list);
}

void StyleTableColumnProperties::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->style(), list);
	
	if (visual_break_)
	{
		Add(ns_->fo(), list);
	}
}

void StyleTableColumnProperties::SetColumnWidth(Length *length)
{
	delete style_column_width_;

	if (length == nullptr)
		style_column_width_ = nullptr;
	else
		style_column_width_ = length->Clone();
}

void
StyleTableColumnProperties::WriteData(QXmlStreamWriter &xml)
{
	if (visual_break_) {
		cauto name = visual_break_->before() ? ns::kBreakBefore : ns::kBreakAfter;
		Write(xml, ns_->fo(), name, visual_break_->toString());
	}
	
	if (style_column_width_)
	{
		Write(xml, ns_->style(), ns::kColumnWidth,
			style_column_width_->toString());
	}
	
	WriteNodes(xml);
}

} // ods::inst::
