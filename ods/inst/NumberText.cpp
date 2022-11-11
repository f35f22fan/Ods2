#include "NumberText.hpp"

#include "../ndff/Container.hpp"
#include "../ndff/Property.hpp"

namespace ods::inst {

NumberText::NumberText(ods::inst::Abstract *parent, Tag *tag, ndff::Container *cntr)
: Abstract(parent, parent->ns(), id::NumberText)
{
	if (cntr)
		Init(cntr);
	else if (tag)
		Init(tag);
}

NumberText::NumberText(const NumberText &cloner)
: Abstract(cloner)
{}

NumberText::~NumberText() {}

Abstract*
NumberText::Clone(Abstract *parent) const
{
	auto *p = new NumberText(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->CloneChildrenOf(this);
	
	return p;
}

const QString*
NumberText::GetFirstString() const
{
	for (StringOrInst *node: nodes_)
	{
		if (node->is_string())
			return node->as_str_ptr(); //&node->as_string();
	}
	
	return nullptr;
}

void NumberText::Init(ndff::Container *cntr)
{
	// number:text(1.2) => no attrs, no child elems, has char data
	ndff(true);
	using Op = ndff::Op;
	ndff::Property prop;
	QHash<UriId, QVector<ndff::Property>> attrs;
	Op op = cntr->Next(prop, Op::TS, &attrs);
	if (op == Op::N32_TE)
		return;
	
	if (op == Op::TCF_CMS)
	{
		op = cntr->Next(prop, op);
	}
	
	if (ndff::is_text(op))
	{
		QString s = cntr->NextString();
		//mtl_printq2("String: ", s);
		Append(s);
		op = cntr->Next(prop, op);
		//mtl_info("Op is now: %d", (i32)op);
	}
	
	if (op != Op::SCT)
		mtl_trace("op: %d", op);
}

void NumberText::Init(Tag *tag)
{
	ScanString(tag);
}

void NumberText::ListKeywords(Keywords &list, const LimitTo lt)
{
	AddKeywords({tag_name()}, list);
}

void NumberText::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->number(), list);
}

void NumberText::SetFirstString(const QString &s)
{
	for (StringOrInst *node: nodes_)
	{
		if (node->is_string())
		{
			node->SetString(s);
			return;
		}
	}
	
	Append(s);
}

void NumberText::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
