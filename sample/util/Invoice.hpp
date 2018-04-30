#pragma once

#include <ods/ods>

namespace util { // util::

class Invoice
{
public:
	Invoice();
	virtual ~Invoice();

private:
	NO_ASSIGN_COPY_MOVE(Invoice);
	
	void Init();
	
	ods::Book *book_ = nullptr;
};

} // util::
