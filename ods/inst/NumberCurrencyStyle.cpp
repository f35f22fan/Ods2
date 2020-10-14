#include "NumberCurrencyStyle.hpp"

#include "NumberCurrencySymbol.hpp"
#include "NumberNumber.hpp"
#include "NumberText.hpp"
#include "StyleMap.hpp"
#include "StyleTextProperties.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

NumberCurrencyStyle::NumberCurrencyStyle(Abstract *parent, ods::Tag *tag)
: Abstract(parent, parent->ns(), id::NumberCurrencyStyle)
{
	if (tag != nullptr)
		Init(tag);
}

NumberCurrencyStyle::NumberCurrencyStyle(const NumberCurrencyStyle &cloner)
: Abstract(cloner)
{}

NumberCurrencyStyle::~NumberCurrencyStyle()
{}

Abstract*
NumberCurrencyStyle::Clone(Abstract *parent) const
{
	auto *p = new NumberCurrencyStyle(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->style_name_ = style_name_;
	p->style_volatile_ = style_volatile_;

	return p;
}

NumberCurrencySymbol*
NumberCurrencyStyle::FetchCurrencySymbol()
{
	auto *p = (ods::inst::NumberCurrencySymbol*)
		Get(ods::Id::NumberCurrencySymbol);
	
	if (p == nullptr)
		p = NewCurrencySymbol();
	
	return p;
}

NumberNumber*
NumberCurrencyStyle::FetchNumber()
{
	auto *p = (ods::inst::NumberNumber*) Get(ods::Id::NumberNumber);
	
	if (p == nullptr)
		p = NewNumber();
	
	return p;
}

void
NumberCurrencyStyle::Init(ods::Tag *tag)
{
	tag->Copy(ns_->style(), ods::ns::kName, style_name_);
	tag->Copy(ns_->style(), ods::ns::kVolatile, style_volatile_);
	
	Scan(tag);
}

inst::NumberCurrencySymbol*
NumberCurrencyStyle::NewCurrencySymbol()
{
	auto *p = new NumberCurrencySymbol(this);
	Append(p);
	return p;
}

inst::NumberNumber*
NumberCurrencyStyle::NewNumber()
{
	auto *p = new NumberNumber(this);
	Append(p);
	return p;
}

void
NumberCurrencyStyle::Scan(ods::Tag *tag)
{
	for (auto *x: tag->nodes())
	{
		if (AddText(x))
			continue;
		
		auto *next = x->as_tag();
		
		if (next->Is(ns_->number(), ods::ns::kCurrencySymbol)) {
			Append(new NumberCurrencySymbol(this, next));
		} else if (next->Is(ns_->number(), ods::ns::kNumber)) {
			Append(new NumberNumber(this, next));
		} else if (next->Is(ns_->number(), ods::ns::kText)) {
			Append(new NumberText(this, next));
		} else if (next->Is(ns_->style(), ods::ns::kMap)) {
			Append(new StyleMap(this, next));
		} else if (next->Is(ns_->style(), ods::ns::kTextProperties)) {
			Append(new StyleTextProperties(this, next));
		} else {
			Scan(next);
		}
	}
}

void
NumberCurrencyStyle::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->style(), ods::ns::kName, style_name_);
	Write(xml, ns_->style(), ods::ns::kVolatile, style_volatile_);
	
	WriteNodes(xml);
}

} // ods::inst::
} // ods::
