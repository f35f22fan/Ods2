#include "OfficeDocumentContent.hpp"

#include "OfficeAutomaticStyles.hpp"
#include "OfficeBody.hpp"
#include "OfficeFontFaceDecls.hpp"
#include "OfficeScripts.hpp"

#include "../Book.hpp"
#include "../ByteArray.hpp"
#include "../filename.hxx"
#include "../ndff/Container.hpp"
#include "../Ns.hpp"
#include "../ns.hxx"
#include "../ods.hh"
#include "../Sheet.hpp"
#include "../Tag.hpp"

namespace ods::inst {

OfficeDocumentContent::OfficeDocumentContent
(ods::Book *book, ods::Ns *ns, ods::Tag *tag)
: Abstract(nullptr, ns, id::OfficeDocumentContent)
{
	book_ = book;
	book_->document_content_ = this;
	
	if (tag != nullptr)
		Init(tag);
	else
		InitDefault();
}

OfficeDocumentContent::OfficeDocumentContent(const OfficeDocumentContent &cloner)
: Abstract(cloner)
{}

OfficeDocumentContent::~OfficeDocumentContent()
{
	delete office_automatic_styles_;
	delete office_body_;
	delete office_font_face_decls_;
	delete office_scripts_;
}

OfficeDocumentContent* OfficeDocumentContent::From(Ns *ns,
	ndff::Container *cntr)
{
	if (false) {
	enum Op_: u8 { // Only up to 4 bits can be used!
		F8 = 0, // separate to be able to store an f64 with no loss
		S1 = 1, // string inline (0 - 15)
		S2 = 2, // string inline (0 - 4095)
		S4_PS = 3, // string inline (0 – 268435455), PS=PropertyStart
		S8 = 4, // string inline (60 bits for size)
		U1 = 5, // inline (0 - 15)
		U2 = 6, // inline (0 - 4095)
		U4_TE = 7, // inline (0 - 268435455) TE=TagEnd "/>"
		U8 = 8, // separate, to be able to store max value
		TS = 9, // TagStart "<"
		TCF_CMS = 10, // TagContentsFollow ">", CMS=Compressed
		SCT = 11, // SeparateClosingTag "</tag name>"
		FS1 = 12, // FreeSpace1
		FS8 = 13,
		JB4 = 14, // JumpBy4 is an unsigned number
		JB8 = 15, // JumpBy8 is unsigned
	};}
	auto &ba = cntr->buf();
	auto &kw = cntr->keywords(); // using Keywords = QHash<QStringView, IdAndCount>;
	cu8 b1 = ba.next_u8();
	using ndff::Op;
	cu8 op = b1 & 0xFu;
	if (op == Op::TS) {
		mtl_info("Tag start");
		UriId uri_id = ba.next_unum();
		if (uri_id == ns->office()->id()) {
			mtl_info("It's the 'office' id!");
		} else {
			mtl_info("Not office id...");
		}
		ci32 tag_id = ba.next_unum();
		mtl_info("prefix id: %d, tag_id: %d", (i32)uri_id, tag_id);
		QString tag_name;
		if (cntr->GetKey(tag_id, tag_name)) {
			auto b = tag_name.toLocal8Bit();
			mtl_info("tag name: \"%s\"", b.data());
		} else {
			mtl_warn("Couldn't find tag name by id %d", tag_id);
		}
	} else {
		mtl_info("Other: %d", (i32)op);
		//if (b1 == Op::)
	}
	return nullptr;
}

Abstract*
OfficeDocumentContent::Clone(Abstract *parent) const
{
	auto *p = new OfficeDocumentContent(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	if (office_automatic_styles_ != nullptr)
	{
		p->office_automatic_styles_ =
			(OfficeAutomaticStyles*)office_automatic_styles_->Clone();
	}
	
	if (office_body_ != nullptr)
	{
		p->office_body_ = (OfficeBody*)office_body_->Clone();
	}
	
	if (office_font_face_decls_ != nullptr)
	{
		p->office_font_face_decls_ = (OfficeFontFaceDecls*)
			office_font_face_decls_->Clone();
	}
	
	if (office_scripts_ != nullptr)
	{
		p->office_scripts_ = (OfficeScripts*) office_scripts_->Clone();
	}
	
	return p;
}

Abstract*
OfficeDocumentContent::GetAnyStyle(const QString &name)
{
	auto *a = office_automatic_styles_->GetStyleRecursive(name);
	
	if (a != nullptr)
		return a;
	
	return office_font_face_decls_->GetStyleRecursive(name);
}

void OfficeDocumentContent::Init(ods::Tag *tag)
{
	tag->Copy(ns_->office(), ods::ns::kVersion, office_version_);
	Scan(tag);
}

void OfficeDocumentContent::InitDefault()
{
	office_scripts_ = new OfficeScripts(this);
	office_body_ = new OfficeBody(this);
	office_automatic_styles_ = new OfficeAutomaticStyles(this);
	office_font_face_decls_ = new OfficeFontFaceDecls(this);
	
	office_version_ = QLatin1String("1.2");
}

void OfficeDocumentContent::ListChildren(QVector<StringOrInst*> &vec,
	const Recursively r)
{
	if (office_scripts_)
	{
		vec.append(new StringOrInst(office_scripts_, TakeOwnership::No));
		if (r == Recursively::Yes)
			office_scripts_->ListChildren(vec, r);
	}
	
	if (office_font_face_decls_)
	{
		vec.append(new StringOrInst(office_font_face_decls_, TakeOwnership::No));
		if (r == Recursively::Yes)
			office_font_face_decls_->ListChildren(vec, r);
	}
	
	if (office_automatic_styles_)
	{
		vec.append(new StringOrInst(office_automatic_styles_, TakeOwnership::No));
		if (r == Recursively::Yes)
			office_automatic_styles_->ListChildren(vec, r);
	}
	
	if (office_body_)
	{
		vec.append(new StringOrInst(office_body_, TakeOwnership::No));
		if (r == Recursively::Yes)
			office_body_->ListChildren(vec, r);
	}
}

void OfficeDocumentContent::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->office(), list);
}

