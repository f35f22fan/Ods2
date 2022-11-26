#include "StyleDefaultStyle.hpp"

#include "StyleParagraphProperties.hpp"
#include "StyleTextProperties.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

#include "../ndff/Container.hpp"
#include "../ndff/Property.hpp"

namespace ods::inst {

StyleDefaultStyle::StyleDefaultStyle(ods::inst::Abstract *parent, Tag *tag, ndff::Container *cntr)
: Abstract(parent, parent->ns(), id::StyleDefaultStyle)
{
	if (cntr)
		Init(cntr);
	else if (tag)
		Init(tag);
}

StyleDefaultStyle::StyleDefaultStyle(const StyleDefaultStyle &cloner)
: Abstract(cloner)
{}

StyleDefaultStyle::~StyleDefaultStyle()
{}

Abstract*
StyleDefaultStyle::Clone(Abstract *parent) const
{
	auto *p = new StyleDefaultStyle(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->style_family_ = style_family_;
	p->CloneChildrenOf(this);
	
	return p;
}

void StyleDefaultStyle::Init(ndff::Container *cntr)
{
	using Op = ndff::Op;
	ndff::Property prop;
	NdffAttrs attrs;
	Op op = cntr->Next(prop, Op::TS, &attrs);
	CopyAttr(attrs, ns_->style(), ns::kFamily, style_family_);
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
				if (prop.name == ns::kParagraphProperties)
					Append(new inst::StyleParagraphProperties(this, 0, cntr), TakeOwnership::Yes);
				else if (prop.name == ns::kTextProperties)
					Append(new inst::StyleTextProperties(this, 0, cntr), TakeOwnership::Yes);
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

void StyleDefaultStyle::Init(ods::Tag *tag)
{
	tag->Copy(ns_->style(), ns::kFamily, style_family_);
	Scan(tag);
}

void StyleDefaultStyle::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name(), ns::kFamily}, list);
}

void StyleDefaultStyle::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->style(), list);
}

void StyleDefaultStyle::Scan(ods::Tag *tag)
{
	foreach (auto *x, tag->nodes())
	{
		if (AddText(x))
			continue;
		
		auto *next = x->as_tag();
		
		if (next->Is(ns_->style(), ns::kParagraphProperties))
		{
			Append(new StyleParagraphProperties(this, next), TakeOwnership::Yes);
		} else if (next->Is(ns_->style(), ns::kTextProperties)) {
			Append(new StyleTextProperties(this, next), TakeOwnership::Yes);
		} else {
			Scan(next);
		}
	}
}

void StyleDefaultStyle::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->style(), ns::kFamily, style_family_);
	WriteNodes(xml);
}

void StyleDefaultStyle::WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba)
{
	CHECK_TRUE_VOID(ba != nullptr);
	WriteTag(kw, *ba);
	WriteNdffProp(kw, *ba, ns_->style(), ns::kFamily, style_family_);
	CloseBasedOnChildren(h, kw, file, ba);
}

} // ods::inst::
