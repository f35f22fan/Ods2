#include "InvoiceItem.hpp"

namespace sample {

InvoiceItem::InvoiceItem(const QString &descr, const double price, const int qtty)
{
	description_ = descr;
	qtty_ordered_ = qtty;
	unit_price_ = price;
	id_ = GetNextId();
}

InvoiceItem::~InvoiceItem()
{}

int
InvoiceItem::GetNextId()
{
	static int id = 0;
	return id++;
}

}
