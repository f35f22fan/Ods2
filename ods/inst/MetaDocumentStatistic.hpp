#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

class ODS_API MetaDocumentStatistic : public Abstract
{
public:
	MetaDocumentStatistic(Abstract *parent, Tag *tag = nullptr);
	MetaDocumentStatistic(const MetaDocumentStatistic &cloner);
	virtual ~MetaDocumentStatistic();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;

	void
	WriteData(QXmlStreamWriter &xml) override;
	
private:
	
	void Init(Tag *tag);
	
//<meta:document-statistic meta:table-count="1" meta:cell-count="9"
//	meta:object-count="0"/>
	QString meta_table_count_;
	QString meta_cell_count_;
	QString meta_object_count_;
};

} // ods::inst::
} // ods::
