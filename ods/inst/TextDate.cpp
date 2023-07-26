#include "TextDate.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"
#include "../ndff/Container.hpp"
#include "../ndff/Property.hpp"

namespace ods::inst {

TextDate::TextDate(Abstract *parent, Tag *tag, ndff::Container *cntr)
: Abstract(parent, parent->ns(), id::TextDate)
{
	if (cntr)
		Init(cntr);
	else if (tag)
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

void TextDate::Init(ndff::Container *cntr)
{
	using Op = ndff::Op;
	ndff::Property prop;
	QHash<UriId, QVector<ndff::Property>> attrs;
	Op op = cntr->Next(prop, Op::TS, &attrs);
	CopyAttr(attrs, ns_->style(), ns::kDataStyleName, style_data_style_name_);
	CopyAttr(attrs, ns_->text(), ns::kDateValue, text_date_value_);
	ReadStrings(cntr, op);
}

void TextDate::Init(Tag *tag)
{
	tag->Copy(ns_->style(), ns::kDataStyleName, style_data_style_name_);
	tag->Copy(ns_->text(), ns::kDateValue, text_date_value_);
	ReadStrings(tag);
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
	MTL_CHECK_VOID(ba != nullptr);
	WriteTag(kw, *ba);
	WriteNdffProp(kw, *ba, ns_->style(), ns::kDataStyleName, style_data_style_name_);
	WriteNdffProp(kw, *ba, ns_->text(), ns::kDateValue, text_date_value_);
	CloseBasedOnChildren(h, kw, file, ba);
}

} // ods::inst::
