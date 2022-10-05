#pragma once

#include "Abstract.hpp"
#include "decl.hxx"

#include "../decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API OfficeDocumentMeta : public Abstract
{
public:
	OfficeDocumentMeta(ods::Book *book, ods::Ns *ns, Tag *tag = nullptr);
	OfficeDocumentMeta(const OfficeDocumentMeta &cloner);
	virtual ~OfficeDocumentMeta();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	bool Save(const QString &dir_path);
	void WriteData(QXmlStreamWriter &xml) override;
	void WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba) override;
private:
	
	void Init(ods::Tag *tag);
	void InitDefault();
	void Scan(ods::Tag *tag);
	
	QString office_version_;
};

} // ods::inst::
