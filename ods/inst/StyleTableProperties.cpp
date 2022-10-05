#include "StyleTableProperties.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods::inst {

StyleTableProperties::StyleTableProperties(Abstract *parent, Tag *tag)
: Abstract(parent, parent->ns(), id::StyleTableProperties)
{
	if (tag != nullptr)
		Init(tag);
}

StyleTableProperties::StyleTableProperties(const StyleTableProperties &cloner)
: Abstract(cloner)
{}

StyleTableProperties::~StyleTableProperties() {}

Abstract*
StyleTableProperties::Clone(Abstract *parent) const
{
	auto *p = new StyleTableProperties(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->table_display_ = table_display_;
	p->style_writing_mode_ = style_writing_mode_;
	
	return p;
}

void StyleTableProperties::Init(Tag *tag)
{
	tag->Copy(ns_->table(), ns::kDisplay, table_display_);
	tag->Copy(ns_->style(), ns::kWritingMode, style_writing_mode_);
}

void StyleTableProperties::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name(), ns::kDisplay, ns::kWritingMode}, list);
}

void StyleTableProperties::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->style(), list);
	
	if (!table_display_.isEmpty())
		Add(ns_->table(), list);
}

void StyleTableProperties::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->table(), ns::kDisplay, table_display_);
	Write(xml, ns_->style(), ns::kWritingMode, style_writing_mode_);
}

void StyleTableProperties::WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba)
{
	CHECK_TRUE_VOID(ba != nullptr);
	WriteTag(kw, *ba);
	WriteNdffProp(kw, *ba, ns_->table(), ns::kDisplay, table_display_);
	WriteNdffProp(kw, *ba, ns_->style(), ns::kWritingMode, style_writing_mode_);
	CloseBasedOnChildren(h, kw, file, ba);
}

} // ods::inst::
