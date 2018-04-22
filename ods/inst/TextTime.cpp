#include "TextTime.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

TextTime::TextTime(Abstract *parent, Tag *tag)
: Abstract (parent, parent->ns(), id::TextTime)
{
	if (tag != nullptr)
		Init(tag);
}

TextTime::TextTime(const TextTime &cloner)
: Abstract(cloner)
{}

TextTime::~TextTime() {}

Abstract*
TextTime::Clone(Abstract *parent) const
{
	auto *p = new TextTime(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->style_data_style_name_ = style_data_style_name_;
	p->text_time_value_ = text_time_value_;
	
	return p;
}

void
TextTime::Init(ods::Tag *tag)
{
	tag->Copy(ns_->style(), ods::ns::kDataStyleName, style_data_style_name_);
	tag->Copy(ns_->text(), ods::ns::kTimeValue, text_time_value_);
	ScanString(tag);
}

void
TextTime::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->style(), ods::ns::kDataStyleName, style_data_style_name_);
	Write(xml, ns_->text(), ods::ns::kTimeValue, text_time_value_);
	WriteNodes(xml);
}

} // ods::inst::
} // ods::
