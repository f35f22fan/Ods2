#pragma once

#include "Abstract.hpp"
#include "decl.hxx"

#include "../decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API ManifestManifest : public Abstract
{
public:
	ManifestManifest(ods::Book *book, ods::Ns *ns, Tag *tag = 0, ndff::Container *cntr = 0);
	ManifestManifest(const ManifestManifest &cloner);
	
	virtual ~ManifestManifest();
	
	ManifestFileEntry* AddEntry(const QString &path);
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	void WriteData(QXmlStreamWriter &xml) override;
	void WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba) override;
private:
	
	ManifestFileEntry* AddEntry2(const QString &path, const QString &media_type);
	void Init(ndff::Container *cntr);
	void Init(Tag *tag);
	void InitDefault();
	void Scan(Tag *tag);
	
	QString manifest_version_;
};

} // ods::inst::
