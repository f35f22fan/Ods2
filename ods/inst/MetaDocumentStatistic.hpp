#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API MetaDocumentStatistic : public Abstract
{
public:
	MetaDocumentStatistic(Abstract *parent, Tag *tag = 0, ndff::Container *cntr = 0);
	MetaDocumentStatistic(const MetaDocumentStatistic &cloner);
	virtual ~MetaDocumentStatistic();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	void WriteData(QXmlStreamWriter &xml) override;
	void WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba) override;
private:
	void Init(ndff::Container *cntr);
	void Init(Tag *tag);
	
//<meta:document-statistic meta:table-count="1" meta:cell-count="9"
//	meta:object-count="0"/>
	QString meta_table_count_;
	QString meta_cell_count_;
	QString meta_object_count_;
};

} // ods::inst::
