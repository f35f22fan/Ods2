#include "NumberSeconds.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods::inst {

NumberSeconds::NumberSeconds(Abstract *parent, ods::Tag *tag)
: Abstract(parent, parent->ns(), id::NumberSeconds)
{
	if (tag != nullptr)
		Init(tag);
}

NumberSeconds::NumberSeconds(const NumberSeconds &cloner)
: Abstract(cloner)
{}

NumberSeconds::~NumberSeconds() {}

Abstract*
NumberSeconds::Clone(Abstract *parent) const
{
	auto *p = new NumberSeconds(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->number_style_ = number_style_;
	p->number_decimal_places_ = number_decimal_places_;
	p->CloneChildrenOf(this, ClonePart::Text);
	
	return p;
}

void NumberSeconds::Init(ods::Tag *tag)
{
	tag->Copy(ns_->number(), ns::kStyle, number_style_);
	tag->Copy(ns_->number(), ns::kDecimalPlaces, number_decimal_places_);
	ScanString(tag);
}

void NumberSeconds::ListKeywords(inst::Keywords &list, const inst::LimitTo lt)
{
	inst::AddKeywords({tag_name(), ns::kStyle, ns::kDecimalPlaces}, list);
}

void NumberSeconds::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->number(), list);
}

void NumberSeconds::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->number(), ods::ns::kStyle, number_style_);
	
	if (number_decimal_places_ != -1)
	{
		Write(xml, ns_->number(), ods::ns::kDecimalPlaces,
			QString::number(number_decimal_places_));
	}
	
	WriteNodes(xml);
}

void NumberSeconds::WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba)
{
	CHECK_TRUE_VOID(ba != nullptr);
	WriteTag(kw, *ba);
	if (number_decimal_places_ != -1)
	{
		WriteNdffProp(kw, *ba, ns_->number(), ods::ns::kDecimalPlaces,
			QString::number(number_decimal_places_));
	}
	
	CloseBasedOnChildren(h, kw, file, ba);
}

} // ods::inst::
