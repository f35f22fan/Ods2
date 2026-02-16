#include "TextTime.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods::inst {

TextTime::TextTime(Abstract *parent, Tag *tag)
: Abstract (parent, parent->ns(), id::TextTime)
{
	if (tag)
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
	
	if (parent)
		p->parent(parent);
	
	p->style_data_style_name_ = style_data_style_name_;
	p->text_time_value_ = text_time_value_;
	p->CloneChildrenOf(this);
	
	return p;
}

void TextTime::Init(ods::Tag *tag)
{
	tag->Copy(ns_->style(), ns::kDataStyleName, style_data_style_name_);
	tag->Copy(ns_->text(), ns::kTimeValue, text_time_value_);
	ReadStrings(tag);
}

void TextTime::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name(), ns::kDataStyleName, ns::kTimeValue}, list);
}

void TextTime::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->text(), list);
	
	if (!style_data_style_name_.isEmpty())
		Add(ns_->style(), list);
}

void TextTime::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->style(), ns::kDataStyleName, style_data_style_name_);
	Write(xml, ns_->text(), ns::kTimeValue, text_time_value_);
	WriteNodes(xml);
}

} // ods::inst::
