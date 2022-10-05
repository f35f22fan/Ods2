#include "ManifestManifest.hpp"

#include "ManifestFileEntry.hpp"

#include "../Book.hpp"
#include "../filename.hxx"
#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods::inst {

ManifestManifest::ManifestManifest(ods::Book *book, Ns *ns, Tag *tag)
: Abstract(nullptr, ns, id::ManifestManifest)
{
	book_ = book;
	book_->manifest_ = this;
	
	if (tag != nullptr)
		Init(tag);
	else
		InitDefault();
}

ManifestManifest::ManifestManifest(const ManifestManifest &cloner)
: Abstract(cloner)
{
	delete ns_;
}

ManifestManifest::~ManifestManifest() {}

ManifestFileEntry*
ManifestManifest::AddEntry(const QString &path)
{
	auto *p = new ManifestFileEntry(this);
	p->full_path(path);
	Append(p);
	return p;
}

ManifestFileEntry*
ManifestManifest::AddEntry2(const QString &path, const QString &media_type)
{
	auto *p = new ManifestFileEntry(this);
	p->full_path(path);
	p->media_type(media_type);
	Append(p);

	return p;
}

Abstract*
ManifestManifest::Clone(Abstract *parent) const
{
	auto *p = new ManifestManifest(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	return p;
}

void ManifestManifest::Init(Tag *tag)
{
	tag->Copy(ns_->manifest(), ns::kVersion, manifest_version_);
	Scan(tag);
}

void ManifestManifest::InitDefault()
{
	manifest_version_ = QLatin1String("1.2");
/*
	<manifest:manifest (...) manifest:version="1.2">
	<manifest:file-entry manifest:full-path="/"
		manifest:media-type="application/vnd.oasis.opendocument.spreadsheet"
		manifest:version="1.2"/>
	<manifest:file-entry manifest:full-path="Thumbnails/thumbnail.png"
		manifest:media-type="image/png"/>
	<manifest:file-entry manifest:full-path="Configurations2/"
		manifest:media-type="application/vnd.sun.xml.ui.configuration"/>
	<manifest:file-entry manifest:full-path="content.xml"
		manifest:media-type="text/xml"/>
	<manifest:file-entry manifest:full-path="meta.xml"
		manifest:media-type="text/xml"/>
	<manifest:file-entry manifest:full-path="styles.xml"
		manifest:media-type="text/xml"/>
	<manifest:file-entry manifest:full-path="manifest.rdf"
		manifest:media-type="application/rdf+xml"/>
	<manifest:file-entry manifest:full-path="settings.xml"
		manifest:media-type="text/xml"/>
	</manifest:manifest>
*/
	
	auto *p = AddEntry2("/", "application/vnd.oasis.opendocument.spreadsheet");
	p->version(manifest_version_);
	AddEntry2(ods::filename::ContentXml, "text/xml");
	AddEntry2(ods::filename::MetaXml, "text/xml");
	AddEntry2(ods::filename::StylesXml, "text/xml");
	
	// Not needed by default nor implemented:
	// Add(ods::filename::SettingsXml, "text/xml");
}

void ManifestManifest::ListUsedNamespaces(NsHash &list)
{
	inst::Add(ns_->manifest(), list);
}

void ManifestManifest::ListKeywords(Keywords &list, const LimitTo lt)
{
	AddKeywords({tag_name(), ns::kFileEntry, ns::kVersion}, list);
}

void ManifestManifest::Scan(Tag *tag)
{
	for (auto *x: tag->nodes())
	{
		if (AddText(x))
			continue;
		
		auto *next = x->as_tag();
		
		if (next->Is(ns_->manifest(), ns::kFileEntry)) {
			Append(new ManifestFileEntry(this, next));
		} else {
			Scan(next);
		}
	}
}

void ManifestManifest::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->manifest(), ods::ns::kVersion, manifest_version_);
	
	WriteNodes(xml);
}

void ManifestManifest::WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba)
{
	CHECK_TRUE_VOID(ba != nullptr);
	WriteTag(kw, *ba);
	WriteNdffProp(kw, *ba, ns_->manifest(), ods::ns::kVersion, manifest_version_);
	CloseBasedOnChildren(h, kw, file, ba);
}

} // ods::inst::
