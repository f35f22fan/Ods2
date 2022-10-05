#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API ManifestFileEntry : public Abstract
{
public:
	ManifestFileEntry(Abstract *parent, Tag *tag = nullptr);
	ManifestFileEntry(const ManifestFileEntry &cloner);
	virtual ~ManifestFileEntry();

	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	const QString&
	full_path() const { return manifest_full_path_; }
	
	void
	full_path(const QString &s) { manifest_full_path_ = s; }
	
	void ListChildren(QVector<StringOrInst*> &vec,
		const Recursively r = Recursively::No) override {}
	
	void ListKeywords(Keywords &list, const LimitTo lt) override;
	
	void ListUsedNamespaces(NsHash &list) override;
	
	const QString& media_type() const { return manifest_media_type_; }
	void media_type(const QString &s) { manifest_media_type_ = s; }
	
	const QString& version() const { return manifest_version_; }
	void version(const QString &s) { manifest_version_ = s; }
	
	void WriteData(QXmlStreamWriter &xml) override;
	void WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba) override;
	
private:
	
	void Init(Tag *tag);
	
// <manifest:file-entry manifest:full-path="content.xml"
// manifest:media-type="text/xml"/>
	
	QString manifest_full_path_;
	QString manifest_media_type_;
	//manifest:version="1.2"
	QString manifest_version_;
};

} // ods::inst::
