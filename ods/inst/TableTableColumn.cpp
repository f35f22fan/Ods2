#include "TableTableColumn.hpp"

#include "StyleStyle.hpp"
#include "StyleTableColumnProperties.hpp"

#include "../Book.hpp"
#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods::inst {

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
	p->ncr_ = ncr_;
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

void TableTableColumn::Init(Tag *tag)
{
	tag->Copy(ns_->table(), ns::kStyleName, table_style_name_);
	tag->Copy(ns_->table(), ns::kNumberColumnsRepeated, ncr_);
	tag->Copy(ns_->table(), ns::kDefaultCellStyleName,
		table_default_cell_style_name_);
}

void TableTableColumn::ListKeywords(inst::Keywords &list, const inst::LimitTo lt)
{
	inst::AddKeywords({tag_name(),ns::kStyleName,
		ns::kNumberColumnsRepeated, ns::kDefaultCellStyleName}, list);
}

void TableTableColumn::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->table(), list);
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

void TableTableColumn::SetStyle(inst::StyleStyle *p)
{
	if (p != nullptr)
	{
		if (p->style_name())
			table_style_name_ = *p->style_name();
		else
			mtl_trace();
	} else {
		table_style_name_.clear();
	}
}

void TableTableColumn::SetWidth(Length *length)
{
	auto *tcp = FetchTableColumnProperties();
	tcp->SetColumnWidth(length);
}

QString
TableTableColumn::ToSchemaString() const
{
	QString s = QLatin1String("R ") + QString::number(ncr_);
	s = QChar('[') + s + QChar(']');
	
	if (selected()) {
		return QString(MTL_COLOR_RED) + s + QString(MTL_COLOR_DEFAULT);
	}
	
	return s;
}

void TableTableColumn::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->table(), ns::kStyleName, table_style_name_);
	Write(xml, ns_->table(), ns::kNumberColumnsRepeated, ncr_, 1);
	Write(xml, ns_->table(), ns::kDefaultCellStyleName,
		table_default_cell_style_name_);
}

void TableTableColumn::WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba)
{
	CHECK_TRUE_VOID(ba != nullptr);
	WriteTag(kw, *ba);
	WriteNdffProp(kw, *ba, ns_->table(), ns::kStyleName, table_style_name_);
	WriteNdffProp(kw, *ba, ns_->table(), ns::kNumberColumnsRepeated, ncr_, 1);
	WriteNdffProp(kw, *ba, ns_->table(), ns::kDefaultCellStyleName,
		table_default_cell_style_name_);
	CloseBasedOnChildren(h, kw, file, ba);
}

} // ods::inst::
