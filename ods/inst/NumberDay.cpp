#include "NumberDay.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

#include "../ndff/Container.hpp"
#include "../ndff/Property.hpp"

namespace ods::inst {

NumberDay::NumberDay(ods::inst::Abstract *parent, ods::Tag *tag, ndff::Container *cntr)
 : Abstract(parent, parent->ns(), id::NumberDay)
{
	if (cntr)
		Init(cntr);
	else if (tag)
		Init(tag);
}

NumberDay::NumberDay(const NumberDay &cloner)
: Abstract(cloner)
{}

NumberDay::~NumberDay() {}

Abstract*
NumberDay::Clone(Abstract *parent) const
{
	auto *p = new NumberDay(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->number_style_ = number_style_;
	p->CloneChildrenOf(this, ClonePart::Text);
	
	return p;
}

void NumberDay::Init(ndff::Container *cntr)
{
	using Op = ndff::Op;
	ndff::Property prop;
	QHash<UriId, QVector<ndff::Property>> attrs;
	Op op = cntr->Next(prop, Op::TS, &attrs);
	CopyAttr(attrs, ns_->number(), ns::kStyle, number_style_);
	ReadStrings(cntr, op);
}

void NumberDay::Init(ods::Tag *tag)
{
	tag->Copy(ns_->number(), ns::kStyle, number_style_);
	ReadStrings(tag);
}

void NumberDay::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name(), ns::kStyle}, list);
}

void NumberDay::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->number(), list);
}

void NumberDay::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->number(), ns::kStyle, number_style_);
	
	WriteNodes(xml);
}

void NumberDay::WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba)
{
	MTL_CHECK_VOID(ba != nullptr);
	WriteTag(kw, *ba);
	WriteNdffProp(kw, *ba, ns_->number(), ns::kStyle, number_style_);
	CloseBasedOnChildren(h, kw, file, ba);
}

} // ods::inst::
