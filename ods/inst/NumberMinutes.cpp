#include "NumberMinutes.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

#include "../ndff/Container.hpp"
#include "../ndff/Property.hpp"

namespace ods::inst {

NumberMinutes::NumberMinutes(Abstract *parent, Tag *tag, ndff::Container *cntr) :
Abstract(parent, parent->ns(), id::NumberMinutes)
{
	if (cntr)
		Init(cntr);
	else if (tag)
		Init(tag);
}

NumberMinutes::NumberMinutes(const NumberMinutes &cloner)
: Abstract(cloner)
{}

NumberMinutes::~NumberMinutes() {}

Abstract*
NumberMinutes::Clone(Abstract *parent) const
{
	auto *p = new NumberMinutes(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->number_style_ = number_style_;
	p->CloneChildrenOf(this, ClonePart::Text);
	
	return p;
}

void NumberMinutes::Init(ndff::Container *cntr)
{
	using Op = ndff::Op;
	ndff::Property prop;
	QHash<UriId, QVector<ndff::Property>> attrs;
	Op op = cntr->Next(prop, Op::TS, &attrs);
	CopyAttr(attrs, ns_->number(), ns::kStyle, number_style_);
	ReadStrings(cntr, op);
}

void NumberMinutes::Init(Tag *tag)
{
	tag->Copy(ns_->number(), ns::kStyle, number_style_);
	ReadStrings(tag);
}

void NumberMinutes::ListKeywords(Keywords &list, const LimitTo lt)
{
	AddKeywords({tag_name(), ns::kStyle}, list);
}

void NumberMinutes::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->number(), list);
}

void NumberMinutes::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->number(), ns::kStyle, number_style_);
	WriteNodes(xml);
}

void NumberMinutes::WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba)
{
	CHECK_TRUE_VOID(ba != nullptr);
	WriteTag(kw, *ba);
	WriteNdffProp(kw, *ba, ns_->number(), ns::kStyle, number_style_);
	CloseBasedOnChildren(h, kw, file, ba);
}

} // ods::inst::
