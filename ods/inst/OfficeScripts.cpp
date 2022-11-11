#include "OfficeScripts.hpp"

#include "../ndff/Container.hpp"
#include "../ndff/Property.hpp"
#include "../Ns.hpp"

namespace ods::inst {

OfficeScripts::OfficeScripts(Abstract *parent, Tag *tag,
	ndff::Container *cntr)
: Abstract(parent, parent->ns(), id::OfficeScripts)
{
	if (cntr)
		Init(cntr);
	else if (tag)
		Init(tag);
	else
		InitDefault();
}

OfficeScripts::OfficeScripts(const OfficeScripts &cloner)
: Abstract(cloner)
{}

OfficeScripts::~OfficeScripts() {}

Abstract*
OfficeScripts::Clone(Abstract *parent) const
{
	auto *p = new OfficeScripts(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->CloneChildrenOf(this);
	
	return p;
}

void OfficeScripts::Init(ndff::Container *cntr)
{
	ndff(true);
	using Op = ndff::Op;
	ndff::Property prop;
	QHash<UriId, QVector<ndff::Property>> h;
	Op op = cntr->Next(prop, Op::TS, &h);
	if (op == Op::N32_TE)
		return;
	
	if (op == Op::TCF_CMS)
		op = cntr->Next(prop, op);
	
	while (op == Op::TS)
	{
		if (prop.is(ns_->office()))
		{
			mtl_info("Tag start: %s", qPrintable(prop.name));
		}
		
		op = cntr->Next(prop, op);
	}
	
	if (op != Op::SCT)
		mtl_trace("op: %d", op);
}

void OfficeScripts::Init(Tag *tag)
{
	ScanString(tag);
}

void OfficeScripts::InitDefault() {}

void OfficeScripts::ListKeywords(inst::Keywords &list, const inst::LimitTo lt)
{
	inst::AddKeywords({tag_name()}, list);
}

void OfficeScripts::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->office(), list);
}

void OfficeScripts::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
