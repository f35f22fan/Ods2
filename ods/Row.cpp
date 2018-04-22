#include "Row.hpp"

#include "Book.hpp"
#include "Cell.hpp"
#include "Length.hpp"
#include "Ns.hpp"
#include "ns.hxx"
#include "Sheet.hpp"
#include "Tag.hpp"

#include "inst/StyleStyle.hpp"
#include "inst/StyleTextProperties.hpp"

namespace ods { // ods::

Row::Row(ods::Sheet *parent, ods::Tag *tag)
: Abstract(parent, parent->ns(), id::TableTableRow),
sheet_(parent)
{
	Init(tag);
}

Row::Row(Sheet *parent)
: Abstract (parent, parent->ns(), id::TableTableRow),
sheet_(parent)
{
	InitDefault();
}

Row::Row(const Row &cloner) : Abstract(cloner)
{}

Row::~Row()
{
	for (auto *cell: cells_)
		delete cell;
}

Cell*
Row::At(const int logical_index, int &starts_at, int &total_li, int &vec_index)
{
	total_li = 0;
	Cell *found_obj = nullptr;
	
	for (int i = 0; i < cells_.size(); i++)
	{
		auto *next = cells_[i];
		total_li += next->ncr();
		
		if (found_obj == nullptr && total_li > logical_index)
		{
			found_obj = next;
			starts_at = total_li - next->ncr();
			vec_index = i;
		}
	}
	
	return found_obj;
}

inst::Abstract*
Row::Clone(inst::Abstract *parent) const
{
	auto *p = new Row(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	for (auto *next: cells_)
	{
		Cell *cell = (Cell*)next->Clone(p);
		p->cells_.append(cell);
	}
	
	p->sheet_ = sheet_;
	p->number_rows_repeated_ = number_rows_repeated_;
	p->num_rows_spanned_ = num_rows_spanned_;
	p->table_style_name_ = table_style_name_;
	
	return p;
}

void
Row::Curtail(const int by_how_much, const int from_where)
{
	if (by_how_much <= 0 || from_where < 0)
	{
		mtl_warn("by_how_much: %d, from_where: %d", by_how_much, from_where);
		return;
	}
	
	//mtl_line("Curtail by %d from %d", by_how_much, from_where);
	int next_off = 0;
	const int max = cells_.size();
	
	for (int i = 0; i < max; i++)
	{
		ods::Cell *cell = cells_[i];
		const int ncr = cell->ncr();
		next_off += ncr;
		
		if (next_off > from_where)
		{
			int diff = ncr - by_how_much;
			
			if (diff == 0) // done
			{
				cells_.removeAt(i);
				delete cell;
				return;
			} else if (diff < 0) {
				cells_.removeAt(i);
				delete cell;
				Curtail(by_how_much - ncr, from_where + ncr);
				return;
			} else { // diff > 0
				cell->ncr(diff);
				return;
			}
		}
	}
}

ods::Cell*
Row::GetCell(const int index)
{
	int start = 0;
	
	for (ods::Cell *cell: cells_)
	{
		start += cell->ncr();
		
		if (start > index)
			return cell;
	}
	
	return nullptr;
}

void
Row::Init(ods::Tag *tag)
{
	tag->Copy(ns_->table(), ods::ns::kNumberRowsRepeated, number_rows_repeated_);
	tag->Copy(ns_->table(), ods::ns::kNumberRowsSpanned, num_rows_spanned_);
	tag->Copy(ns_->table(), ods::ns::kStyleName, table_style_name_);
	Scan(tag);
}

void
Row::InitDefault()
{
	const int max = sheet_->num_cols();
	int count = 0;
	
	for (Cell *cell: cells_)
		count += cell->ncr();
	
	if (count >= max)
		return;
	
	auto *cell = new Cell(this);
	const int diff = max - count;
	cell->ncr(diff);
	cells_.append(cell);
}

Cell*
Row::NewCell(const int insert_li, const int ncr, const AddMode mode)
{
	if (ncr <= 0)
	{
		mtl_warn();
		return nullptr;
	}
	
	int vec_index = 0;
	int total_li = 0;
	int next_obj_starts_at = 0;
	Cell *next_cell = At(insert_li, next_obj_starts_at, total_li, vec_index);
	
	if (next_cell == nullptr)
	{
		mtl_warn("no next_obj at li %d, obj_vec size: %d", insert_li, cells_.size());
		return nullptr;
	}
	
	int need_to_cut = ncr;
	Cell *new_obj = new Cell(this);
	new_obj->ncr(ncr);
	const bool do_replace = (mode == AddMode::Replace);
	
	int new_obj_start_offset = insert_li - next_obj_starts_at;
	int orig_next_obj_ncr = next_cell->ncr();
	
	if (new_obj_start_offset > 0)
	{
		// split
		next_cell->ncr(new_obj_start_offset);
		cells_.insert(vec_index + 1, new_obj);
		
		if (do_replace)
		{
			int skip = new_obj_start_offset + ncr;
			int next_row_left = orig_next_obj_ncr - skip;
			
			if (next_row_left >= 0)
			{
				if (next_row_left > 0)
				{
					auto *sh = (Cell*)next_cell->Clone();
					sh->ncr(next_row_left);
					
					cells_.insert(vec_index + 2, sh);
				}
				
				return new_obj;
			} else { // next_row_left < 0
				need_to_cut = -next_row_left;
				Curtail(need_to_cut, insert_li + ncr);
			}
		} else {
			// insert remaining million!
			int new_num = orig_next_obj_ncr - ncr - new_obj_start_offset;
			auto *sh = (Cell*)next_cell->Clone();
			sh->ncr(new_num);
			cells_.insert(vec_index + 2, sh);
			//Curtail(ncr, total_li - ncr);
		}
	} else {
		cells_.insert(vec_index, new_obj);
		if (do_replace)
		{
			Curtail(ncr, insert_li + ncr);
		} else {
			Curtail(ncr, total_li - ncr);
		}
	}
	
	return new_obj;
}

ods::Cell*
Row::NewCellAt(const int col_index, const int ncr)
{
	return NewCell(col_index, ncr, AddMode::Insert);
}

ods::Cell*
Row::NewCellInPlaceOf(const int col_index, const int ncr)
{
	return NewCell(col_index, ncr, AddMode::Replace);
}

Length*
Row::QueryOptimalHeight() const
{
	Length *tallest = nullptr;
	
	for (ods::Cell *cell: cells_)
	{
		Length *h = cell->QueryDesiredHeight();
		
		if (h == nullptr)
			continue;
		
		if (tallest == nullptr || *h > *tallest)
			tallest = h;
		else
			delete h;
	}
	
	return tallest;
}

int
Row::QueryCellStart(const Cell *cell) const
{
	int index = 0;
	
	for (auto *next: cells_)
	{
		if (next == cell)
			return index;
		
		index += next->ncr();
	}
	
	return -1;
}

int
Row::QueryStart() const
{
	return sheet_->QueryRowStart(this);
}

void
Row::Scan(ods::Tag *tag)
{
	foreach (StringOrTag *x, tag->nodes())
	{
		if (!x->is_tag())
			continue;
		
		auto *next = x->as_tag();
		
		if (next->IsAnyCell())
		{
			cells_.append(new ods::Cell(this, next));
		} else {
			Scan(next);
		}
	}
}

void
Row::SetCellAt(ods::Cell *new_cell, const int insert_at)
{
	// !! Don't forget to Append(new_cell)
	int prev_cell_up_to = 0;
	ods::Cell *prev_cell = nullptr;
	int prev_cell_index = 0;
	
	for (; prev_cell_index < cells_.size(); prev_cell_index++)
	{
		ods::Cell *next = cells_[prev_cell_index];
		prev_cell_up_to += next->ncr();
//mtl_line("insert at:%d, current up to:%d", insert_at, prev_cell_up_to);
		
		if (prev_cell_up_to > insert_at)
		{
			prev_cell = next;
			break;
		}
	}
	
	if (prev_cell == nullptr)
	{
//		mtl_line("At zero, cells count: %d, insert_at: %d",
//			cells_.size(), insert_at);
		cells_.insert(insert_at, new_cell);
		const int by_how_much = new_cell->ncr();
		const int from_where = insert_at + new_cell->ncr();
		Curtail(by_how_much, from_where);
		return;
	}
	
	const int new_cell_ncr = new_cell->ncr();
	const int prev_ncr = prev_cell->ncr();
	const int prev_cell_starts_at = prev_cell_up_to - prev_ncr;
	
//mtl_line("prev_cell_index: %d, up_to: %d, starts at: %d",
//		prev_cell_index, prev_cell_up_to, prev_cell_starts_at);
	
	if (prev_cell_starts_at == insert_at)
	{
//mtl_line("CASE 1");
		// 1.1 Shrink prev_cell's num_cols_repeated (NCR) accordingly.
		// If prev_cell's NCR <= 0 - delete it.
		
		int diff = prev_ncr - new_cell_ncr;
		
		if (diff == 0)
		{
			cells_.removeAt(prev_cell_index);
			delete prev_cell;
		} else if (diff < 0) {
			Curtail(new_cell_ncr, prev_cell_up_to);
		} else {
			prev_cell->ncr(diff);
		}
		
		cells_.insert(prev_cell_index, new_cell);
		return;
	} else { // (prev_cell_up_to > insert_at)
		const int ncr_before = insert_at - prev_cell_starts_at;
//mtl_line("CASE 2, cut_ncr_to: %d", ncr_before);
		prev_cell->ncr(ncr_before);
		const int new_cell_index = prev_cell_index + 1;
		cells_.insert(new_cell_index, new_cell);
		
		int ncr_after = prev_cell_up_to - (insert_at + new_cell_ncr);
		
		if (ncr_after == 0)
		{
//mtl_line("Done!");
		} else if (ncr_after > 0) {
//mtl_line();
			auto *rem_cell = (ods::Cell*)prev_cell->Clone();
			rem_cell->ncr(ncr_after);
			const int rem_index = new_cell_index + 1;
			cells_.insert(rem_index, rem_cell);
		} else {
			mtl_warn("ncr_after: %d", ncr_after);
		}
	}
}

void
Row::SetStyle(const ods::Row *p)
{
	if (p != nullptr)
		table_style_name_ = p->table_style_name_;
	else
		table_style_name_.clear();
}

void
Row::SetStyle(inst::StyleStyle *p)
{
	if (p != nullptr)
		table_style_name_ = *p->style_name();
	else
		table_style_name_.clear();
}

void
Row::WriteData(QXmlStreamWriter &xml)
{
	if (number_rows_repeated_ != 1)
		xml.writeAttribute(ns_->table()->With(ods::ns::kNumberRowsRepeated),
			QString::number(number_rows_repeated_));
	
	if (num_rows_spanned_ != 1)
		xml.writeAttribute(ns_->table()->With(ods::ns::kNumberRowsSpanned),
			QString::number(num_rows_spanned_));
	
	if (!table_style_name_.isEmpty())
		xml.writeAttribute(ns_->table()->With(ods::ns::kStyleName),
		table_style_name_);
	
	for (auto *next: cells_)
		next->Write(xml);
}

} // ods::
