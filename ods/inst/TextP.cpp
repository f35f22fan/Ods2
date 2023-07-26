#include "TextP.hpp"

#include "TextDate.hpp"
#include "TextPageCount.hpp"
#include "TextPageNumber.hpp"
#include "TextS.hpp"
#include "TextSheetName.hpp"
#include "TextTime.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

#include "../ndff/Container.hpp"
#include "../ndff/Property.hpp"

namespace ods::inst {

TextP::TextP(Abstract *parent, ods::Tag *tag, ndff::Container *cntr)
: Abstract(parent, parent->ns(), id::TextP)
{
	if (cntr)
		Init(cntr);
	else if (tag)
		Init(tag);
}

TextP::TextP(const TextP &cloner) : Abstract(cloner)
{}

TextP::~TextP()
{}

void TextP::AppendString(const QString &s)
{
	for (StringOrInst *node: nodes_)
	{
		if (node->is_string())
		{
			node->AppendString(s);
			return;
		}
	}
	
	Append(s);
}

Abstract*
TextP::Clone(Abstract *parent) const
{
	auto *p = new TextP(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	return p;
}

const QString*
TextP::GetFirstString() const
{
	for (StringOrInst *node: nodes_)
	{
		if (node->is_string())
			return node->as_str_ptr();
	}
	
	return nullptr;
}

void TextP::Init(ndff::Container *cntr)
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
				if (prop.name == ns::kPageNumber)
					Append(new inst::TextPageNumber(this, 0, cntr), TakeOwnership::Yes);
				else if (prop.name == ns::kS)
					Append(new inst::TextS(this, 0, cntr), TakeOwnership::Yes);
				else if (prop.name == ns::kSheetName)
					Append(new inst::TextSheetName(this, 0, cntr), TakeOwnership::Yes);
				else if (prop.name == ns::kPageCount)
					Append(new inst::TextPageCount(this, 0, cntr), TakeOwnership::Yes);
				else if (prop.name == ns::kTime)
					Append(new inst::TextTime(this, 0, cntr), TakeOwnership::Yes);
				else if (prop.name == ns::kDate)
					Append(new inst::TextDate(this, 0, cntr), TakeOwnership::Yes);
			}
		} else if (ndff::is_text(op)) {
			QString s = cntr->NextString();
			//mtl_printq(s);
			Append(s);
		} else {
			break;
		}
		op = cntr->Next(prop, op);
	}

	if (op != Op::SCT)
		mtl_trace("Unexpected op: %d", op);
}

void TextP::Init(ods::Tag *tag)
{
	Scan(tag);
}

void TextP::ListKeywords(Keywords &list, const LimitTo lt)
{
	AddKeywords({tag_name()}, list);
}

void TextP::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->text(), list);
}

void TextP::Scan(ods::Tag *tag)
{
	for (StringOrTag *node: tag->nodes())
	{
		if (AddText(node))
			continue;
		
		Tag *next = node->as_tag();
		
		if (next->Is(ns_->text(), ns::kPageNumber)) {
			Append(new TextPageNumber(this, next), TakeOwnership::Yes);
		} else if (next->Is(ns_->text(), ns::kS)) {
			Append(new TextS(this, next), TakeOwnership::Yes);
		} else if (next->Is(ns_->text(), ns::kSheetName)) {
			Append(new TextSheetName(this, next), TakeOwnership::Yes);
		} else if (next->Is(ns_->text(), ns::kPageCount)) {
			Append(new TextPageCount(this, next), TakeOwnership::Yes);
		} else if (next->Is(ns_->text(), ns::kTime)) {
			Append(new TextTime(this, next), TakeOwnership::Yes);
		} else if (next->Is(ns_->text(), ns::kDate)) {
			Append(new TextDate(this, next), TakeOwnership::Yes);
		} else {
			Scan(next);
		}
	}
}

void TextP::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

void TextP::WriteNDFF(NsHash &h, Keywords &kw, QFileDevice *file, ByteArray *ba)
{
	MTL_CHECK_VOID(ba != nullptr);
	WriteTag(kw, *ba);
	CloseWriteNodesAndClose(h, kw, file, ba);
}

} // ods::inst::

