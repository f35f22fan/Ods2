#include "NumberNumber.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

#include "../ndff/Container.hpp"
#include "../ndff/Property.hpp"

namespace ods::inst {

NumberNumber::NumberNumber(Abstract *parent, Tag *tag, ndff::Container *cntr) :
Abstract(parent, parent->ns(), id::NumberNumber)
{
	if (cntr)
		Init(cntr);
	else if (tag)
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

void NumberNumber::Init(ndff::Container *cntr)
{
	// number:number(1.2) => has attrs, has children, no char data
	using Op = ndff::Op;
	ndff::Property prop;
	QHash<UriId, QVector<ndff::Property>> attrs;
	Op op = cntr->Next(prop, Op::TS, &attrs);
	CopyAttrI8(attrs, ns_->number(), ns::kDecimalPlaces, number_decimal_places_);
	CopyAttrI8(attrs, ns_->number(), ns::kMinIntegerDigits, number_min_integer_digits_);
	
	QString grouping;
	CopyAttr(attrs, ns_->number(), ns::kGrouping, grouping);
	if (!grouping.isEmpty())
		number_grouping_ = (grouping == ns::True) ? 1 : 0;
	
	ReadStrings(cntr, op);
}

void NumberNumber::Init(Tag *tag)
{
	tag->Copy(ns_->number(), ns::kDecimalPlaces, number_decimal_places_);
	tag->Copy(ns_->number(), ns::kMinIntegerDigits, number_min_integer_digits_);
	
	QString str;
	tag->Copy(ns_->number(), ns::kGrouping, str);
	
	if (!str.isEmpty())
		number_grouping_ = (str == ns::True) ? 1 : 0;
	
	ReadStrings(tag);
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
			(number_grouping_ == 1) ? ns::True : ns::False);
	}
	
	WriteNodes(xml);
}

void NumberNumber::WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba)
{
	MTL_CHECK_VOID(ba != nullptr);
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
			(number_grouping_ == 1) ? ns::True : ns::False);
	}
	
	CloseBasedOnChildren(h, kw, file, ba);
}

} // ods::inst::
