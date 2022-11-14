#include "NumberCurrencyStyle.hpp"

#include "NumberCurrencySymbol.hpp"
#include "NumberNumber.hpp"
#include "NumberText.hpp"
#include "StyleMap.hpp"
#include "StyleTextProperties.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

#include "../ndff/Container.hpp"
#include "../ndff/Property.hpp"

namespace ods::inst {

NumberCurrencyStyle::NumberCurrencyStyle(Abstract *parent, ods::Tag *tag,
	ndff::Container *cntr)
: Abstract(parent, parent->ns(), id::NumberCurrencyStyle)
{
	if (cntr)
		Init(cntr);
	else if (tag)
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
	p->CloneChildrenOf(this);

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

void NumberCurrencyStyle::Init(ndff::Container *cntr)
{
	using Op = ndff::Op;
	ndff::Property prop;
	QHash<UriId, QVector<ndff::Property>> attrs;
	Op op = cntr->Next(prop, Op::TS, &attrs);
	CopyAttr(attrs, ns_->style(), ns::kName, style_name_);
	CopyAttr(attrs, ns_->style(), ns::kVolatile, style_volatile_);
	
	if (op == Op::N32_TE)
		return;
	
	if (op == Op::TCF_CMS)
		op = cntr->Next(prop, op);
	
	while (true)
	{
		if (op == Op::TS)
		{
			if (prop.is(ns_->number()))
			{
				if (prop.name == ns::kCurrencySymbol)
					Append(new NumberCurrencySymbol(this, 0, cntr), TakeOwnership::Yes);
				else if (prop.name == ns::kNumber)
					Append(new NumberNumber(this, 0, cntr), TakeOwnership::Yes);
				else if (prop.name == ns::kText)
					Append(new NumberText(this, 0, cntr), TakeOwnership::Yes);
			} else if (prop.is(ns_->style())) {
				if (prop.name == ns::kMap)
					Append(new StyleMap(this, 0, cntr), TakeOwnership::Yes);
				else if (prop.name == ns::kTextProperties)
					Append(new StyleTextProperties(this, 0, cntr), TakeOwnership::Yes);
			}
		} else if (ndff::is_text(op)) {
			Append(cntr->NextString());
		} else {
			break;
		}
		
		op = cntr->Next(prop, op);
	}
	
	if (op != Op::SCT)
		mtl_trace("Unexpected op: %d", op);
}

void NumberCurrencyStyle::Init(ods::Tag *tag)
{
	tag->Copy(ns_->style(), ns::kName, style_name_);
	tag->Copy(ns_->style(), ns::kVolatile, style_volatile_);
	
	Scan(tag);
}

void NumberCurrencyStyle::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name(), ns::kName, ns::kVolatile}, list);
}

void NumberCurrencyStyle::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->number(), list);
	Add(ns_->style(), list);
}

inst::NumberCurrencySymbol*
NumberCurrencyStyle::NewCurrencySymbol()
{
	auto *p = new NumberCurrencySymbol(this);
	Append(p, TakeOwnership::Yes);
	return p;
}

inst::NumberNumber*
NumberCurrencyStyle::NewNumber()
{
	auto *p = new NumberNumber(this);
	Append(p, TakeOwnership::Yes);
	return p;
}

void NumberCurrencyStyle::Scan(ods::Tag *tag)
{
	for (auto *x: tag->nodes())
	{
		if (AddText(x))
			continue;
		
		auto *next = x->as_tag();
		
		if (next->Is(ns_->number(), ns::kCurrencySymbol)) {
			Append(new NumberCurrencySymbol(this, next), TakeOwnership::Yes);
		} else if (next->Is(ns_->number(), ns::kNumber)) {
			Append(new NumberNumber(this, next), TakeOwnership::Yes);
		} else if (next->Is(ns_->number(), ns::kText)) {
			Append(new NumberText(this, next), TakeOwnership::Yes);
		} else if (next->Is(ns_->style(), ns::kMap)) {
			Append(new StyleMap(this, next), TakeOwnership::Yes);
		} else if (next->Is(ns_->style(), ns::kTextProperties)) {
			Append(new StyleTextProperties(this, next), TakeOwnership::Yes);
		} else {
			Scan(next);
		}
	}
}

void NumberCurrencyStyle::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->style(), ns::kName, style_name_);
	Write(xml, ns_->style(), ns::kVolatile, style_volatile_);
	
	WriteNodes(xml);
}

void NumberCurrencyStyle::WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba)
{
	CHECK_TRUE_VOID(ba != nullptr);
	WriteTag(kw, *ba);
	WriteNdffProp(kw, *ba, ns_->style(), ns::kName, style_name_);
	WriteNdffProp(kw, *ba, ns_->style(), ns::kVolatile, style_volatile_);
	CloseBasedOnChildren(h, kw, file, ba);
}

} // ods::inst::
