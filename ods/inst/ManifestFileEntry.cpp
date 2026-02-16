#include "ManifestFileEntry.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods::inst {

ManifestFileEntry::ManifestFileEntry(Abstract *parent, Tag *tag)
: Abstract(parent, parent->ns(), id::ManifestFileEntry)
{
	if (tag)
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

void ManifestFileEntry::Init(Tag *tag)
{
	tag->Copy(ns_->manifest(), ns::kFullPath, manifest_full_path_);
	tag->Copy(ns_->manifest(), ns::kMediaType, manifest_media_type_);
	tag->Copy(ns_->manifest(), ns::kVersion, manifest_version_);
}

void ManifestFileEntry::ListKeywords(Keywords &list, const LimitTo lt)
{
	AddKeywords({tag_name(), ns::kFullPath,
		ns::kMediaType, ns::kVersion}, list);
}

void ManifestFileEntry::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->manifest(), list);
}

void ManifestFileEntry::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->manifest(), ns::kFullPath, manifest_full_path_);
	Write(xml, ns_->manifest(), ns::kMediaType, manifest_media_type_);
	Write(xml, ns_->manifest(), ns::kVersion, manifest_version_);
}

} // ods::inst::
