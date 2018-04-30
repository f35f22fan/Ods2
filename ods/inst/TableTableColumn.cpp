#include "TableTableColumn.hpp"

#include "StyleStyle.hpp"
#include "StyleTableColumnProperties.hpp"

#include "../Book.hpp"
#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

TableTableColumn::TableTableColumn(Abstract *parent, Tag *tag)
: Abstract(parent, parent->ns(), id::TableTableColumn)
{
	if (tag != nullptr)
		Init(tag);
}

TableTableColumn::TableTableColumn(const TableTableColumn &cloner)
: Abstract(cloner)
{}

TableTableColumn::~TableTableColumn() {}

inst::Abstract*
TableTableColumn::Clone(Abstract *parent) const
{
	auto *p = new TableTableColumn(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->table_style_name_ = table_style_name_;
	p->table_number_columns_repeated_ = table_number_columns_repeated_;
	p->table_default_cell_style_name_ = table_default_cell_style_name_;
	
	return p;
}

inst::StyleTableColumnProperties*
TableTableColumn::FetchTableColumnProperties()
{
	auto *style = GetStyle();
	
	if (style == nullptr)
	{
		style = book_->NewColumnStyle();
		SetStyle(style);
	}
	
	inst::StyleTableColumnProperties *tcp = (inst::StyleTableColumnProperties*)
		style->Get(Id::StyleTableColumnProperties);
	
	return (tcp == nullptr) ? style->NewTableColumnProperties() : tcp;
}

StyleStyle*
TableTableColumn::GetDefaultCellStyle() const
{
	return (inst::StyleStyle*)Get(table_default_cell_style_name_);
}

inst::StyleStyle*
TableTableColumn::GetStyle() const
{
	return Get(table_style_name_);
}

void
TableTableColumn::Init(Tag *tag)
{
	tag->Copy(ns_->table(), ods::ns::kStyleName, table_style_name_);
	tag->Copy(ns_->table(), ods::ns::kNumberColumnsRepeated, table_number_columns_repeated_);
	tag->Copy(ns_->table(), ods::ns::kDefaultCellStyleName,
		table_default_cell_style_name_);
}

Length*
TableTableColumn::QueryColumnWidth() const
{
	auto *style = GetStyle();
	
	if (style == nullptr)
		return nullptr;
	
	inst::StyleTableColumnProperties *tcp = (inst::StyleTableColumnProperties*)
		style->Get(Id::StyleTableColumnProperties);
	
	return (tcp == nullptr) ? nullptr : tcp->column_width();
}

void
TableTableColumn::SetStyle(inst::StyleStyle *p)
{
	if (p != nullptr)
		table_style_name_ = *p->style_name();
	else
		table_style_name_.clear();
}

void
TableTableColumn::SetWidth(Length *length)
{
	auto *tcp = FetchTableColumnProperties();
	tcp->SetColumnWidth(length);
}

void
TableTableColumn::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->table(), ods::ns::kStyleName, table_style_name_);
	
	Write(xml, ns_->table(), ods::ns::kNumberColumnsRepeated,
		table_number_columns_repeated_, 1);
	
	Write(xml, ns_->table(), ods::ns::kDefaultCellStyleName,
		table_default_cell_style_name_);
}

} // ods::inst::
} // ods::
