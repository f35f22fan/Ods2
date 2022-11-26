#include "TextPageNumber.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

#include "../ndff/Container.hpp"
#include "../ndff/Property.hpp"

namespace ods::inst {

TextPageNumber::TextPageNumber(Abstract *parent, ods::Tag *tag, ndff::Container *cntr)
: Abstract(parent, parent->ns(), id::TextPageNumber)
{
	if (cntr)
		Init(cntr);
	else if (tag)
		Init(tag);
}

TextPageNumber::TextPageNumber(const TextPageNumber &cloner)
: Abstract(cloner)
{}

TextPageNumber::~TextPageNumber() {}

Abstract*
TextPageNumber::Clone(Abstract *parent) const
{
	auto *p = new TextPageNumber(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->CloneChildrenOf(this);
	
	return p;
}

void TextPageNumber::Init(ndff::Container *cntr)
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

void TextPageNumber::Init(ods::Tag *tag)
{
	ReadStrings(tag);
}

void TextPageNumber::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name()}, list);
}

void TextPageNumber::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->text(), list);
}

void TextPageNumber::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
