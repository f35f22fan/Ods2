#include "TableNamedRange.hpp"

#include "OfficeSpreadsheet.hpp"
#include "TableNamedExpressions.hpp"
#include "../Reference.hpp"
#include "../Book.hpp"
#include "../Ns.hpp"
#include "../ns.hxx"
#include "../ods.hh"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

TableNamedRange::TableNamedRange(Abstract *parent, Tag *tag)
: Abstract(parent, parent->ns(), id::TableNamedRange)
{
	if (tag != nullptr)
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
	
	return p;
}

ods::Reference*
TableNamedRange::GetReference()
{
	if (bits_ & TriedLoadingAddressBit)
		return reference_;
	
	bits_ |= TriedLoadingAddressBit;
	
	ods::Sheet *default_sheet = GetSheet();
	CHECK_PTR_NULL(default_sheet);
	
	reference_ = Reference::From(table_cell_range_address_.midRef(0), default_sheet);
	return reference_;
}

ods::Sheet*
TableNamedRange::GetSheet()
{
	if (sheet_ != nullptr)
		return sheet_;
	
	OfficeSpreadsheet *spreadsheet = book_->spreadsheet();
	CHECK_PTR_NULL(spreadsheet);
	
	QStringRef table_name;
	QStringRef address = table_base_cell_address_.midRef(0);
	CHECK_TRUE_NULL(ods::ParseTableName(address, table_name));
	sheet_ = spreadsheet->GetSheet(table_name);
	CHECK_PTR_NULL(sheet_); // to warn if sheet not found
	return sheet_;
}

void
TableNamedRange::Init(Tag *tag)
{
	tag->Copy(ns_->table(), ods::ns::kName, name_);
	tag->Copy(ns_->table(), ods::ns::kBaseCellAddress, table_base_cell_address_);
	tag->Copy(ns_->table(), ods::ns::kCellRangeAddress, table_cell_range_address_);
}

void
TableNamedRange::InitDefault()
{}

void
TableNamedRange::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->table(), ods::ns::kName, name_);
	Write(xml, ns_->table(), ods::ns::kBaseCellAddress, table_base_cell_address_);
	Write(xml, ns_->table(), ods::ns::kCellRangeAddress, table_cell_range_address_);
	WriteNodes(xml);
}

} // ods::inst::
} // ods::
 
