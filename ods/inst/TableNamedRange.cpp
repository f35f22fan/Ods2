#include "TableNamedRange.hpp"

#include "OfficeSpreadsheet.hpp"
#include "TableNamedExpressions.hpp"
#include "../Reference.hpp"
#include "../Book.hpp"
#include "../Ns.hpp"
#include "../ns.hxx"
#include "../ods.hh"
#include "../Tag.hpp"

namespace ods::inst {

TableNamedRange::TableNamedRange(Abstract *parent, Tag *tag)
: Abstract(parent, parent->ns(), id::TableNamedRange)
{
	if (tag)
		Init(tag);
	else
		InitDefault();
}

TableNamedRange::TableNamedRange(const TableNamedRange &cloner)
: Abstract(cloner)
{
	delete reference_;
	reference_ = nullptr;
}

TableNamedRange::~TableNamedRange() {}

Abstract*
TableNamedRange::Clone(Abstract *parent) const
{
	auto *p = new TableNamedRange(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->CloneChildrenOf(this);
	
	return p;
}

ods::Reference*
TableNamedRange::GetReference()
{
	if (bits_ & TriedLoadingAddressBit)
		return reference_;
	
	bits_ |= TriedLoadingAddressBit;
	
	ods::Sheet *default_sheet = GetSheet();
	MTL_CHECK_NULL(default_sheet);
	
	reference_ = Reference::From(table_cell_range_address_, default_sheet);
	return reference_;
}

ods::Sheet*
TableNamedRange::GetSheet()
{
	if (sheet_ != nullptr)
		return sheet_;
	
	OfficeSpreadsheet *spreadsheet = book_->spreadsheet();
	MTL_CHECK_NULL(spreadsheet);
	
	QStringView table_name;
	QStringView address = table_base_cell_address_;
	MTL_CHECK_NULL(ods::ParseTableName(address, table_name));
	sheet_ = spreadsheet->GetSheet(table_name);
	MTL_CHECK_NULL(sheet_); // to warn if sheet not found
	return sheet_;
}

void TableNamedRange::Init(Tag *tag)
{
	tag->Copy(ns_->table(), ns::kName, name_);
	tag->Copy(ns_->table(), ns::kBaseCellAddress, table_base_cell_address_);
	tag->Copy(ns_->table(), ns::kCellRangeAddress, table_cell_range_address_);
}

void TableNamedRange::InitDefault()
{}

void TableNamedRange::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name(), ns::kName, ns::kBaseCellAddress,
		ns::kCellRangeAddress}, list);
}

void TableNamedRange::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->table(), list);
}

void TableNamedRange::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->table(), ns::kName, name_);
	Write(xml, ns_->table(), ns::kBaseCellAddress, table_base_cell_address_);
	Write(xml, ns_->table(), ns::kCellRangeAddress, table_cell_range_address_);
	WriteNodes(xml);
}

} // ods::inst::
 
