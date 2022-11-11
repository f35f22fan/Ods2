#include "NumberTextContent.hpp"

#include "../Ns.hpp"

#include "../ndff/Container.hpp"
#include "../ndff/Property.hpp"

namespace ods::inst {

NumberTextContent::NumberTextContent(Abstract *parent, Tag *tag, ndff::Container *cntr)
: Abstract(parent, parent->ns(), id::NumberTextContent)
{
	if (cntr)
		Init(cntr);
	else if (tag)
		Init(tag);
}

NumberTextContent::NumberTextContent(const NumberTextContent &cloner)
: Abstract(cloner)
{}

NumberTextContent::~NumberTextContent() {}

Abstract*
NumberTextContent::Clone(Abstract *parent) const
{
	auto *p = new NumberTextContent(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->CloneChildrenOf(this);
	
	return p;
}

void NumberTextContent::Init(ndff::Container *cntr)
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
//			if (prop.is(ns_->number()))
//			{
//				if (prop.name == ns::kTextContent)
//					Append(new NumberTextContent(this, 0, cntr), TakeOwnership::Yes);
//			}
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

void NumberTextContent::Init(ods::Tag *tag)
{
	ScanString(tag);
}

void NumberTextContent::ListKeywords(inst::Keywords &list, const inst::LimitTo lt)
{
	inst::AddKeywords({tag_name()}, list);
}

void NumberTextContent::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->number(), list);
}

void NumberTextContent::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
