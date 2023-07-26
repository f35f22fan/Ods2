#include "NumberMonth.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

#include "../ndff/Container.hpp"
#include "../ndff/Property.hpp"

namespace ods::inst {

NumberMonth::NumberMonth(Abstract *parent, Tag *tag, ndff::Container *cntr) :
Abstract(parent, parent->ns(), id::NumberMonth)
{
	if (cntr)
		Init(cntr);
	else if (tag)
		Init(tag);
}

NumberMonth::NumberMonth(const NumberMonth &cloner)
: Abstract(cloner)
{}

NumberMonth::~NumberMonth() {}

Abstract*
NumberMonth::Clone(Abstract *parent) const
{
	auto *p = new NumberMonth(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->number_style_ = number_style_;
	p->CloneChildrenOf(this, ClonePart::Text);
	
	return p;
}

void NumberMonth::Init(ndff::Container *cntr)
{
	using Op = ndff::Op;
	ndff::Property prop;
	QHash<UriId, QVector<ndff::Property>> attrs;
	Op op = cntr->Next(prop, Op::TS, &attrs);
	CopyAttr(attrs, ns_->number(), ns::kStyle, number_style_);
	ReadStrings(cntr, op);
}

void NumberMonth::Init(ods::Tag *tag)
{
	tag->Copy(ns_->number(), ns::kStyle, number_style_);
	ReadStrings(tag);
}

void NumberMonth::ListKeywords(Keywords &list, const LimitTo lt)
{
	AddKeywords({tag_name(), ns::kStyle}, list);
}

void NumberMonth::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->number(), list);
}

void NumberMonth::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->number(), ods::ns::kStyle, number_style_);
	WriteNodes(xml);
}

void NumberMonth::WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba)
{
	MTL_CHECK_VOID(ba != nullptr);
	WriteTag(kw, *ba);
	WriteNdffProp(kw, *ba, ns_->number(), ns::kStyle, number_style_);
	CloseBasedOnChildren(h, kw, file, ba);
}

} // ods::inst::
