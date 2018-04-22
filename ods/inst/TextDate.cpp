#include "TextDate.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

TextDate::TextDate(Abstract *parent, Tag *tag)
: Abstract(parent, parent->ns(), id::TextDate)
{
	if (tag != nullptr)
		Init(tag);
}

TextDate::TextDate(const TextDate &cloner)
: Abstract(cloner)
{}

TextDate::~TextDate() {}

Abstract*
TextDate::Clone(Abstract *parent) const
{
	auto *p = new TextDate(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->style_data_style_name_ = style_data_style_name_;
	p->text_date_value_ = text_date_value_;
	
	return p;
}

void
TextDate::Init(Tag *tag)
{
	tag->Copy(ns_->style(), ods::ns::kDataStyleName, style_data_style_name_);
	tag->Copy(ns_->text(), ods::ns::kDateValue, text_date_value_);
	ScanString(tag);
}

void
TextDate::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->style(), ods::ns::kDataStyleName, style_data_style_name_);
	Write(xml, ns_->text(), ods::ns::kDateValue, text_date_value_);
	WriteNodes(xml);
}

} // ods::inst::
} // ods::
