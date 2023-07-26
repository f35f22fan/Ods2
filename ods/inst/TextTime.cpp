#include "TextTime.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

#include "../ndff/Container.hpp"
#include "../ndff/Property.hpp"

namespace ods::inst {

TextTime::TextTime(Abstract *parent, Tag *tag, ndff::Container *cntr)
: Abstract (parent, parent->ns(), id::TextTime)
{
	if (cntr)
		Init(cntr);
	else if (tag)
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

void TextTime::Init(ndff::Container *cntr)
{
	using Op = ndff::Op;
	ndff::Property prop;
	QHash<UriId, QVector<ndff::Property>> attrs;
	Op op = cntr->Next(prop, Op::TS, &attrs);
	CopyAttr(attrs, ns_->style(), ns::kDataStyleName, style_data_style_name_);
	CopyAttr(attrs, ns_->text(), ns::kTimeValue, text_time_value_);
	ReadStrings(cntr, op);
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

void TextTime::WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba)
{
	MTL_CHECK_VOID(ba);
	WriteTag(kw, *ba);
	WriteNdffProp(kw, *ba, ns_->style(), ns::kDataStyleName, style_data_style_name_);
	WriteNdffProp(kw, *ba, ns_->text(), ns::kTimeValue, text_time_value_);
	CloseBasedOnChildren(h, kw, file, ba);
}

} // ods::inst::
