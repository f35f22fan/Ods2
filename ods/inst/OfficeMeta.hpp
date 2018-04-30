#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

class ODS_API OfficeMeta : public Abstract
{
public:
	OfficeMeta(Abstract *parent, Tag *tag = nullptr);
	OfficeMeta(const OfficeMeta &cloner);
	virtual ~OfficeMeta();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;

	void
	WriteData(QXmlStreamWriter &xml) override;
	
private:
	
	void Init(ods::Tag *tag);
	void InitDefault();
	void Scan(ods::Tag *tag);
};

} // ods::inst::
} // ods::
