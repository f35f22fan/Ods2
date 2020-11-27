#include "Reference.hpp"

#include "Book.hpp"
#include "Cell.hpp"
#include "CellRef.hpp"
#include "Formula.hpp"
#include "ods.hh"
#include "Sheet.hpp"
#include "inst/OfficeSpreadsheet.hpp"

namespace ods {

Reference::Reference() {}

Reference::~Reference() {
	delete cell_ref_;
	delete end_cell_ref_;
	cell_ref_ = end_cell_ref_ = nullptr;
}

void
Reference::DeepCopy(Reference &dest, const Reference &src) {
	dest.default_sheet_ = src.default_sheet_;
	
	if (src.cell_ref_ != nullptr)
		dest.cell_ref_ = src.cell_ref_->Clone();
	
	if (src.end_cell_ref_ != nullptr)
		dest.end_cell_ref_ = src.end_cell_ref_->Clone();
	
	dest.bits_ = src.bits_;
	dest.r_ = src.r_;
	dest.c_ = src.c_;
}

Reference::Reference(const Reference &c)
{
	DeepCopy(*this, c);
}

ods::CellRef*
Reference::cell()
{
	if (!r1c1())
		return cell_ref_;
	
	if (end_cell_ref_ != nullptr)
		return end_cell_ref_;
	
	if (relative()) {
		CHECK_PTR_NULL(cell_ref_);
		end_cell_ref_ = cell_ref_->NewRelativeTo(r_, c_);
	} else {
		end_cell_ref_ = CellRef::New(default_sheet_, r_, c_);
	}
	
	return end_cell_ref_;
}

Reference*
Reference::Cell(Sheet *default_sheet, CellRef *ref)
{
	Reference *reference = new Reference();
	reference->default_sheet_ = default_sheet;
	reference->cell_ref_ = ref;
	return reference;
}

Reference*
Reference::Cell(Sheet *default_sheet, QStringRef str)
{
	Reference *reference = new Reference();
	reference->default_sheet_ = default_sheet;
	reference->cell_ref_ = ods::CreateCellRef(default_sheet, str);
	return reference;
}

Reference*
Reference::CellRange(Sheet *default_sheet, CellRef *start, CellRef *end)
{
	Reference *reference = new Reference();
	reference->default_sheet_ = default_sheet;
	reference->cell_ref_ = start;
	reference->end_cell_ref_ = end;
	return reference;
}

Reference*
Reference::CellRange(Sheet *default_sheet, QStringRef start, QStringRef end)
{
	Reference *reference = new Reference();
	reference->default_sheet_ = default_sheet;
	reference->cell_ref_ = ods::CreateCellRef(default_sheet, start);
	reference->end_cell_ref_ = ods::CreateCellRef(default_sheet, end, reference->cell_ref_);
	
	return reference;
}

QString
Reference::CellToString(const ods::CellRef *cell_ref) const
{
	CHECK_TRUE_QSTR((cell_ref != nullptr));
	ods::Cell *cell = cell_ref->GetCell();
	
	CHECK_TRUE_QSTR((cell != nullptr));
	ods::Sheet *sheet = cell_ref->sheet();
	CHECK_TRUE_QSTR((sheet != nullptr));
	QString s = QLatin1String(".") + cell->QueryAddress();
	
	if (sheet != default_sheet_) {
		s = QChar('\'') + sheet->name() + QChar('\'') + s;
	}
	
	return s;
}

Reference*
Reference::Clone() const
{
	auto *p = new Reference();
	DeepCopy(*p, *this);
	
	return p;
}

Reference*
Reference::From(const QStringRef &str, Sheet *default_sheet)
{
	int index = str.indexOf(':');
	if (index == -1)
		return Reference::Cell(default_sheet, str);
	
	QStringRef start = str.mid(0, index);
	QStringRef end = str.mid(index + 1);
	return Reference::CellRange(default_sheet, start, end);
}

bool
Reference::GenCells(QVector<ods::Cell*> &cells)
{
	CHECK_PTR(end_cell_ref_);
	ods::Sheet *sheet = cell_ref_->sheet();
	
	const int col_start = std::min(cell_ref_->col(), end_cell_ref_->col());
	const int col_end = std::max(cell_ref_->col(), end_cell_ref_->col());
	
	const int row_start = std::min(cell_ref_->row(), end_cell_ref_->row());
	const int row_end = std::max(cell_ref_->row(), end_cell_ref_->row());
	
	for (int r = row_start; r <= row_end; r++) {
		for (int c = col_start; c <= col_end; c++) {
			ods::Cell *p = CellRef::FetchCell(sheet, r, c);
			CHECK_PTR(p);
			cells.append(p);
		}
	}
	
	return true;
}

Reference*
Reference::Offset(int row_off, int col_off, int new_h, int new_w) const
{
	auto *r = Clone();
	
	if (r1c1()) {
		r->r_ += row_off;
		r->c_ += col_off;
		
		if (new_h != -1 || new_w != -1) {
			mtl_warn("Offset() not fully implemented for R1C1 References");
		}
		
		return r;
	}
	
	if (is_cell_range()) {
		CellRef *ref = r->cell();
		
		ref->row(ref->row() + row_off);
		ref->col(ref->col() + col_off);
		
		CellRef *end_ref = r->end_cell();
		
		if (new_w <= 1 && new_h <= 1) {
			// no range anymore
			delete end_ref;
			r->end_cell(nullptr);
			return r;
		}
		
		if (new_h > 1)
			end_ref->row(ref->row() + new_h - 1);
		
		if (new_w > 1)
			end_ref->col(ref->col() + new_w - 1);
	} else {
		CellRef *ref = r->cell();
		
		ref->row(ref->row() + row_off);
		ref->col(ref->col() + col_off);
		
		if (new_w <= 1 && new_h <= 1)
			return r;
		
		CellRef *end_ref = ref->Clone();
		r->end_cell(end_ref);
		
		if (new_h > 1)
			end_ref->row(ref->row() + new_h - 1);
		
		if (new_w > 1)
			end_ref->col(ref->col() + new_w - 1);
	}
	
	return r;
}

Reference*
Reference::R1C1From(QStringRef str, ods::Formula *formula)
{
	int excl = str.indexOf('!');
	ods::Sheet *reference_sheet = nullptr;
	bool is_relative = false;
	
	if (excl != -1) {
		auto sheet_name = str.mid(0, excl);
		auto *book = formula->default_sheet()->book();
		auto *sp = book->spreadsheet();
		reference_sheet = sp->GetSheet(sheet_name);
		str = str.mid(excl + 1);
	}
	
	if (reference_sheet == nullptr)
		reference_sheet = formula->default_sheet();
	
	int R = str.indexOf('R');
	CHECK_TRUE_NULL((R != -1));

	int C = str.indexOf('C', R + 1);
	CHECK_TRUE_NULL((C != -1));
	
	auto row_ref = str.mid(0, C);
	auto col_ref = str.mid(C);
	QStringRef num_ref;
	
	if (row_ref.endsWith(']')) {
		is_relative = true;
		num_ref = row_ref.mid(2, row_ref.size() - 3);
	} else {
		num_ref = row_ref.mid(1);
	}
	
	bool ok = true;
	int row_num = num_ref.isEmpty() ? 0 : num_ref.toInt(&ok);
	CHECK_TRUE_NULL(ok);
	
	if (col_ref.endsWith(']')) {
		is_relative = true;
		num_ref = col_ref.mid(2, col_ref.size() - 3);
	} else {
		num_ref = col_ref.mid(1);
	}
	
	int col_num = num_ref.isEmpty() ? 0 : num_ref.toInt(&ok);
	CHECK_TRUE_NULL(ok);
	
	auto *a = new Reference();
	a->default_sheet_ = reference_sheet;
	a->r_ = row_num;
	a->c_ = col_num;
	a->relative(is_relative);
	a->r1c1(true);
	
	if (is_relative) {
		// In this case cell_ref_ is the anchor cell
		// which the numbers are relative to.
		a->cell_ref_ = formula->cell_->NewRef();
	}
	
	return a;
}

void
Reference::PrintR1C1() const {
	mtl_info("r:%d, c:%d, r1c1: %s, relative: %s",
		r_, c_,
		r1c1() ? "true" : "false",
		relative() ? "true" : "false");
}

QString
Reference::toString() const
{
	QString s;
	if (r1c1()) {
		s.append('R');
		if (r_ != 0) {
			if (relative())
				s.append('[');
			s.append(QString::number(r_));
			s.append(']');
		}
		s.append('C');
		if (c_ != 0) {
			if (relative())
				s.append('[');
			s.append(QString::number(c_));
			s.append(']');
		}
	} else {
		s.append('[');
		s.append(CellToString(cell_ref_));
		if (is_cell_range()) {
			s.append(':');
			s.append(CellToString(end_cell_ref_));
		}
		s.append(']');
	}
	
	return s;
}

}
