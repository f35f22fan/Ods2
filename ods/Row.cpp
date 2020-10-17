#include "Row.hpp"

#include "Book.hpp"
#include "Cell.hpp"
#include "Length.hpp"
#include "Ns.hpp"
#include "ns.hxx"
#include "Sheet.hpp"
#include "Tag.hpp"

#include "inst/StyleStyle.hpp"
#include "inst/StyleTableRowProperties.hpp"
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
Row::At(const int place, int &vec_index) {
	int so_far = 0;
	const int count = cells_.size();
	
	for (int i = 0; i < count; i++) {
		ods::Cell *cell = cells_[i];
		so_far += cell->ncr();
//		mtl_line("place: %d, so_far: %d, i=%d", place, so_far, i);
		if (so_far > place) {
			vec_index = i;
//			mtl_line("vec_index=%d", vec_index);
			return cell;
		}
	}
	
	return nullptr;
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

ods::Cell*
Row::GetCell(const int place)
{
	int start = 0;
	
	for (ods::Cell *cell: cells_)
	{
		start += cell->ncr();
		
		if (start > place)
			return cell;
	}
	
	return nullptr;
}

inst::StyleStyle*
Row::GetStyle() const
{
	return Get(table_style_name_);
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

void
Row::DeleteRegion(ods::Cell *cell, const int vec_index)
{
	const ods::DeleteRegion &dr = cell->delete_region();
	
	if (dr.count == cell->ncr()) {
		delete cell;
		cells_.remove(dr.vec_index);
		return;
	}
	
	const int ncr = cell->ncr();
	const int region_len = dr.start + dr.count;
	
	if (dr.start > 0 && region_len < ncr) {
		Cell *cell2 = static_cast<Cell*>(cell->Clone());
		cell2->ncr(dr.start);
		cells_.insert(vec_index, cell2);
		cell->ncr(ncr - region_len);
	} else {
		cell->ncr(ncr - dr.count);
	}
	
	cell->delete_region({-1, -1, -1});
}

void
Row::MarkDeleteRegion(int from, int remaining)
{
	const int cell_count = cells_.size();
	int so_far = 0;
	const int till = from + remaining;
	
	for (int i = 0; i < cell_count; i++) {
		auto *cell = cells_[i];
		const int cell_start = so_far;
		so_far += cell->ncr();
		const int cell_end = so_far;
		
		if (till < cell_start || from > cell_end)
			continue;
		
		int range_start = std::max(from, cell_start);
		int range_end = std::min(till, cell_end);
//		printf("\n(%d, %d) - (%d, %d) = (%d, %d)\n",
//			cell_start, cell_end, from, till, range_start, range_end);
		
		if (range_end - range_start <= 0)
			continue;
		
		ods::DeleteRegion region;
		region.vec_index = i;
		region.start = range_start - cell_start;
		region.count = range_end - range_start;
		cell->delete_region(region);
//		printf("DeleteRegion [%d, %d] (%d, %d)\n",
//			region.start, region.count, range_start, range_end);
	}
}

void
Row::MarkCoveredCellsAfter(ods::Cell *cell, const int vec_index)
{
	const int vec_size = cells_.size();
	const bool more_cells_follow = vec_index < vec_size - 1;
	
	if (!more_cells_follow || cell->ncs() <= 1)
		return;
	
//	mtl_line("make %d cells covered", cell->ncs() - 1);
	int remaining = cell->ncs() - 1;
	
	for (int i = vec_index + 1; i < vec_size; i++) {
		auto *next = cells_[i];
		int x = next->ncr() - remaining;
		
		if (x <= 0) {
			next->covered(true);
//			auto ba = next->ValueToString().toLocal8Bit();
//			mtl_line("Covered cell: %s", ba.data());
			remaining -= next->ncr();
		} else {
			Cell *cloned = static_cast<Cell*>(next->Clone());
			cloned->ncr(remaining);
			cloned->covered(true);
//			auto ba = cloned->ValueToString().toLocal8Bit();
//			mtl_line("Covered cell: %s", ba.data());
			cells_.insert(i, cloned);
			next->ncr(x);
			remaining = 0;
		}
		
		if (remaining <= 0)
			break;
	}
}

Cell*
Row::NewCellAt(const int place, const int ncr, const int ncs)
{
	MarkDeleteRegion(place, ncr);
	int total = 0;
	
	for (int i = cells_.size() - 1; i >= 0; i--) {
		auto *cell = cells_[i];
		total += cell->ncr();
		
		if (cell->has_delete_region()) {
			DeleteRegion(cell, i);
		}
	}
	
	int vec_index;
	if (place == total - ncr) {
		vec_index = cells_.size();
	} else if (At(place, vec_index) == nullptr) {
		mtl_trace("place: %d, total: %d", place, total);
		return nullptr;
	}

	auto *cell = new Cell(this);
	cell->ncr(ncr);
	cell->ncs(ncs);
	cell->selected(true);
	cells_.insert(vec_index, cell);
	MarkCoveredCellsAfter(cell, vec_index);
	
	return cell;
}

inst::StyleStyle*
Row::NewStyle()
{
	auto *style = book_->NewRowStyle();
	SetStyle(style);
	
	return style;
}

void
Row::Print() const
{
	int count = 0;
	for (auto *cell: cells_) {
		auto ba = cell->ToSchemaString().toLocal8Bit();
		printf("%s ", ba.data());
		count += cell->ncr();
	}
	
	printf("{%d}\n", count);
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
Row::SetOptimalHeight()
{
	Length *size = QueryOptimalHeight();
	
	if (size == nullptr)
		return;
	
	inst::StyleStyle *style = GetStyle();
	
	if (style == nullptr)
		style = NewStyle();
	
	inst::StyleTableRowProperties *trp = style->GetTableRowProperties();
	
	if (trp == nullptr)
		trp = style->NewTableRowProperties();
	
	trp->SetOptimal(size);
}

void
Row::SetStyle(const ods::Row *p)
{
	if (p == nullptr)
		table_style_name_.clear();
	else
		table_style_name_ = p->table_style_name_;
}

void
Row::SetStyle(inst::StyleStyle *p)
{
	if (p == nullptr)
		table_style_name_.clear();
	else
		table_style_name_ = *p->style_name();
		
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
