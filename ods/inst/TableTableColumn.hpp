#pragma once

#include "Abstract.hpp"
#include "../cell.hxx"
#include "decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API TableTableColumn : public Abstract
{
public:
	TableTableColumn(Abstract *parent, Tag *tag = 0, ndff::Container *cntr = 0);
	TableTableColumn(const TableTableColumn &cloner);
	virtual ~TableTableColumn();
	
	virtual inst::Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	inst::StyleTableColumnProperties*
	FetchTableColumnProperties();
	
	StyleStyle*
	GetDefaultCellStyle() const;
	
	inst::StyleStyle*
	GetStyle() const;
	
	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	
	int num() const { return ncr_; }
	void num(const int n) { ncr_ = n; }
	int number_columns_repeated() const { return ncr_; }
	void number_columns_repeated(const int n) { ncr_ = n; }

	 // caller must not delete returned value
	Length* QueryColumnWidth() const;
	
	void SetStyle(inst::StyleStyle *style);
	void SetWidth(Length *length);
	void WriteData(QXmlStreamWriter &xml) override;
	void WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba) override;
private:
	
	void delete_region(const DeleteRegion &dr) {
		delete_region_ = dr;
	}
	
	const DeleteRegion&
	delete_region() const { return delete_region_; }
	bool has_delete_region() const { return delete_region_.start != -1; }
	
	void Init(ndff::Container *cntr);
	void Init(Tag *tag);
	QString ToSchemaString() const;
	
	bool selected() const { return bits_ & SelectedBit; }
	
	void selected(const bool do_set) {
		if (do_set)
			bits_ |= SelectedBit;
		else
			bits_ &= ~SelectedBit;
	}
	
	QString table_style_name_;
	int ncr_ = 1;
	QString table_default_cell_style_name_;
	
	u8 bits_ = 0;
	ods::DeleteRegion delete_region_ = {-1, -1, -1};
	
	friend class ods::Sheet;
};

} // ods::inst::
