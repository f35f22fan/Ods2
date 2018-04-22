#include "StyleTableProperties.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

StyleTableProperties::StyleTableProperties(Abstract *parent, Tag *tag)
: Abstract(parent, parent->ns(), id::StyleTableProperties)
{
	if (tag != nullptr)
		Init(tag);
}

StyleTableProperties::StyleTableProperties(const StyleTableProperties &cloner)
: Abstract(cloner)
{}

StyleTableProperties::~StyleTableProperties() {}

Abstract*
StyleTableProperties::Clone(Abstract *parent) const
{
	auto *p = new StyleTableProperties(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->table_display_ = table_display_;
	p->style_writing_mode_ = style_writing_mode_;
	
	return p;
}

void
StyleTableProperties::Init(Tag *tag)
{
	tag->Copy(ns_->table(), ods::ns::kDisplay, table_display_);
	tag->Copy(ns_->style(), ods::ns::kWritingMode, style_writing_mode_);
}

void
StyleTableProperties::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->table(), ods::ns::kDisplay, table_display_);
	Write(xml, ns_->style(), ods::ns::kWritingMode, style_writing_mode_);
}

} // ods::inst::
} // ods::
