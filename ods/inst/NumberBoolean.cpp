#include "NumberBoolean.hpp"

#include "../Ns.hpp"
#include "../Tag.hpp"

#include "../ndff/Property.hpp"
#include "../ndff/Container.hpp"

namespace ods::inst {

NumberBoolean::NumberBoolean(Abstract *parent, ods::Tag *tag,
	ndff::Container *cntr) :
Abstract(parent, parent->ns(), id::NumberBoolean)
{
	if (cntr)
		Init(cntr);
//	else if (tag)
//		Init(tag); // cannot have any params
}

NumberBoolean::NumberBoolean(const NumberBoolean &cloner)
: Abstract(cloner)
{}

NumberBoolean::~NumberBoolean() {}

Abstract*
NumberBoolean::Clone(Abstract *parent) const
{
	auto *p = new NumberBoolean(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	return p;
}

void NumberBoolean::Init(ndff::Container *cntr)
{
	ndff(true);
	using Op = ndff::Op;
	ndff::Property prop;
	QHash<UriId, QVector<ndff::Property>> attrs;
	Op op = cntr->Next(prop, Op::TS, &attrs);
	if (op == Op::N32_TE)
	{
		mtl_info("Op::TE");
		return;
	}
	
	if (op == Op::TCF_CMS)
	{
		mtl_info("Op::TCF");
		op = cntr->Next(prop, op);
	}
	
	while (op == Op::TS)
	{
//		if (prop.is(ns_->number()))
//		{
//			if (prop.name == ns::kBoolean)
//				Append(new NumberBoolean(this, 0, cntr), TakeOwnership::Yes);
//			else if (prop.name == ns::kText)
//				Append(new NumberText(this, 0, cntr), TakeOwnership::Yes);
//		}
		
		op = cntr->Next(prop, op);
	}
	
	if (op != Op::SCT)
		mtl_trace("op: %d", op);
}

void NumberBoolean::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name()}, list);
}

void NumberBoolean::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->number(), list);
}

void NumberBoolean::WriteData(QXmlStreamWriter &xml)
{
//	WriteNodes(xml); // cannot have any children
}

} // ods::inst::
