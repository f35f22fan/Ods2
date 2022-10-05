#include "TextDate.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods::inst {

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

void TextDate::Init(Tag *tag)
{
	tag->Copy(ns_->style(), ns::kDataStyleName, style_data_style_name_);
	tag->Copy(ns_->text(), ns::kDateValue, text_date_value_);
	ScanString(tag);
}

void TextDate::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name(), ns::kDataStyleName, ns::kDateValue}, list);
}

void TextDate::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->text(), list);
	
	if (!style_data_style_name_.isEmpty())
	{
		Add(ns_->style(), list);
	}
}

void TextDate::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->style(), ns::kDataStyleName, style_data_style_name_);
	Write(xml, ns_->text(), ns::kDateValue, text_date_value_);
	WriteNodes(xml);
}

void TextDate::WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba)
{
	CHECK_TRUE_VOID(ba != nullptr);
	WriteTag(kw, *ba);
	WriteNdffProp(kw, *ba, ns_->style(), ns::kDataStyleName, style_data_style_name_);
	WriteNdffProp(kw, *ba, ns_->text(), ns::kDateValue, text_date_value_);
	CloseBasedOnChildren(h, kw, file, ba);
}

} // ods::inst::
