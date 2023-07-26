#include "StyleTableRowProperties.hpp"

#include "../Length.hpp"
#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

#include "../ndff/Container.hpp"
#include "../ndff/Property.hpp"

namespace ods::inst {

StyleTableRowProperties::StyleTableRowProperties(Abstract *parent,
	ods::Tag *tag, ndff::Container *cntr)
: Abstract(parent, parent->ns(), id::StyleTableRowProperties)
{
	if (cntr)
		Init(cntr);
	else if (tag)
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

void StyleTableRowProperties::Init(ndff::Container *cntr)
{
	using Op = ndff::Op;
	ndff::Property prop;
	NdffAttrs attrs;
	Op op = cntr->Next(prop, Op::TS, &attrs);
	CopyAttr(attrs, ns_->fo(), ns::kBreakBefore, fo_break_before_);
	CopyAttr(attrs, ns_->style(), ns::kUseOptimalRowHeight, style_use_optimal_row_height_);
	CopyAttr(attrs, ns_->style(), ns::kRowHeight, &style_row_height_);
	ReadStrings(cntr, op);
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
	MTL_CHECK_VOID(ba);
	WriteTag(kw, *ba);
	WriteNdffProp(kw, *ba, ns_->fo(), ns::kBreakBefore, fo_break_before_);
	WriteNdffProp(kw, *ba, ns_->style(), ns::kUseOptimalRowHeight, style_use_optimal_row_height_);
	WriteNdffProp(kw, *ba, ns_->style(), ns::kRowHeight, style_row_height_);
	CloseBasedOnChildren(h, kw, file, ba);
}

} // ods::inst::
