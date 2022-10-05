#pragma once

#include "decl.hxx"
#include "inst/decl.hxx"
#include "err.hpp"
#include "global.hxx"

#include "inst/Abstract.hpp"

namespace ods {

class ODS_API Sheet : public ods::inst::Abstract
{
public:
	Sheet(inst::Abstract *parent, Tag *sheet_tag);
	Sheet(inst::Abstract *parent);
	Sheet(const Sheet &cloner);
	virtual ~Sheet();
	
	ods::Book*
	book() const { return book_; }
	
	virtual inst::Abstract*
	Clone(inst::Abstract *parent = nullptr) const override;
	
	int CountColumns();
	int CountRows() const;
	
	inst::TableTableColumn*
	GetColumn(const int place) const;
	
	inst::StyleStyle*
	GetDefaultCellStyle(const Cell *cell) const;
	
	ods::Row*
	GetRow(const int place);
	
	bool has_children(const inst::IncludingText itx = inst::IncludingText::Yes) const override {
		return named_expressions_ || rows_.size() > 0 ||
		columns_.size() > 0;
	}
	void ListChildren(QVector<StringOrInst*> &vec, const Recursively r) override;
	void ListKeywords(inst::Keywords &list, const inst::LimitTo lt) override;
	void ListUsedNamespaces(inst::NsHash &list) override;
	
	const QString&
	name() const { return table_name_; }
	
	inst::TableNamedExpressions*
	named_expressions() const { return named_expressions_; }
	
	ods::Reference*
	NewReference(ods::Cell *cell, ods::Cell *end_cell = nullptr);
	
	inst::TableTableColumn*
	NewColumnAt(cint place, cint ncr = 1);
	
	ods::Row*
	NewRowAt(const int place, const int nrr = 1);
	
	int num_cols() const { return num_cols_; }
	int QueryRowStart(const Row *row) const;
	// returns true if successful
	bool SetName(const QString &name);
	void WriteData(QXmlStreamWriter &xml) override;
	void WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba) override;
private:
	
	Row* RowAt(const int place, int &vec_index);
	inst::TableTableColumn* ColumnAt(const int place, int &vec_index);
	void DeleteRowRegion(ods::Row *row, const int vec_index);
	void DeleteColumnRegion(inst::TableTableColumn *col, const int vec_index);
	void Init(ods::Tag *sheet_tag);
	void InitDefault();
	void MarkColumnDeleteRegion(int from, int remaining);
	void MarkRowDeleteRegion(int from, int remaining);
	void name(const QString &name) { table_name_ = name; }
	
	void Scan(ods::Tag *tag);
	
	QString table_name_;
	QString table_style_name_;
	QVector<ods::Row*> rows_;
	QVector<inst::TableTableColumn*> columns_;
	inst::TableNamedExpressions *named_expressions_ = nullptr;
	int num_cols_ = 0;
	
	cint DefaultColumnCountPerSheet = 1024;
	cint DefaultRowCountPerSheet = 0x0FFFFF; // 1048575
	
	friend class inst::OfficeSpreadsheet;
};

} // ods::
