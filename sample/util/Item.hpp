#pragma once

#include <QString>

namespace util { // util::

class Item
{
public:
	Item(const QString &descr, const double price, const int qtty);
	virtual ~Item();
	
	const QString&
	description() const { return description_; }
	
	void
	description_set(const QString &s) { description_ = s; }
	
	static int
	GetNextId();
	
	int
	id() const { return id_; }
	
	void
	id_set(const int n) { id_ = n; }
	
	int
	qtty_ordered() const { return qtty_ordered_; }
	
	void
	qtty_ordered_set(const int n) { qtty_ordered_ = n; }
	
	double
	unit_price() const { return unit_price_; }
	
	void
	unit_price_set(const double n) { unit_price_ = n; }
	
private:
	int id_ = -1;
	QString description_;
	double unit_price_ = 0.0f;
	int qtty_ordered_ = 0;
};

} // util::

