#pragma once

#include <ods/ods>

namespace sample {

class InvoiceItem;

class Invoice {
public:
	Invoice(const ods::EnableNdff en = ods::EnableNdff::No);
	virtual ~Invoice();
	
	ods::Book* book() const { return book_; }
	
private:
	
	int CreateBillToHeader(const int kLastRow);
	int CreateSellerHeader();
	int CreateTable(QVector<InvoiceItem*> *vec, const int kLastRow);
	int CreateTableHeader(const int kLastRow);
	
	QVector<InvoiceItem*>* GenItems();
	void Init(const ods::EnableNdff en);
	QString save_path() const { return save_path_; }
	
	ods::Book *book_ = nullptr;
	ods::Sheet *sheet_ = nullptr;
	QString save_path_;
};

}
