#include "Sheet.hpp"

#include "Reference.hpp"
#include "Book.hpp"
#include "Cell.hpp"
#include "Ns.hpp"
#include "ns.hxx"
#include "Row.hpp"
#include "Tag.hpp"

#include "inst/OfficeAutomaticStyles.hpp"
#include "inst/OfficeDocumentContent.hpp"
#include "inst/OfficeSpreadsheet.hpp"
#include "inst/TableTableColumn.hpp"
#include "inst/TableNamedExpressions.hpp"

#include "ndff/Container.hpp"
#include "ndff/Property.hpp"

namespace ods {

Sheet::Sheet(ods::inst::Abstract *parent, Tag *tag, ndff::Container *cntr) :
	Abstract(parent, parent->ns(), id::TableTable)
{
	if (cntr)
		Init(cntr);
	else if (tag)
		Init(tag);
	CountColumns();
}

Sheet::Sheet(ods::inst::Abstract *parent) :
	Abstract(parent, parent->ns(), id::TableTable)
{
	InitDefault();
}

Sheet::Sheet(const Sheet &cloner)
: Abstract(cloner)
{}

Sheet::~Sheet()
{
	for (auto *next: rows_)
		delete next;
	
	for (auto *next: columns_)
		delete next;
}

Row*
Sheet::RowAt(cint place, int &vec_index)
{
	int so_far = 0;
	cint count = rows_.size();
	
	for (int i = 0; i < count; i++) {
		ods::Row *row = rows_[i];
		so_far += row->num();
		
		if (so_far > place) {
			vec_index = i;
			return row;
		}
	}
	
	return nullptr;
}

inst::Abstract*
Sheet::Clone(inst::Abstract *parent) const
{
	auto *p = new Sheet(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->table_name_ = table_name_;
	p->table_style_name_ = table_style_name_;
	p->num_cols_ = num_cols_;
	
	for (auto *next: rows_)
	{
		Row *row = (Row*)next->Clone(p);
		p->rows_.append(row);
	}
	
	for (auto *next: columns_)
	{
		auto *col = (inst::TableTableColumn*)next->Clone(p);
		p->columns_.append(col);
	}
	
	return p;
}

inst::TableTableColumn*
Sheet::ColumnAt(cint place, int &vec_index)
{
	int so_far = 0;
	cint count = columns_.size();
	
	for (int i = 0; i < count; i++) {
		inst::TableTableColumn *col = columns_[i];
		so_far += col->num();
		
		if (so_far > place) {
			vec_index = i;
			return col;
		}
	}
	
	return nullptr;
}

i32 Sheet::CountColumns()
{
	num_cols_ = 0;
	
	for (inst::TableTableColumn *col: columns_)
		num_cols_ += col->number_columns_repeated();
	
	return num_cols_;
}

i32 Sheet::CountRows() const
{
	int n = 0;
	
	for (Row *row: rows_)
		n += row->num();
	
	return n;
}

void Sheet::DeleteColumnRegion(inst::TableTableColumn *col, cint vec_index)
{
	const ods::DeleteRegion &dr = col->delete_region();
	
	if (dr.count == col->num()) {
		delete col;
		columns_.remove(dr.vec_index);
		return;
	}
	
	cint num = col->num();
	cint region_len = dr.start + dr.count;
	
	if (dr.start > 0 && region_len < num) {
		auto *col2 = static_cast<inst::TableTableColumn*>(col->Clone());
		col2->num(dr.start);
		columns_.insert(vec_index, col2);
		col->num(num - region_len);
	} else {
		col->num(num - dr.count);
	}
	
	col->delete_region({-1, -1, -1});
}

void Sheet::DeleteRowRegion(ods::Row *row, cint vec_index)
{
	const ods::DeleteRegion &dr = row->delete_region();
	
	if (dr.count == row->num()) {
		delete row;
		rows_.remove(dr.vec_index);
		return;
	}
	
	cint num = row->num();
	cint region_len = dr.start + dr.count;
	
	if (dr.start > 0 && region_len < num) {
		Row *row2 = static_cast<Row*>(row->Clone());
		row2->num(dr.start);
		rows_.insert(vec_index, row2);
		row->num(num - region_len);
	} else {
		row->num(num - dr.count);
	}
	
	row->delete_region({-1, -1, -1});
}

inst::TableTableColumn*
Sheet::GetColumn(cint place) const
{
	int next_col = 0;
	
	for (auto *next: columns_)
	{
		next_col += next->num();
		
		if (next_col > place)
			return next;
	}
	
	return nullptr;
}

inst::StyleStyle*
Sheet::GetDefaultCellStyle(const ods::Cell *cell) const
{
	auto *column = GetColumn(cell->QueryStart());
	
	if (column == nullptr)
	{
		mtl_warn();
		return nullptr;
	}
	
	return column->GetDefaultCellStyle();
}

ods::Row*
Sheet::GetRow(cint place)
{
	MTL_CHECK_NULL((place >= 0));
	int at = -1;
	
	for (ods::Row *r: rows_)
	{
		at += r->num();
		
		if (at >= place)
			return r;
	}
	
	return nullptr;
}

void Sheet::Init(ndff::Container *cntr)
{
	using Op = ndff::Op;
	ndff::Property prop;
	QHash<UriId, QVector<ndff::Property>> attrs;
	Op op = cntr->Next(prop, Op::TS, &attrs);
	CopyAttr(attrs, ns_->table(), ns::kName, table_name_);
	CopyAttr(attrs, ns_->table(), ns::kStyleName, table_style_name_);

	if (op == Op::N32_TE)
		return;

	if (op == Op::TCF_CMS)
		op = cntr->Next(prop, op);

	while (true)
	{
		if (op == Op::TS)
		{
			if (prop.is(ns_->table()))
			{
				if (prop.name == ns::kTableRow)
					rows_.append(new ods::Row(this, 0, cntr));
				else if (prop.name == ns::kTableColumn)
					columns_.append(new inst::TableTableColumn(this, 0, cntr));
				else if (prop.name == ns::kNamedExpressions)
					named_expressions_ = new inst::TableNamedExpressions(this, 0, cntr);
			}
		} else if (ndff::is_text(op)) {
			Append(cntr->NextString());
		} else {
			break;
		}
		op = cntr->Next(prop, op);
	}

	if (op != Op::SCT)
		mtl_trace("Unexpected op: %d", op);
}

void Sheet::Init(ods::Tag *tag)
{
	tag->Copy(ns_->table(), ns::kName, table_name_);
	tag->Copy(ns_->table(), ns::kStyleName, table_style_name_);
	Scan(tag);
}

void Sheet::InitDefault()
{
	auto *column = new inst::TableTableColumn(this);
	num_cols_ = DefaultColumnCountPerSheet;
	column->number_columns_repeated(num_cols_);
	columns_.append(column);
	
	ods::Tag *tag = nullptr;
	auto *row = new ods::Row(this, tag, 0);
	row->num(DefaultRowCountPerSheet);
	rows_.append(row);
}

void Sheet::ListChildren(QVector<StringOrInst*> &vec,
	const Recursively r)
{
	for (auto *next: columns_)
	{
		vec.append(new StringOrInst(next, TakeOwnership::No));
		if (r == Recursively::Yes)
			next->ListChildren(vec, r);
	}
	
	for (auto *next: rows_)
	{
		vec.append(new StringOrInst(next, TakeOwnership::No));
		if (r == Recursively::Yes)
			next->ListChildren(vec, r);
	}
	
	if (named_expressions_)
	{
		vec.append(new StringOrInst(named_expressions_, TakeOwnership::No));
		
		if (r == Recursively::Yes)
			named_expressions_->ListChildren(vec, r);
	}
}

void Sheet::ListKeywords(inst::Keywords &list, const inst::LimitTo lt)
{
	inst::AddKeywords({tag_name(),
		ns::kName, ns::kStyleName}, list);
}

void Sheet::ListUsedNamespaces(inst::NsHash &list)
{
	inst::Add(ns_->table(), list);
}

void Sheet::MarkColumnDeleteRegion(int from, int remaining)
{
	cint col_count = columns_.size();
	int so_far = 0;
	cint till = from + remaining;
	
	for (int i = 0; i < col_count; i++) {
		inst::TableTableColumn *col = columns_[i];
		cint col_start = so_far;
		so_far += col->num();
		cint col_end = so_far;
		
		if (till < col_start || from > col_end)
			continue;
		
		int range_start = std::max(from, col_start);
		int range_end = std::min(till, col_end);
		
		if (range_end - range_start <= 0)
			continue;
		
		ods::DeleteRegion region;
		region.vec_index = i;
		region.start = range_start - col_start;
		region.count = range_end - range_start;
		col->delete_region(region);
	}
}

void Sheet::MarkRowDeleteRegion(int from, int remaining)
{
	cint row_count = rows_.size();
	int so_far = 0;
	cint till = from + remaining;
	
	for (int i = 0; i < row_count; i++) {
		ods::Row *row = rows_[i];
		cint row_start = so_far;
		so_far += row->num();
		cint row_end = so_far;
		
		if (till < row_start || from > row_end)
			continue;
		
		int range_start = std::max(from, row_start);
		int range_end = std::min(till, row_end);
		
		if (range_end - range_start <= 0)
			continue;
		
		ods::DeleteRegion region;
		region.vec_index = i;
		region.start = range_start - row_start;
		region.count = range_end - range_start;
		row->delete_region(region);
	}
}

inst::TableTableColumn*
Sheet::NewColumnAt(cint place, cint ncr)
{
	MarkColumnDeleteRegion(place, ncr);
	int total = 0;
	
	for (int i = columns_.size() - 1; i >= 0; i--) {
		inst::TableTableColumn *col = columns_[i];
		total += col->num();
		
		if (col->has_delete_region()) {
			DeleteColumnRegion(col, i);
		}
	}
	
	int vec_index;
	if (place == total - ncr) {
		vec_index = columns_.size();
	} else if (ColumnAt(place, vec_index) == nullptr) {
		mtl_trace("place: %d, total: %d", place, total);
		return nullptr;
	}

	auto *col = new inst::TableTableColumn(this);
	col->num(ncr);
	col->selected(true);
	columns_.insert(vec_index, col);
	
	return col;
}

ods::Reference*
Sheet::NewReference(ods::Cell *cell, ods::Cell *end_cell)
{
	if (end_cell == nullptr)
		return ods::Reference::Cell(this, cell->NewRef());
	
	return ods::Reference::CellRange(this, cell->NewRef(), end_cell->NewRef());
}

ods::Row*
Sheet::NewRowAt(cint place, cint nrr)
{
	MarkRowDeleteRegion(place, nrr);
	int total = 0;
	
	for (int i = rows_.size() - 1; i >= 0; i--) {
		ods::Row *row = rows_[i];
		total += row->num();
		
		if (row->has_delete_region()) {
			DeleteRowRegion(row, i);
		}
	}
	
	int vec_index;
	if (place == total - nrr) {
		vec_index = rows_.size();
	} else if (RowAt(place, vec_index) == nullptr) {
		mtl_trace("place: %d, total: %d", place, total);
		return nullptr;
	}

	auto *row = new Row(this);
	row->num(nrr);
	row->selected(true);
	rows_.insert(vec_index, row);
	
	return row;
}

int Sheet::QueryRowStart(const Row *row) const
{
	int index = 0;
	
	for (auto *next: rows_)
	{
		if (next == row)
			return index;
		
		index += next->num();
	}
	
	return -1;
}

void Sheet::Scan(ods::Tag *tag)
{
	foreach (auto *x, tag->nodes())
	{
		if (AddText(x))
			continue;
		
		auto *next = x->as_tag();
		if (next->Has(ns_->table())) {
			if (next->Has(ns::kTableRow))
			{
				rows_.append(new ods::Row(this, next));
			} else if (next->Has(ns::kTableColumn)) {
				columns_.append(new ods::inst::TableTableColumn(this, next));
			} else if (next->Has(ns::kNamedExpressions)) {
				named_expressions_ = new inst::TableNamedExpressions(this, next);
			} else {
				Scan(next);
			}
		} else {
			Scan(next);
		}
	}
}

bool Sheet::SetName(const QString &name)
{
	if (name.contains('\'') || name.contains('\"'))
	{
		mtl_warn("Sheet name: No quotation marks");
		return false;
	}
	
	auto *spreadsheet = (inst::OfficeSpreadsheet*)parent_;
	
	if (spreadsheet->GetSheet(name) != nullptr)
		return false;
	
	table_name_ = name;
	return true;
}

void Sheet::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->table(), ns::kName, table_name_);
	Write(xml, ns_->table(), ns::kStyleName, table_style_name_);
	
	for (auto *next: columns_)
		next->Write(xml);
	
	for (auto *next: rows_)
		next->Write(xml);
	
	if (named_expressions_)
		named_expressions_->Write(xml);
}

void Sheet::WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba)
{
	MTL_CHECK_VOID(ba != nullptr);
	WriteTag(kw, *ba);
	WriteNdffProp(kw, *ba, ns_->table(), ns::kName, table_name_);
	WriteNdffProp(kw, *ba, ns_->table(), ns::kStyleName, table_style_name_);
	CloseBasedOnChildren(h, kw, file, ba);
}

} // ods::
