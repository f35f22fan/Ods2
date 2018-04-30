#include "Invoice.hpp"

#include "util.hh"

namespace util { // util::

Invoice::Invoice()
{
	Init();
}

Invoice::~Invoice()
{
	if (book_ != nullptr)
	{
		util::Save(book_, "Invoice.ods");
		delete book_;
	}
}

void
Invoice::Init()
{
	book_ = ods::Book::New();
	auto *spreadsheet = book_->spreadsheet();
	auto *sheet = spreadsheet->NewSheet("Sheet name");
	auto *row = sheet->NewRowAt(0);
	auto *cell0 = row->NewCellAt(0);
	cell0->SetDouble(5.0);
	
}

} // util::
