#include "TextSheetName.hpp"

#include "../Ns.hpp"
#include "../Tag.hpp"

#include "../ndff/Container.hpp"
#include "../ndff/Property.hpp"

namespace ods::inst {

TextSheetName::TextSheetName(Abstract *parent, Tag *tag, ndff::Container *cntr)
: Abstract(parent, parent->ns(), id::TextSheetName)
{
	if (cntr)
		Init(cntr);
	else if (tag)
		Init(tag);
}

TextSheetName::TextSheetName(const TextSheetName &cloner)
: Abstract(cloner)
{}

TextSheetName::~TextSheetName() {}

Abstract*
TextSheetName::Clone(Abstract *parent) const
{
	auto *p = new TextSheetName(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->CloneChildrenOf(this);
	
	return p;
}

void TextSheetName::Init(ndff::Container *cntr)
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

void TextSheetName::Init(Tag *tag)
{
	ReadStrings(tag);
}

void TextSheetName::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name()}, list);
}

void TextSheetName::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->text(), list);
}

void TextSheetName::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
