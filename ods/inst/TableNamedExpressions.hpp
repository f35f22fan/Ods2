#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

class ODS_API TableNamedExpressions : public Abstract
{
public:
	TableNamedExpressions(Abstract *parent, Tag *tag = nullptr);
	TableNamedExpressions(const TableNamedExpressions &cloner);
	virtual ~TableNamedExpressions();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;

	void
	WriteData(QXmlStreamWriter &xml) override;
	
private:
	
	void Init(ods::Tag *tag);
	void InitDefault();
};

} // ods::inst::
} // ods::
