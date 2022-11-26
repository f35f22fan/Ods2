#include "ManifestFileEntry.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

#include "../ndff/Container.hpp"
#include "../ndff/Property.hpp"

namespace ods::inst {

ManifestFileEntry::ManifestFileEntry(Abstract *parent, Tag *tag, ndff::Container *cntr)
: Abstract(parent, parent->ns(), id::ManifestFileEntry)
{
	if (cntr)
		Init(cntr);
	else if (tag)
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

void ManifestFileEntry::Init(ndff::Container *cntr)
{
	using Op = ndff::Op;
	ndff::Property prop;
	QHash<UriId, QVector<ndff::Property>> attrs;
	Op op = cntr->Next(prop, Op::TS, &attrs);
	CopyAttr(attrs, ns_->manifest(), ns::kFullPath, manifest_full_path_);
	CopyAttr(attrs, ns_->manifest(), ns::kMediaType, manifest_media_type_);
	CopyAttr(attrs, ns_->manifest(), ns::kVersion, manifest_version_);
	ReadStrings(cntr, op);
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

void ManifestFileEntry::WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba)
{
	CHECK_TRUE_VOID(ba != nullptr);
	WriteTag(kw, *ba);
	WriteNdffProp(kw, *ba, ns_->manifest(), ns::kFullPath, manifest_full_path_);
	WriteNdffProp(kw, *ba, ns_->manifest(), ns::kMediaType, manifest_media_type_);
	WriteNdffProp(kw, *ba, ns_->manifest(), ns::kVersion, manifest_version_);
	CloseBasedOnChildren(h, kw, file, ba);
}

} // ods::inst::
