#include "StyleFooter.hpp"

#include "TextP.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

#include "../ndff/Container.hpp"
#include "../ndff/Property.hpp"

namespace ods::inst {

StyleFooter::StyleFooter(Abstract *parent, Tag *tag, ndff::Container *cntr)
: Abstract(parent, parent->ns(), id::StyleFooter)
{
	if (cntr)
		Init(cntr);
	else if (tag)
		Init(tag);
}

StyleFooter::StyleFooter(const StyleFooter &cloner)
: Abstract(cloner)
{}

StyleFooter::~StyleFooter()
{}

Abstract*
StyleFooter::Clone(Abstract *parent) const
{
	auto *p = new StyleFooter(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->CloneChildrenOf(this);
	
	return p;
}

void StyleFooter::Init(ndff::Container *cntr)
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
			if (prop.is(ns_->text()))
			{
				if (prop.name == ns::kP)
					Append(new TextP(this, 0, cntr), TakeOwnership::Yes);
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

void StyleFooter::Init(ods::Tag *tag)
{
	Scan(tag);
}

void StyleFooter::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name()}, list);
}

void StyleFooter::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->style(), list);
}

void StyleFooter::Scan(ods::Tag *tag)
{
	for (auto *x: tag->nodes())
	{
		if (AddText(x))
			continue;
		
		auto *next = x->as_tag();
		
		if (next->Is(ns_->text(), ns::kP)) {
			Append(new TextP(this, next), TakeOwnership::Yes);
		} else {
			Scan(next);
		}
	}
}

void
StyleFooter::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
