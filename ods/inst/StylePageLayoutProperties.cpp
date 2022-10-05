#include "StylePageLayoutProperties.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods::inst {

StylePageLayoutProperties::StylePageLayoutProperties(Abstract *parent, Tag *tag)
: Abstract(parent, parent->ns(), id::StylePageLayoutProperties)
{
	if (tag != nullptr)
		Init(tag);
}

StylePageLayoutProperties::StylePageLayoutProperties(const StylePageLayoutProperties &cloner)
: Abstract(cloner)
{}

StylePageLayoutProperties::~StylePageLayoutProperties() {}

Abstract*
StylePageLayoutProperties::Clone(Abstract *parent) const
{
	auto *p = new StylePageLayoutProperties(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->style_writing_mode_ = style_writing_mode_;
	p->CloneChildrenOf(this);
	
	return p;
}

void StylePageLayoutProperties::Init(ods::Tag *tag)
{
	tag->Copy(ns_->style(), ns::kWritingMode, style_writing_mode_);
	ScanString(tag);
}

void StylePageLayoutProperties::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name(), ns::kWritingMode}, list);
}

void StylePageLayoutProperties::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->style(), list);
}

void StylePageLayoutProperties::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->style(), ns::kWritingMode, style_writing_mode_);
	WriteNodes(xml);
}

void StylePageLayoutProperties::WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba)
{
	CHECK_TRUE_VOID(ba != nullptr);
	WriteTag(kw, *ba);
	WriteNdffProp(kw, *ba, ns_->style(), ns::kWritingMode, style_writing_mode_);
	CloseBasedOnChildren(h, kw, file, ba);
}

} // ods::inst::
