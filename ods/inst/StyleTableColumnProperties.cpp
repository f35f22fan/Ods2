#include "StyleTableColumnProperties.hpp"

#include "../Length.hpp"
#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

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
}

Abstract*
StyleTableColumnProperties::Clone(Abstract *parent) const
{
	auto *p = new StyleTableColumnProperties(*this);
	
	if (parent)
		p->parent(parent);
	
	p->fo_break_before_ = fo_break_before_;
	
	if (style_column_width_)
		p->style_column_width_ = style_column_width_->Clone();
	
	p->CloneChildrenOf(this);
	
	return p;
}

void StyleTableColumnProperties::Init(ndff::Container *cntr)
{
	ndff(true);
	using Op = ndff::Op;
	ndff::Property prop;
	QHash<UriId, QVector<ndff::Property>> attrs;
	Op op = cntr->Next(prop, Op::TS, &attrs);
	CopyAttr(attrs, ns_->fo(), ns::kBreakBefore, fo_break_before_);
	QString col_width;
	CopyAttr(attrs, ns_->style(), ns::kColumnWidth, col_width);
	style_column_width_ = Length::FromString(col_width);
	if (op == Op::N32_TE)
		return;
	
	if (op == Op::TCF_CMS)
		op = cntr->Next(prop, op);
	
	while (op == Op::TS)
	{
		mtl_tbd();
//		if (prop.is(ns_->style()))
//		{
//			if (prop.name == ns::kTableColumnProperties) {
//				Append(new StyleTableColumnProperties(this, 0, cntr), TakeOwnership::Yes);
//			}
//			mtl_info("Tag start: %s", qPrintable(prop.name));
			
//		}
		
		op = cntr->Next(prop, op);
	}
	
	if (op != Op::SCT)
		mtl_trace("op: %d", op);
}

void StyleTableColumnProperties::Init(ods::Tag *tag)
{
	tag->Copy(ns_->fo(), ns::kBreakBefore, fo_break_before_);
	
	QString str;
	tag->Copy(ns_->style(), ns::kColumnWidth, str);
	style_column_width_ = Length::FromString(str);
	
	ScanString(tag);
}

void StyleTableColumnProperties::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name(), ns::kBreakBefore, ns::kColumnWidth}, list);
}

void StyleTableColumnProperties::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->style(), list);
	
	if (!fo_break_before_.isEmpty())
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
	Write(xml, ns_->fo(), ns::kBreakBefore, fo_break_before_);
	
	if (style_column_width_)
	{
		Write(xml, ns_->style(), ns::kColumnWidth,
			style_column_width_->toString());
	}
	
	WriteNodes(xml);
}

void StyleTableColumnProperties::WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba)
{
	CHECK_TRUE_VOID(ba != nullptr);
	WriteTag(kw, *ba);
	WriteNdffProp(kw, *ba, ns_->fo(), ns::kBreakBefore, fo_break_before_);
	if (style_column_width_)
	{
		WriteNdffProp(kw, *ba, ns_->style(), ns::kColumnWidth,
			style_column_width_->toString());
	}
	
	CloseBasedOnChildren(h, kw, file, ba);
}

} // ods::inst::
