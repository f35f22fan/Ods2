#include "OfficeStyles.hpp"

#include "StyleDefaultStyle.hpp"
#include "NumberCurrencyStyle.hpp"
#include "NumberNumberStyle.hpp"
#include "StyleStyle.hpp"

#include "../Book.hpp"
#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

#include "../ndff/Container.hpp"
#include "../ndff/Property.hpp"

namespace ods::inst {

OfficeStyles::OfficeStyles(ods::inst::Abstract *parent, ods::Tag *tag,
	ndff::Container *cntr)
: Abstract(parent, parent->ns(), id::OfficeStyles)
{
	if (cntr)
		Init(cntr);
	else if (tag)
		Init(tag);
}

OfficeStyles::OfficeStyles(const OfficeStyles &cloner)
: Abstract(cloner)
{}

OfficeStyles::~OfficeStyles()
{}

Abstract*
OfficeStyles::Clone(Abstract *parent) const
{
	auto *p = new OfficeStyles(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->CloneChildrenOf(this);
	
	return p;
}

void OfficeStyles::Init(ndff::Container *cntr)
{
	using Op = ndff::Op;
	ndff::Property prop;
	Op op = cntr->Next(prop, Op::TS);
	if (op == Op::N32_TE)
		return;
	
	if (op == Op::TCF_CMS)
		op = cntr->Next(prop, op);
	
	while (true)
	{
		if (op == Op::TS)
		{
			if (prop.is(ns_->style()))
			{
				if (prop.name == ns::kDefaultStyle)
					Append(new StyleDefaultStyle(this, 0, cntr), TakeOwnership::Yes);
				else if (prop.name == ns::kStyle)
					Append(new StyleStyle(this, 0, cntr), TakeOwnership::Yes);
			} else if (prop.is(ns_->number())) {
				if (prop.name == ns::kNumberStyle)
					Append(new NumberNumberStyle(this, 0, cntr), TakeOwnership::Yes);
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

void OfficeStyles::Init(ods::Tag *tag)
{
	Scan(tag);
}

void OfficeStyles::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name()}, list);
}

void OfficeStyles::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->office(), list);
}

NumberCurrencyStyle*
OfficeStyles::NewNumberCurrencyStyle()
{
	QString new_name = book_->GenUniqueStyleName(style::Family::None,
		id::NumberCurrencyStyle);
	auto *p = new NumberCurrencyStyle(this);
	p->style_name(new_name);
	Append(p, TakeOwnership::Yes);
	
	return p;
}

StyleStyle*
OfficeStyles::NewStyleStyle(const style::Family f)
{
	QString new_name = book_->GenUniqueStyleName(f);
	auto *p = new StyleStyle(this);
	p->style_name(new_name);
	p->SetFamily(f);
	Append(p, TakeOwnership::Yes);
	
	return p;
}

void
OfficeStyles::Scan(ods::Tag *tag)
{
	for (auto *x: tag->nodes())
	{
		if (!x->is_tag())
			continue;
		
		auto *next = x->as_tag();
		
		if (next->Is(ns_->style(), ns::kDefaultStyle))
		{
			Append(new StyleDefaultStyle(this, next), TakeOwnership::Yes);
		} else if (next->Is(ns_->number(), ns::kNumberStyle)) {
			Append(new NumberNumberStyle(this, next), TakeOwnership::Yes);
		} else if (next->Is(ns_->style(), ns::kStyle)) {
			Append(new StyleStyle(this, next), TakeOwnership::Yes);
		} else {
			Scan(next);
		}
	}
}

void
OfficeStyles::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