void OfficeDocumentContent::ListKeywords(Keywords &list, const LimitTo lt)
{
	AddKeywords({tag_name(), ns::kVersion}, list);
}

void OfficeDocumentContent::Scan(ods::Tag *tag)
{
	for (auto *node: tag->nodes())
	{
		if (!node->is_tag())
			continue;
		
		bool scan = false;
		ods::Tag *subtag = node->as_tag();
		if (subtag->Has(ns_->office()))
		{
			if (subtag->Has(ods::ns::kAutomaticStyles)) {
				office_automatic_styles_ = new OfficeAutomaticStyles(this, subtag);
			} else if (subtag->Has(ods::ns::kBody)) {
				office_body_ = new OfficeBody(this, subtag);
			} else if (subtag->Has(ods::ns::kFontFaceDecls)) {
				office_font_face_decls_ = new OfficeFontFaceDecls(this, subtag);
			} else if (subtag->Has(ods::ns::kScripts)) {
				office_scripts_ = new OfficeScripts(this, subtag);
			} else {
				scan = true;
			}
		} else {
			scan = true;
		}
		
		if (scan)
			Scan(subtag);
	}
}

void OfficeDocumentContent::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->office(), ods::ns::kVersion, office_version_);
	office_scripts_->Write(xml);
	office_font_face_decls_->Write(xml);
	office_automatic_styles_->Write(xml);
	office_body_->Write(xml);
}

void OfficeDocumentContent::WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba)
{
	CHECK_TRUE_VOID(ba != nullptr);
	WriteTag(kw, *ba);
	WriteNdffProp(kw, *ba, ns_->office(), ns::kVersion, office_version_);
	CloseBasedOnChildren(h, kw, file, ba);
}

} // ods::inst::
