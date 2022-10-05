#include "NumberNumber.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../str.hxx"
#include "../Tag.hpp"

namespace ods::inst {

NumberNumber::NumberNumber(Abstract *parent, Tag *tag) :
Abstract(parent, parent->ns(), id::NumberNumber)
{
	if (tag != nullptr)
		Init(tag);
}

NumberNumber::NumberNumber(const NumberNumber &cloner)
: Abstract(cloner)
{}

NumberNumber::~NumberNumber() {}

Abstract*
NumberNumber::Clone(Abstract *parent) const
{
	auto *p = new NumberNumber(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	//p->loext_min_decimal_places_ = loext_min_decimal_places_;
	p->number_decimal_places_ = number_decimal_places_;
	p->number_min_integer_digits_ = number_min_integer_digits_;
	p->number_grouping_ = number_grouping_;
	
	p->CloneChildrenOf(this, ClonePart::Text);

	return p;
}

void NumberNumber::Init(ods::Tag *tag)
{
	tag->Copy(ns_->number(), ns::kDecimalPlaces, number_decimal_places_);
	tag->Copy(ns_->number(), ns::kMinIntegerDigits, number_min_integer_digits_);
	
	QString str;
	tag->Copy(ns_->number(), ns::kGrouping, str);
	
	if (!str.isEmpty())
		number_grouping_ = (str == ods::str::True) ? 1 : 0;
	
	ScanString(tag);
}

void NumberNumber::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name(),ns::kDecimalPlaces,
		ns::kMinIntegerDigits, ns::kGrouping}, list);
}

void NumberNumber::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->number(), list);
}

void NumberNumber::WriteData(QXmlStreamWriter &xml)
{
	if (number_decimal_places_ != -1)
		Write(xml, ns_->number(), ns::kDecimalPlaces,
			QString::number(number_decimal_places_));
	
	if (number_min_integer_digits_ != -1)
		Write(xml, ns_->number(), ns::kMinIntegerDigits,
			QString::number(number_min_integer_digits_));
	
	if (number_grouping_ != -1)
	{
		Write(xml, ns_->number(), ns::kGrouping,
			(number_grouping_ == 1) ? ods::str::True : ods::str::False);
	}
	
	WriteNodes(xml);
}

void NumberNumber::WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba)
{
	CHECK_TRUE_VOID(ba != nullptr);
	WriteTag(kw, *ba);
	
	if (number_decimal_places_ != -1)
		WriteNdffProp(kw, *ba, ns_->number(), ns::kDecimalPlaces,
			QString::number(number_decimal_places_));
	
	if (number_min_integer_digits_ != -1)
		WriteNdffProp(kw, *ba, ns_->number(), ns::kMinIntegerDigits,
			QString::number(number_min_integer_digits_));
	
	if (number_grouping_ != -1)
	{
		WriteNdffProp(kw, *ba, ns_->number(), ns::kGrouping,
			(number_grouping_ == 1) ? ods::str::True : ods::str::False);
	}
	
	CloseBasedOnChildren(h, kw, file, ba);
}

} // ods::inst::
