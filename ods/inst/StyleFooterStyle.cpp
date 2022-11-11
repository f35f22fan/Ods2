#include "StyleFooterStyle.hpp"

#include "StyleHeaderFooterProperties.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

#include "../ndff/Container.hpp"
#include "../ndff/Property.hpp"

namespace ods::inst {

StyleFooterStyle::StyleFooterStyle(Abstract *parent, Tag *tag, ndff::Container *cntr)
: Abstract(parent, parent->ns(), id::StyleFooterStyle)
{
	if (cntr)
		Init(cntr);
	else if (tag)
		Init(tag);
}

StyleFooterStyle::StyleFooterStyle(const StyleFooterStyle &cloner)
: Abstract(cloner)
{}

StyleFooterStyle::~StyleFooterStyle()
{}

Abstract*
StyleFooterStyle::Clone(Abstract *parent) const
{
	auto *p = new StyleFooterStyle(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->CloneChildrenOf(this);
	
	return p;
}

void StyleFooterStyle::Init(ndff::Container *cntr)
{
	ndff(true);
	using Op = ndff::Op;
	ndff::Property prop;
	QHash<UriId, QVector<ndff::Property>> attrs;
	Op op = cntr->Next(prop, Op::TS, &attrs);
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
				if (prop.name == ns::kHeaderFooterProperties)
					Append(new StyleHeaderFooterProperties(this, 0, cntr), TakeOwnership::Yes);
			}
		} else if (ndff::is_text(op)) {
			Append(cntr->NextString());
		} else {
			break;
		}
		op = cntr->Next(prop, op);
	}
	
	if (op != Op::SCT)
		mtl_trace("op: %d", op);
}

void StyleFooterStyle::Init(ods::Tag *tag)
{
	Scan(tag);
}

void StyleFooterStyle::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name()}, list);
}

void StyleFooterStyle::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->style(), list);
}

void StyleFooterStyle::Scan(ods::Tag *tag)
{
	foreach (auto *x, tag->nodes())
	{
		if (AddText(x))
			continue;
		
		auto *next = x->as_tag();
		
		if (next->Is(ns_->style(), ns::kHeaderFooterProperties)) {
			Append(new StyleHeaderFooterProperties(this, next), TakeOwnership::Yes);
		} else {
			Scan(next);
		}
	}
}

void StyleFooterStyle::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::

