#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"
#include "../types.hxx"

namespace ods { // ods::
namespace inst { // ods::inst::

const u8 TriedLoadingAddressBit = 1u << 0;
const u8 IsGlobalBit = 1u << 1;

class ODS_API TableNamedRange : public Abstract
{
public:
	TableNamedRange(Abstract *parent, Tag *tag = nullptr);
	TableNamedRange(const TableNamedRange &cloner);
	virtual ~TableNamedRange();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	const QString& name() const { return name_; }
	const QString& cell_range_address() const { return table_cell_range_address_; }
	ods::Reference* GetReference();
	bool global() const { return bits_ & IsGlobalBit; }
	void global(const bool flag) {
		if (flag)
			bits_ |= IsGlobalBit;
		else
			bits_ &= ~IsGlobalBit;
	}
	
	void
	WriteData(QXmlStreamWriter &xml) override;
	
	ods::Sheet*
	GetSheet();
	
private:
	
	void Init(ods::Tag *tag);
	void InitDefault();
	
	QString table_base_cell_address_;
	QString table_cell_range_address_;
	 // This is the name of the named range, not of the table:
	QString name_;
	// The sheet it belongs to:
	ods::Sheet *sheet_ = nullptr;
	ods::Reference *reference_ = nullptr;
	u8 bits_ = 0;
};

} // ods::inst::
} // ods::
