#pragma once

#include "Abstract.hpp"
#include "decl.hxx"

#include "../decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API ManifestManifest : public Abstract
{
public:
	ManifestManifest(ods::Book *book, ods::Ns *ns, Tag *tag = 0);
	ManifestManifest(const ManifestManifest &cloner);
	
	virtual ~ManifestManifest();
	
	ManifestFileEntry* AddEntry(QString path, QString media_type);
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	void WriteData(QXmlStreamWriter &xml) override;
private:
	
	ManifestFileEntry* AddEntry2(const QString &path, const QString &media_type);
	void Init(Tag *tag);
	void InitDefault();
	void Scan(Tag *tag);
	
	QString manifest_version_;
	QList<ManifestFileEntry*> file_entries_;
};

} // ods::inst::
