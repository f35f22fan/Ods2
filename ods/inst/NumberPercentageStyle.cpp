#include "NumberPercentageStyle.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

#include "NumberNumber.hpp"
#include "NumberText.hpp"

#include "../ndff/Container.hpp"
#include "../ndff/Property.hpp"

namespace ods::inst {

NumberPercentageStyle::NumberPercentageStyle(ods::inst::Abstract *parent,
	ods::Tag *tag, ndff::Container *cntr)
: Abstract(parent, parent->ns(), id::NumberPercentageStyle)
{
	if (cntr)
		Init(cntr);
	else if (tag)
		Init(tag);
}

NumberPercentageStyle::NumberPercentageStyle(const NumberPercentageStyle &cloner)
: Abstract(cloner)
{}

NumberPercentageStyle::~NumberPercentageStyle()
{}

Abstract*
NumberPercentageStyle::Clone(Abstract *parent) const
{
	auto *p = new NumberPercentageStyle(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->style_name_ = style_name_;
	p->CloneChildrenOf(this);
	
	return p;
}

inst::NumberNumber*
NumberPercentageStyle::FetchNumberStyle()
{
	auto *p = Get(Id::NumberNumber);
	
	if (p == nullptr)
		return NewNumberStyle();
	
	return (inst::NumberNumber*) p;
}

inst::NumberText*
NumberPercentageStyle::FetchNumberText()
{
	auto *p = Get(Id::NumberText);
	
	if (p == nullptr) 
		return NewNumberText();
		
	return (inst::NumberText*) p;
}

inst::NumberNumber*
NumberPercentageStyle::GetNumberStyle() const
{
	auto *p = Get(Id::NumberNumber);
	
	if (p == nullptr)
		return nullptr;
	
	return (inst::NumberNumber*) p;
}

inst::NumberText*
NumberPercentageStyle::GetNumberText() const
{
	auto *p = Get(Id::NumberText);
	
	if (p == nullptr)
		return nullptr;
	
	return (inst::NumberText*) p;
}

void NumberPercentageStyle::Init(ndff::Container *cntr)
{
	using Op = ndff::Op;
	ndff::Property prop;
	QHash<UriId, QVector<ndff::Property>> attrs;
	Op op = cntr->Next(prop, Op::TS, &attrs);
	CopyAttr(attrs, ns_->style(), ns::kName, style_name_);
	
	if (op == Op::N32_TE)
		return;
	
	if (op == Op::TCF_CMS)
		op = cntr->Next(prop, op);
	
	while (true)
	{
		if (op == Op::TS)
		{
			if (prop.is(ns_->number()))
			{
				if (prop.name == ns::kNumber)
					Append(new NumberNumber(this, 0, cntr), TakeOwnership::Yes);
				else if (prop.name == ns::kText)
					Append(new NumberText(this, 0, cntr), TakeOwnership::Yes);
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

void NumberPercentageStyle::Init(ods::Tag *tag)
{
	tag->Copy(ns_->style(), ods::ns::kName, style_name_);
	Scan(tag);
}

void NumberPercentageStyle::ListKeywords(inst::Keywords &list, const inst::LimitTo lt)
{
	inst::AddKeywords({tag_name(), ns::kName}, list);
}

void NumberPercentageStyle::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->number(), list);
	Add(ns_->style(), list);
}

inst::NumberNumber*
NumberPercentageStyle::NewNumberStyle()
{
	auto *p = new NumberNumber(this);
	Append(p, TakeOwnership::Yes);
	return p;
}

NumberText*
NumberPercentageStyle::NewNumberText()
{
	auto *nt = new NumberText(this);
	Append(nt, TakeOwnership::Yes);
	return nt;
}

void NumberPercentageStyle::Scan(ods::Tag *tag)
{
	foreach (auto *x, tag->nodes())
	{
		if (AddText(x))
			continue;
		
		auto *next = x->as_tag();
		
		if (next->Is(ns_->number(), ods::ns::kNumber))
		{
			Append(new NumberNumber(this, next), TakeOwnership::Yes);
		} else if (next->Is(ns_->number(), ns::kText)) {
			Append(new NumberText(this, next), TakeOwnership::Yes);
		} else {
			Scan(next);
		}
	}
}

void NumberPercentageStyle::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->style(), ods::ns::kName, style_name_);
	WriteNodes(xml);
}

void NumberPercentageStyle::WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba)
{
	MTL_CHECK_VOID(ba != nullptr);
	WriteTag(kw, *ba);
	WriteNdffProp(kw, *ba, ns_->style(), ods::ns::kName, style_name_);
	CloseBasedOnChildren(h, kw, file, ba);
}

} // ods::inst::
