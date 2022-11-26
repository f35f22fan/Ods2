#include "StyleTableRowProperties.hpp"

#include "../Length.hpp"
#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods::inst {

StyleTableRowProperties::StyleTableRowProperties(Abstract *parent, ods::Tag *tag)
: Abstract(parent, parent->ns(), id::StyleTableRowProperties)
{
	if (tag != nullptr)
		Init(tag);
}

StyleTableRowProperties::StyleTableRowProperties(const StyleTableRowProperties &cloner)
: Abstract(cloner)
{}

StyleTableRowProperties::~StyleTableRowProperties()
{
	delete style_row_height_;
	style_row_height_ = nullptr;
}

Abstract*
StyleTableRowProperties::Clone(Abstract *parent) const
{
	auto *p = new StyleTableRowProperties(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->fo_break_before_ = fo_break_before_;
	p->style_use_optimal_row_height_ = style_use_optimal_row_height_;
	p->style_row_height_ = style_row_height_;
	p->CloneChildrenOf(this);

	return p;
}

void StyleTableRowProperties::Init(ods::Tag *tag)
{
	tag->Copy(ns_->fo(), ns::kBreakBefore, fo_break_before_);
	tag->Copy(ns_->style(), ns::kUseOptimalRowHeight, style_use_optimal_row_height_);
	tag->Copy(ns_->style(), ns::kRowHeight, &style_row_height_);
	ReadStrings(tag);
}

void StyleTableRowProperties::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name(), ns::kBreakBefore, ns::kUseOptimalRowHeight,
		ns::kRowHeight}, list);
}

void StyleTableRowProperties::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->style(), list);
	
	if (!fo_break_before_.isEmpty())
	{
		Add(ns_->fo(), list);
	}
}

void StyleTableRowProperties::SetOptimal(Length *size)
{
	style_use_optimal_row_height_ = ods::Bool::True;
	style_row_height_ = size;
}

void StyleTableRowProperties::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->fo(), ns::kBreakBefore, fo_break_before_);
	Write(xml, ns_->style(), ns::kUseOptimalRowHeight, style_use_optimal_row_height_);
	Write(xml, ns_->style(), ns::kRowHeight, style_row_height_);
	WriteNodes(xml);
}

void StyleTableRowProperties::WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba)
{
	CHECK_TRUE_VOID(ba != nullptr);
	WriteTag(kw, *ba);
	WriteNdffProp(kw, *ba, ns_->fo(), ns::kBreakBefore, fo_break_before_);
	WriteNdffProp(kw, *ba, ns_->style(), ns::kUseOptimalRowHeight, style_use_optimal_row_height_);
	WriteNdffProp(kw, *ba, ns_->style(), ns::kRowHeight, style_row_height_);
	CloseBasedOnChildren(h, kw, file, ba);
}

} // ods::inst::
