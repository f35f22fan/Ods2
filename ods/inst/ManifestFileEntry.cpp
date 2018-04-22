#include "ManifestFileEntry.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

ManifestFileEntry::ManifestFileEntry(Abstract *parent, Tag *tag)
: Abstract(parent, parent->ns(), id::ManifestFileEntry)
{
	if (tag != nullptr)
		Init(tag);
}

ManifestFileEntry::ManifestFileEntry(const ManifestFileEntry &cloner)
: Abstract(cloner)
{}

ManifestFileEntry::~ManifestFileEntry() {}

Abstract*
ManifestFileEntry::Clone(Abstract *parent) const
{
	auto *p = new ManifestFileEntry(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->manifest_full_path_ = manifest_full_path_;
	p->manifest_media_type_ = manifest_media_type_;
	p->manifest_version_ = manifest_version_;
	
	return p;
}

void
ManifestFileEntry::Init(Tag *tag)
{
	tag->Copy(ns_->manifest(), ods::ns::kFullPath, manifest_full_path_);
	tag->Copy(ns_->manifest(), ods::ns::kMediaType, manifest_media_type_);
	tag->Copy(ns_->manifest(), ods::ns::kVersion, manifest_version_);
}

void
ManifestFileEntry::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->manifest(), ods::ns::kFullPath, manifest_full_path_);
	Write(xml, ns_->manifest(), ods::ns::kMediaType, manifest_media_type_);
	Write(xml, ns_->manifest(), ods::ns::kVersion, manifest_version_);
}

} // ods::inst::
} // ods::
