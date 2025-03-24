#include "StyleTableColumnProperties.hpp"

#include "../Length.hpp"
#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"
#include "../attr/VisualBreak.hpp"

#include "../ndff/Container.hpp"
#include "../ndff/Property.hpp"

namespace ods::inst {

StyleTableColumnProperties::StyleTableColumnProperties(Abstract *parent,
ods::Tag *tag, ndff::Container *cntr) : Abstract(parent, parent->ns(), ods::id::StyleTableColumnProperties)
{
	if (cntr)
		Init(cntr);
	else if (tag)
		Init(tag);
}

StyleTableColumnProperties::StyleTableColumnProperties(const StyleTableColumnProperties &cloner)
: Abstract(cloner)
{}

StyleTableColumnProperties::~StyleTableColumnProperties()
{
	delete style_column_width_;
	style_column_width_ = nullptr;
	
	delete visual_break_;
	visual_break_ = nullptr;
}

Abstract*
StyleTableColumnProperties::Clone(Abstract *parent) const
{
	auto *p = new StyleTableColumnProperties(*this);
	
	if (parent)
		p->parent(parent);
	
	if (visual_break_) {
		p->visual_break_ = visual_break_->Clone();
	}
	
	if (style_column_width_)
		p->style_column_width_ = style_column_width_->Clone();
	
	p->CloneChildrenOf(this);
	
	return p;
}

void StyleTableColumnProperties::Init(ndff::Container *cntr)
{
	using Op = ndff::Op;
	ndff::Property prop;
	QHash<UriId, QVector<ndff::Property>> attrs;
	Op op = cntr->Next(prop, Op::TS, &attrs);
	
	QString input_str;
	{
		CopyAttr(attrs, ns_->fo(), ns::kBreakBefore, input_str);
		visual_break_ = attr::VisualBreak::FromString(input_str);
		
		if (!visual_break_) {
			input_str.clear();
			CopyAttr(attrs, ns_->fo(), ns::kBreakAfter, input_str);
			visual_break_ = attr::VisualBreak::FromString(input_str);
		}
		input_str.clear();
	}
	
	CopyAttr(attrs, ns_->style(), ns::kColumnWidth, input_str);
	style_column_width_ = Length::FromString(input_str);
	ReadStrings(cntr, op);
}

void StyleTableColumnProperties::Init(ods::Tag *tag)
{
	QString input_str;
	{
		tag->Copy(ns_->fo(), ns::kBreakBefore, input_str);
		visual_break_ = attr::VisualBreak::FromString(input_str);
		
		if (!visual_break_) {
			tag->Copy(ns_->fo(), ns::kBreakAfter, input_str);
			visual_break_ = attr::VisualBreak::FromString(input_str);
		}
		input_str.clear();
	}
	
	tag->Copy(ns_->style(), ns::kColumnWidth, input_str);
	style_column_width_ = Length::FromString(input_str);
	
	ReadStrings(tag);
}

void StyleTableColumnProperties::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name(), ns::kBreakBefore, ns::kBreakAfter, ns::kColumnWidth}, list);
}

void StyleTableColumnProperties::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->style(), list);
	
	if (visual_break_)
	{
		Add(ns_->fo(), list);
	}
}

void StyleTableColumnProperties::SetColumnWidth(Length *length)
{
	delete style_column_width_;

	if (length == nullptr)
		style_column_width_ = nullptr;
	else
		style_column_width_ = length->Clone();
}

void
StyleTableColumnProperties::WriteData(QXmlStreamWriter &xml)
{
	if (visual_break_) {
		cauto name = visual_break_->before() ? ns::kBreakBefore : ns::kBreakAfter;
		Write(xml, ns_->fo(), name, visual_break_->toString());
	}
	
	if (style_column_width_)
	{
		Write(xml, ns_->style(), ns::kColumnWidth,
			style_column_width_->toString());
	}
	
	WriteNodes(xml);
}

void StyleTableColumnProperties::WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba)
{
	MTL_CHECK_VOID(ba != nullptr);
	WriteTag(kw, *ba);
	
	if (visual_break_) {
		cauto name = visual_break_->before() ? ns::kBreakBefore : ns::kBreakAfter;
		WriteNdffProp(kw, *ba, ns_->fo(), name, visual_break_->toString());
	}
	
	if (style_column_width_)
	{
		WriteNdffProp(kw, *ba, ns_->style(), ns::kColumnWidth,
			style_column_width_->toString());
	}
	
	CloseBasedOnChildren(h, kw, file, ba);
}

} // ods::inst::
