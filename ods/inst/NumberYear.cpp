#include "NumberYear.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

#include "../ndff/Container.hpp"
#include "../ndff/Property.hpp"

namespace ods::inst {

NumberYear::NumberYear(Abstract *parent, ods::Tag *tag, ndff::Container *cntr)
: Abstract(parent, parent->ns(), id::NumberYear)
{
	if (cntr)
		Init(cntr);
	else if (tag)
		Init(tag);
}

NumberYear::NumberYear(const NumberYear &cloner)
: Abstract(cloner)
{}

NumberYear::~NumberYear() {}

Abstract*
NumberYear::Clone(Abstract *parent) const
{
	auto *p = new NumberYear(*this);
	
	if (parent)
		p->parent(parent);
	
	p->number_style_ = number_style_;
	p->CloneChildrenOf(this, ClonePart::Text);
	
	return p;
}

void NumberYear::Init(ndff::Container *cntr)
{
	using Op = ndff::Op;
	ndff::Property prop;
	QHash<UriId, QVector<ndff::Property>> attrs;
	Op op = cntr->Next(prop, Op::TS, &attrs);
	CopyAttr(attrs, ns_->number(), ns::kStyle, number_style_);
	ReadStrings(cntr, op);
}

void NumberYear::Init(ods::Tag *tag)
{
	tag->Copy(ns_->number(), ns::kStyle, number_style_);
	ReadStrings(tag);
}

void NumberYear::ListKeywords(inst::Keywords &list, const inst::LimitTo lt)
{
	inst::AddKeywords({tag_name(), ns::kStyle}, list);
}

void NumberYear::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->number(), list);
}

void NumberYear::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->number(), ods::ns::kStyle, number_style_);
	WriteNodes(xml);
}

void NumberYear::WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba)
{
	CHECK_TRUE_VOID(ba != nullptr);
	WriteTag(kw, *ba);
	WriteNdffProp(kw, *ba, ns_->number(), ods::ns::kStyle, number_style_);
	CloseBasedOnChildren(h, kw, file, ba);
}

} // ods::inst::
