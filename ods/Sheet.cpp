#include "Sheet.hpp"

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

namespace ods { // ods::

Sheet::Sheet(ods::inst::Abstract *parent, Tag *tag) :
	Abstract(parent, parent->ns(), id::TableTable)
{
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
Sheet::At(const int logical_index, int &starts_at, int &total_li, int &vec_index)
{
	total_li = 0;
	Row *found_obj = nullptr;
	auto *vec = &rows_;
	
	for (int i = 0; i < vec->size(); i++)
	{
		auto *next = (*vec)[i];
		total_li += next->num();
		
		if (found_obj == nullptr && total_li > logical_index)
		{
			found_obj = next;
			starts_at = total_li - next->num();
			vec_index = i;
		}
	}
	
	return found_obj;
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
Sheet::ColumnAt(const int logical_index, int &starts_at, int &total_li, int &vec_index)
{
	total_li = 0;
	inst::TableTableColumn *found_obj = nullptr;
	auto *vec = &columns_;
	
	for (int i = 0; i < vec->size(); i++)
	{
		auto *next = (*vec)[i];
		total_li += next->num();
		
		if (found_obj == nullptr && total_li > logical_index)
		{
			found_obj = next;
			starts_at = total_li - next->num();
			vec_index = i;
		}
	}
	
	return found_obj;
}

int
Sheet::CountColumns()
{
	num_cols_ = 0;
	
	for (inst::TableTableColumn *col: columns_)
		num_cols_ += col->number_columns_repeated();
	
	return num_cols_;
}

int
Sheet::CountRows() const
{
	int n = 0;
	
	for (Row *row: rows_)
		n += row->num();
	
	return n;
}

void
Sheet::Curtail(const int by_how_much, const int from_where)
{
	if (by_how_much <= 0 || from_where < 0)
	{
		mtl_warn("by_how_much: %d, from_where: %d", by_how_much, from_where);
		return;
	}
	
	int next_off = 0;
	auto *vec = &rows_;
	const int max = vec->size();
	
	for (int i = 0; i < max; i++)
	{
		auto *slave = (*vec)[i];
		const int num = slave->num();
		next_off += num;
		
		if (next_off > from_where)
		{
			int diff = num - by_how_much;
			
			if (diff == 0) // done
			{
				vec->removeAt(i);
				delete slave;
				return;
			} else if (diff < 0) {
				vec->removeAt(i);
				delete slave;
				Curtail(by_how_much - num, from_where + num);
				return;
			} else { // diff > 0
				slave->num(diff);
				return;
			}
		}
	}
}

void
Sheet::CurtailCols(const int by_how_much, const int from_where)
{
	if (by_how_much <= 0 || from_where < 0)
	{
		mtl_warn("by_how_much: %d, from_where: %d", by_how_much, from_where);
		return;
	}
	
	int next_off = 0;
	auto *vec = &columns_;
	const int max = vec->size();
	
	for (int i = 0; i < max; i++)
	{
		auto *slave = (*vec)[i];
		const int num = slave->num();
		next_off += num;
		if (next_off > from_where)
		{
			int diff = num - by_how_much;
			
			if (diff == 0) // done
			{
				vec->removeAt(i);
				delete slave;
				return;
			} else if (diff < 0) {
				vec->removeAt(i);
				delete slave;
				CurtailCols(by_how_much - num, from_where + num);
				return;
			} else { // diff > 0
				slave->num(diff);
				return;
			}
		}
	}
}

inst::TableTableColumn*
Sheet::GetColumn(const int col_num) const
{
	int next_col = 0;
	
	for (auto *next: columns_)
	{
		next_col += next->num();
		
		if (next_col > col_num)
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
Sheet::GetRow(const int index)
{
	int start = 0;
	
	for (ods::Row *r: rows_)
	{
		start += r->num();
		
		if (start > index)
			return r;
	}
	
	return nullptr;
}

void
Sheet::Init(ods::Tag *tag)
{
	tag->Copy(ns_->table(), ods::ns::kName, table_name_);
	tag->Copy(ns_->table(), ods::ns::kStyleName, table_style_name_);
	Scan(tag);
}

void
Sheet::InitDefault()
{
	auto *column = new inst::TableTableColumn(this);
	num_cols_ = DefaultColumnCountPerSheet;
	column->number_columns_repeated(num_cols_);
	columns_.append(column);
	
	auto *row = new ods::Row(this);
	row->num(DefaultRowCountPerSheet);
	rows_.append(row);
}

inst::TableTableColumn*
Sheet::NewColumn(const int insert_li, const int num, const AddMode mode)
{
	if (num <= 0)
	{
		mtl_warn();
		return nullptr;
	}
	
	int vec_index = 0;
	int total_li = 0;
	int next_slave_starts_at = 0;
	auto *next_slave = ColumnAt(insert_li, next_slave_starts_at, total_li, vec_index);
	
	if (next_slave == nullptr)
	{
		mtl_warn("no next_slave at li %d", insert_li);
		return nullptr;
	}
	
	int need_to_cut = num;
	auto *new_slave = new inst::TableTableColumn(this);
	new_slave->num(num);
	const bool do_replace = (mode == AddMode::Replace);
	
	int new_slave_start_offset = insert_li - next_slave_starts_at;
	int orig_next_slave_num = next_slave->num();
	auto *vec = &columns_;
	
	if (new_slave_start_offset > 0)
	{
		next_slave->num(new_slave_start_offset);
		vec->insert(vec_index + 1, new_slave);
		
		// split
		if (do_replace)
		{
			
			int skip = new_slave_start_offset + num;
			int next_slave_left = orig_next_slave_num - skip;
			
			if (next_slave_left >= 0)
			{
				if (next_slave_left > 0)
				{
					auto *sh = (inst::TableTableColumn*)next_slave->Clone();
					sh->num(next_slave_left);
					vec->insert(vec_index + 2, sh);
				}
				
				return new_slave;
			} else { // next_slave_left < 0
				need_to_cut = -next_slave_left;
				CurtailCols(need_to_cut, insert_li + num);
			}
		} else {
			// insert remaining million!
			int new_num = orig_next_slave_num - num - new_slave_start_offset;
			auto *sh = (inst::TableTableColumn*)next_slave->Clone();
			sh->num(new_num);
//			mtl_line("orig_next_slave_num: %d, num: %d,\
// new_slave_start_offset: %d, new_num: %d",
//				orig_next_slave_num, num, new_slave_start_offset, new_num);
			vec->insert(vec_index + 2, sh);
//			CurtailCols(num, total_li - num);
//			mtl_line("Curtail, by_how_much: %d, from where: %d, total_li: %d",
//				num, total_li - num, total_li);
		}
	} else {
		vec->insert(vec_index, new_slave);
		
		if (do_replace)
			CurtailCols(num, insert_li + num);
		else
			CurtailCols(num, total_li - num);
	}
	
	return new_slave;
}

inst::TableTableColumn*
Sheet::NewColumnAt(const int index, const int nrr)
{
	return NewColumn(index, nrr, AddMode::Insert);
}

inst::TableTableColumn*
Sheet::NewColumnInPlaceOf(const int index, const int nrr)
{
	return NewColumn(index, nrr, AddMode::Replace);
}

ods::Row*
Sheet::NewRowAt(const int row_index, const int nrr)
{
	return NewRow(row_index, nrr, AddMode::Insert);
}

ods::Row*
Sheet::NewRowInPlaceOf(const int row_index, const int nrr)
{
	return NewRow(row_index, nrr, AddMode::Replace);
}

Row*
Sheet::NewRow(const int insert_li, const int num, const AddMode mode)
{
	if (num <= 0)
	{
		mtl_warn();
		return nullptr;
	}
	
	int vec_index = 0;
	int total_li = 0;
	int next_slave_starts_at = 0;
	auto *next_slave = At(insert_li, next_slave_starts_at, total_li, vec_index);
	
	if (next_slave == nullptr)
	{
		mtl_warn("no next_slave at li %d", insert_li);
		return nullptr;
	}
	
	int need_to_cut = num;
	auto *new_slave = new Row(this);
	new_slave->num(num);
	const bool do_replace = (mode == AddMode::Replace);
	
	int new_slave_start_offset = insert_li - next_slave_starts_at;
	int orig_next_slave_num = next_slave->num();
	auto *vec = &rows_;
	
	if (new_slave_start_offset > 0)
	{
		// split
		next_slave->num(new_slave_start_offset);
		vec->insert(vec_index + 1, new_slave);
		
		if (do_replace)
		{
			int skip = new_slave_start_offset + num;
			int next_slave_left = orig_next_slave_num - skip;
			
			if (next_slave_left >= 0)
			{
				if (next_slave_left > 0)
				{
					auto *sh = (Row*)next_slave->Clone();
					sh->num(next_slave_left);
					vec->insert(vec_index + 2, sh);
				}
				
				return new_slave;
			} else { // next_slave_left < 0
				need_to_cut = -next_slave_left;
				Curtail(need_to_cut, insert_li + num);
			}
		} else {
			// insert remaining million!
			int new_num = orig_next_slave_num - num - new_slave_start_offset;
			auto *sh = (Row*)next_slave->Clone();
			sh->num(new_num);
			vec->insert(vec_index + 2, sh);
			//Curtail(num, total_li - num);
		}
	} else {
		vec->insert(vec_index, new_slave);
		
		if (do_replace)
			Curtail(num, insert_li + num);
		else
			Curtail(num, total_li - num);
	}
	
	return new_slave;
}

int
Sheet::QueryRowStart(const Row *row) const
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

void
Sheet::Scan(ods::Tag *tag)
{
	foreach (auto *x, tag->nodes())
	{
		if (AddText(x))
			continue;
		
		auto *next = x->as_tag();
		
		if (next->Is(ns_->table(), ods::ns::kTableRow))
		{
			rows_.append(new ods::Row(this, next));
		} else if (next->Is(ns_->table(), ods::ns::kTableColumn)) {
			columns_.append(new ods::inst::TableTableColumn(this, next));
		} else {
			Scan(next);
		}
	}
}

bool // returns true if successfull
Sheet::SetName(const QString &name)
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

void
Sheet::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->table(), ods::ns::kName, table_name_);
	Write(xml, ns_->table(), ods::ns::kStyleName, table_style_name_);
	
	for (auto *next: columns_)
		next->Write(xml);
	
	for (auto *next: rows_)
		next->Write(xml);
}

} // ods::
