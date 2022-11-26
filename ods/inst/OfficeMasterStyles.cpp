#include "OfficeMasterStyles.hpp"

#include "StyleMasterPage.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

#include "../ndff/Container.hpp"
#include "../ndff/Property.hpp"

namespace ods::inst {

OfficeMasterStyles::OfficeMasterStyles(Abstract *parent, Tag *tag, ndff::Container *cntr)
: Abstract(parent, parent->ns(), id::OfficeMasterStyles)
{
	if (cntr)
		Init(cntr);
	else if (tag)
		Init(tag);
}

OfficeMasterStyles::OfficeMasterStyles(const OfficeMasterStyles &cloner)
: Abstract(cloner)
{}

OfficeMasterStyles::~OfficeMasterStyles()
{}

Abstract*
OfficeMasterStyles::Clone(Abstract *parent) const
{
	auto *p = new OfficeMasterStyles(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->CloneChildrenOf(this);
	
	return p;
}

void OfficeMasterStyles::Init(ndff::Container *cntr)
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
				if (prop.name == ns::kMasterPage)
					Append(new StyleMasterPage(this, 0, cntr), TakeOwnership::Yes);
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

void OfficeMasterStyles::Init(Tag *tag)
{
	Scan(tag);
}

void OfficeMasterStyles::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name()}, list);
}

void OfficeMasterStyles::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->office(), list);
}

void OfficeMasterStyles::Scan(Tag *tag)
{
	for (auto *x: tag->nodes())
	{
		if (!x->is_tag())
			continue;
		
		auto *next = x->as_tag();
		
		if (next->Is(ns_->style(), ns::kMasterPage)) {
			Append(new StyleMasterPage(this, next), TakeOwnership::Yes);
		} else {
			Scan(next);
		}
	}
}

void OfficeMasterStyles::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
