#include "SvgDesc.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"
#include "../ndff/Container.hpp"
#include "../ndff/Property.hpp"

namespace ods::inst {

SvgDesc::SvgDesc(Abstract *parent, Tag * tag, ndff::Container *cntr)
: Abstract (parent, parent->ns(), id::SvgDesc)
{
	if (cntr)
		Init(cntr);
	else if (tag)
		Init(tag);
}

SvgDesc::SvgDesc(const SvgDesc &cloner)
: Abstract (cloner)
{}

SvgDesc::~SvgDesc() {}

Abstract*
SvgDesc::Clone(Abstract *parent) const
{
	auto *p = new SvgDesc(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	return p;
}

const QString*
SvgDesc::GetFirstString() const
{
	for (StringOrInst *node: nodes_)
	{
		if (node->is_string())
			return node->as_str_ptr();
	}
	
	return nullptr;
}

void SvgDesc::Init(ndff::Container *cntr)
{
	using Op = ndff::Op;
	ndff::Property prop;
	//QHash<UriId, QVector<ndff::Property>> attrs;
	Op op = cntr->Next(prop, Op::TS);
//	CopyAttr(attrs, ns_->xlink(), ns::kHref, xlink_href_);
//	CopyAttr(attrs, ns_->xlink(), ns::kType, xlink_type_);
//	CopyAttr(attrs, ns_->xlink(), ns::kShow, xlink_show_);
//	CopyAttr(attrs, ns_->xlink(), ns::kActuate, xlink_actuate_);
	
	if (op == Op::N32_TE)
		return;

	if (op == Op::TCF_CMS)
		op = cntr->Next(prop, op);

	while (true)
	{
		if (op == Op::TS)
		{
//			if (prop.is(ns_->table()))
//			{
//				if (prop.name == ns::kNamedRange)
//					Append(new inst::TableNamedRange(this, 0, cntr), TakeOwnership::Yes);
//			}
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

void SvgDesc::Init(ods::Tag *tag)
{
	ReadStrings(tag);
}

void SvgDesc::ListKeywords(inst::Keywords &list, const inst::LimitTo lt)
{
	inst::AddKeywords({tag_name()}, list);
}

void SvgDesc::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->svg(), list);
}

void SvgDesc::SetFirstString(const QString &s)
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

void SvgDesc::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
