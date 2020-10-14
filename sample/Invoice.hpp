#pragma once

#include <ods/ods>

namespace sample {

class InvoiceItem;

class Invoice {
public:
	Invoice();
	virtual ~Invoice();
	
private:
	
	int CreateBillToHeader(const int kLastRow);
	int CreateSellerHeader();
	int CreateTable(QVector<InvoiceItem*> *vec, const int kLastRow);
	int CreateTableHeader(const int kLastRow);
	
	QVector<InvoiceItem*>* GenItems();
	void Init();
	
	ods::Book *book_ = nullptr;
	ods::Sheet *sheet_ = nullptr;
};

}
