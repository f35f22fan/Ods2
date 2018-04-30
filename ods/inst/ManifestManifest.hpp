#pragma once

#include "Abstract.hpp"
#include "decl.hxx"

#include "../decl.hxx"
#include "../err.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

class ODS_API ManifestManifest : public Abstract
{
public:
	ManifestManifest(ods::Book *book, ods::Ns *ns, Tag *tag = nullptr);
	ManifestManifest(const ManifestManifest &cloner);
	
	virtual ~ManifestManifest();
	
	ManifestFileEntry*
	Add(const QString &path);
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	void
	WriteData(QXmlStreamWriter &xml) override;

private:
	
	ManifestFileEntry*
	Add(const QString &path, const QString &media_type);
	
	void Init(Tag *tag);
	void InitDefault();
	void Scan(Tag *tag);
	
	QString manifest_version_;
};

} // ods::inst::
} // ods::
