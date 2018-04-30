#include "Item.hpp"

namespace util { // util::

Item::Item(const QString &descr, const double price, const int qtty)
: unit_price_(price), qtty_ordered_(qtty)
{
	description_ = descr;
	id_ = GetNextId();
}

Item::~Item()
{}

int
Item::GetNextId()
{
	static int id = 0;
	return id++;
}


} // util::
