#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

class ODS_API OfficeBody : public Abstract
{
public:
	OfficeBody(Abstract *parent, Tag *tag = nullptr);
	OfficeBody(const OfficeBody &cloner);
	virtual ~OfficeBody();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	OfficeSpreadsheet*
	spreadsheet() const { return office_spreadsheet_; }
	
	void
	WriteData(QXmlStreamWriter &xml) override;
	
private:
	
	void Init(Tag *tag);
	void InitDefault();
	void Scan(Tag *tag);
	
	OfficeSpreadsheet *office_spreadsheet_ = nullptr;
};

} // ods::inst::
} // ods::
