#include "OfficeDocumentMeta.hpp"

#include "OfficeMeta.hpp"

#include "../Book.hpp"
#include "../filename.hxx"
#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Sheet.hpp"
#include "../Tag.hpp"

namespace ods::inst {

OfficeDocumentMeta::OfficeDocumentMeta(ods::Book *book, ods::Ns *ns, ods::Tag *tag, ndff::Container *cntr)
: Abstract(nullptr, ns, id::OfficeDocumentMeta)
{
	book_ = book;
	book_->document_meta_ = this;
	
	if (cntr)
		Init(cntr);
	else if (tag)
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

void OfficeDocumentMeta::Init(ndff::Container *cntr)
{
	using Op = ndff::Op;
	ndff::Property prop;
	Op op = cntr->Next(prop, Op::None);
	NdffAttrs attrs;
	op = cntr->Next(prop, op, &attrs);
	CopyAttr(attrs, ns_->office(), ns::kVersion, office_version_);
	mtl_info("office_version_: %s", qPrintable(office_version_));
	if (op == Op::N32_TE)
		return;
	
	if (op == Op::TCF_CMS)
		op = cntr->Next(prop, op);
	
	while (true)
	{
		if (op == Op::TS)
		{
			if (prop.is(ns_->office()))
			{
				if (prop.name == ns::kMeta)
					Append(new OfficeMeta(this, 0, cntr), TakeOwnership::Yes);
			}
		} else if (ndff::is_text(op)) {
			Append(cntr->NextString());
		} else {
			break;
		}
		op = cntr->Next(prop, op);
	}
	
	if (op != Op::SCT)
		mtl_trace("Unexpected op: %d", op);
}

void OfficeDocumentMeta::Init(Tag *tag)
{
	tag->Copy(ns_->office(), ns::kVersion, office_version_);
	Scan(tag);
}

void OfficeDocumentMeta::InitDefault()
{
	Append(new OfficeMeta(this), TakeOwnership::Yes);
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
		if (next->Is(ns_->office(), ns::kMeta))
		{
			Append(new OfficeMeta(this, next), TakeOwnership::Yes);
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
