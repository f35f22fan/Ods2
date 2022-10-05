#include "MetaDocumentStatistic.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods::inst {

MetaDocumentStatistic::MetaDocumentStatistic(Abstract *parent, Tag *tag)
: Abstract(parent, parent->ns(), id::MetaDocumentStatistic)
{
	if (tag != nullptr)
		Init(tag);
}

MetaDocumentStatistic::MetaDocumentStatistic(const MetaDocumentStatistic &cloner)
: Abstract(cloner)
{}

MetaDocumentStatistic::~MetaDocumentStatistic() {}

Abstract*
MetaDocumentStatistic::Clone(Abstract *parent) const
{
	auto *p = new MetaDocumentStatistic(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->meta_table_count_ = meta_table_count_;
	p->meta_cell_count_ = meta_cell_count_;
	p->meta_object_count_ = meta_object_count_;
	
	return p;
}

void MetaDocumentStatistic::Init(Tag *tag)
{
	tag->Copy(ns_->meta(), ods::ns::kTableCount, meta_table_count_);
	tag->Copy(ns_->meta(), ods::ns::kCellCount, meta_cell_count_);
	tag->Copy(ns_->meta(), ods::ns::kObjectCount, meta_object_count_);
}

void MetaDocumentStatistic::ListKeywords(inst::Keywords &list, const inst::LimitTo lt)
{
	inst::AddKeywords({tag_name(),
		ns::kTableCount, ns::kCellCount,
		ns::kObjectCount}, list);
}

void MetaDocumentStatistic::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->meta(), list);
}

void MetaDocumentStatistic::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->meta(), ods::ns::kTableCount, meta_table_count_);
	Write(xml, ns_->meta(), ods::ns::kCellCount, meta_cell_count_);
	Write(xml, ns_->meta(), ods::ns::kObjectCount, meta_object_count_);
}

void MetaDocumentStatistic::WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba)
{
	CHECK_TRUE_VOID(ba != nullptr);
	WriteTag(kw, *ba);
	WriteNdffProp(kw, *ba, ns_->meta(), ods::ns::kTableCount, meta_table_count_);
	WriteNdffProp(kw, *ba, ns_->meta(), ods::ns::kCellCount, meta_cell_count_);
	WriteNdffProp(kw, *ba, ns_->meta(), ods::ns::kObjectCount, meta_object_count_);
	CloseBasedOnChildren(h, kw, file, ba);
}

} // ods::inst::
