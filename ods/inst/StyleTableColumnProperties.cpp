#include "StyleTableColumnProperties.hpp"

#include "../Length.hpp"
#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

StyleTableColumnProperties::StyleTableColumnProperties(Abstract *parent,
ods::Tag *tag) : Abstract(parent, parent->ns(), ods::id::StyleTableColumnProperties)
{
	if (tag != nullptr)
		Init(tag);

//	mtl_line("func: %p", (void*)func());
//	mtl_line("tcp func: %p", (void*) ods::id::StyleTableColumnProperties);
}

StyleTableColumnProperties::StyleTableColumnProperties(const StyleTableColumnProperties &cloner)
: Abstract(cloner)
{}

StyleTableColumnProperties::~StyleTableColumnProperties()
{
	delete style_column_width_;
}

Abstract*
StyleTableColumnProperties::Clone(Abstract *parent) const
{
	auto *p = new StyleTableColumnProperties(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->fo_break_before_ = fo_break_before_;
	
	if (style_column_width_ != nullptr)
		p->style_column_width_ = style_column_width_->Clone();
	
	return p;
}

void
StyleTableColumnProperties::Init(ods::Tag *tag)
{
	tag->Copy(ns_->fo(), ods::ns::kBreakBefore, fo_break_before_);
	
	QString str;
	tag->Copy(ns_->style(), ods::ns::kColumnWidth, str);
	style_column_width_ = Length::FromString(str);
	
	ScanString(tag);
}

void
StyleTableColumnProperties::SetColumnWidth(Length *length)
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
	Write(xml, ns_->fo(), ods::ns::kBreakBefore, fo_break_before_);
	
	if (style_column_width_ != nullptr)
	{
		Write(xml, ns_->style(), ods::ns::kColumnWidth,
			style_column_width_->toString());
	}
	
	WriteNodes(xml);
}

} // ods::inst::
} // ods::
