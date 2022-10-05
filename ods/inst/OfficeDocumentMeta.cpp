#include "OfficeDocumentMeta.hpp"

#include "OfficeMeta.hpp"

#include "../Book.hpp"
#include "../filename.hxx"
#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Sheet.hpp"
#include "../Tag.hpp"

namespace ods::inst {

OfficeDocumentMeta::OfficeDocumentMeta(ods::Book *book, ods::Ns *ns, ods::Tag *tag)
: Abstract(nullptr, ns, id::OfficeDocumentMeta)
{
	book_ = book;
	book_->document_meta_ = this;
	
	if (tag != nullptr)
		Init(tag);
	else
		InitDefault();
}

OfficeDocumentMeta::OfficeDocumentMeta(const OfficeDocumentMeta &cloner)
: Abstract(cloner)
{}

OfficeDocumentMeta::~OfficeDocumentMeta()
{}

Abstract*
OfficeDocumentMeta::Clone(Abstract *parent) const
{
	auto *p = new OfficeDocumentMeta(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->office_version_ = office_version_;
	return p;
}

void OfficeDocumentMeta::Init(Tag *tag)
{
	tag->Copy(ns_->office(), ods::ns::kVersion, office_version_);
	Scan(tag);
}

void OfficeDocumentMeta::InitDefault()
{
	Append(new OfficeMeta(this));
	office_version_ = QLatin1String("1.2");
}

void OfficeDocumentMeta::ListKeywords(Keywords &list, const LimitTo lt)
{
	AddKeywords({tag_name(), ns::kVersion}, list);
}

void OfficeDocumentMeta::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->office(), list);
}

void OfficeDocumentMeta::Scan(Tag *tag)
{
	for (auto *x: tag->nodes())
	{
		if (!x->is_tag())
			continue;
		
		auto *next = x->as_tag();
		
		if (next->Is(ns_->office(), ods::ns::kMeta))
		{
			Append(new OfficeMeta(this, next));
		} else {
			Scan(next);
		}
	}
}

void OfficeDocumentMeta::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->office(), ods::ns::kVersion, office_version_);
	WriteNodes(xml);
}

void OfficeDocumentMeta::WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba)
{
	CHECK_TRUE_VOID(ba != nullptr);
	WriteTag(kw, *ba);
	WriteNdffProp(kw, *ba, ns_->office(), ods::ns::kVersion, office_version_);
	CloseBasedOnChildren(h, kw, file, ba);
}

} // ods::inst::
