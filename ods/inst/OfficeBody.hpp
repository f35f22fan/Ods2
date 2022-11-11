#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API OfficeBody : public Abstract
{
public:
	OfficeBody(Abstract *parent, Tag *tag = 0, ndff::Container *cntr = 0);
	OfficeBody(const OfficeBody &cloner);
	virtual ~OfficeBody();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	bool has_children(const IncludingText itx) const override { return office_spreadsheet_; }
	void ListChildren(QVector<StringOrInst*> &vec, const Recursively r) override;
	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	
	OfficeSpreadsheet*
	spreadsheet() const { return office_spreadsheet_; }
	
	void WriteData(QXmlStreamWriter &xml) override;
private:
	
	void Init(ndff::Container *cntr);
	void Init(Tag *tag);
	void InitDefault();
	void Scan(Tag *tag);
	
	OfficeSpreadsheet *office_spreadsheet_ = nullptr;
};

} // ods::inst::
