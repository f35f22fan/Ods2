#include "Abstract.hpp"

#include "OfficeAutomaticStyles.hpp"
#include "OfficeDocumentContent.hpp"
#include "StyleTableCellProperties.hpp"
#include "StyleTextProperties.hpp"

#include "../Book.hpp"
#include "../Length.hpp"
#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Prefix.hpp"
#include "../str.hxx"
#include "../Tag.hpp"

namespace ods::inst {

Abstract::Abstract(Abstract *parent, ods::Ns *ns, ods::id::func f)
: parent_(parent)
{
	if (parent_ != nullptr)
		book_ = parent_->book();
	
	f(ns, this);
}

Abstract::Abstract(const Abstract &cloner)
: parent_(cloner.parent())
{
	(cloner.func())(cloner.ns(), this);
	
	for (auto next: cloner.nodes_)
		nodes_.append(next);
}

Abstract::~Abstract()
{
	if (is_root())
		delete ns_;
	
	for (auto *node: nodes_)
		delete node;
}

bool Abstract::AddText(StringOrTag *tot)
{
	if (tot->is_string())
	{
		Append(*tot->as_string());
		return true;
	}
	
	return false;
}

void Abstract::Append(const QString &s)
{
	nodes_.append(new StringOrInst(s));
}

void Abstract::Append(Abstract *a)
{
	nodes_.append(new StringOrInst(a, TakeOwnership::Yes));
}

bool Abstract::changed() const
{
	// Returns whether the tag has changed or if it was created
	// and not yet saved (loc_within_file = -1)
	
	return changed_properties() || changed_subnodes();
}

bool Abstract::CheckChanged(const Recursively r)
{
	if (changed())
		return true;
	
	if (r == Recursively::Yes)
	{
		QVector<StringOrInst*> vec;
		ListChildren(vec, Recursively::Yes);
		
		for (StringOrInst *item: vec)
		{
			if (item->is_inst())
			{
				Abstract *inst = item->as_inst();
				if (inst->CheckChanged(Recursively::No))
					return true;
			}
		}
	}
	
	return false;
}

void Abstract::CloneChildrenOf(const Abstract *rhs, const ClonePart co)
{
	for (StringOrInst *node: rhs->nodes_)
	{
		if (node->is_string())
		{
			if (co & ClonePart::Text)
				Append(node->as_string());
		} else {
			if (co & ClonePart::Class)
				Append(node->as_inst()->Clone());
		}
	}
}

void Abstract::DeleteNodes()
{
	for (auto *next: nodes_)
		delete next;
	
	nodes_.clear();
}

QString
Abstract::FullName() const
{
	return prefix_->With(tag_name_);
}

Abstract*
Abstract::Get(const Id id) const
{
	for (StringOrInst *node: nodes_)
	{
		if (node->Is(id))
			return node->as_inst();
	}
	
	return nullptr;
}

inst::StyleStyle*
Abstract::Get(const QString &style_name) const
{
	if (style_name.isEmpty())
		return nullptr;
	
	auto *dc = book_->document_content();
	auto *aus = dc->automatic_styles();
	
	if (aus == nullptr)
		return nullptr;
	
	return (inst::StyleStyle*)aus->ByStyleName(style_name);
}

inst::Abstract*
Abstract::GetAnyStyle(const QString &style_name) const
{
	if (style_name.isEmpty())
		return nullptr;
	
	auto *dc = book_->document_content();
	auto *aus = dc->automatic_styles();
	
	if (aus == nullptr)
	{
		mtl_warn();
		return nullptr;
	}
	
	return aus->ByStyleName(style_name);
}

Abstract*
Abstract::GetStyleRecursive(const QString &name)
{
	if (IsStyle())
	{
		auto *sn = style_name();
		
		if ((sn != nullptr) && (*sn == name))
			return this;
	}
	
	for (StringOrInst *node: nodes_)
	{
		if (!node->is_inst())
			continue;
		
		auto *inst = node->as_inst();
		if (!inst->IsStyle())
			continue;
		
		auto *n = inst->style_name();
		
		if ((n != nullptr) && (*n == name))
			return inst;
	}
	
	return nullptr;
}

bool Abstract::has_children(const IncludingText itx) const
{
	if (itx == IncludingText::Yes)
		return nodes_.size() > 0;
	
	for (StringOrInst *node: nodes_)
	{
		if (node->is_inst())
			return true;
	}
	
	return false;
}

bool Abstract::Is(const Id id1, const Id id2) const
{
	if (id_ != id1)
		return false;
	
	return (id2 == Id::None || id_ == id2);
}

void Abstract::ListChildren(QVector<StringOrInst*> &vec,
	const Recursively r)
{
	for (StringOrInst *node: nodes_)
	{
		vec.append(node);
		
		if (r == Recursively::Yes && node->is_inst())
		{
			node->as_inst()->ListChildren(vec, r);
		}
	}
}

bool Abstract::ndff() const { return book_->ndff(); }

void Abstract::ScanString(Tag *tag)
{
	for (StringOrTag *x: tag->nodes())
	{
		if (x->is_string())
			Append(*x->as_string());
	}
}

void Abstract::Write(QXmlStreamWriter &xml)
{
	xml.writeStartElement(FullName());
	
	if (parent_ == nullptr)
		ns_->WriteNamespaces(xml, this);
	
	WriteData(xml);
	xml.writeEndElement();
}

void Abstract::Write(QXmlStreamWriter &xml, Abstract *a)
{
	if (a != nullptr)
		a->Write(xml);
}

void Abstract::Write(QXmlStreamWriter &xml, QStringView str)
{
	if (!str.isEmpty())
		xml.writeCharacters(str.toString());
}

void Abstract::Write(QXmlStreamWriter &xml, ods::Prefix *prefix,
	QStringView name, QStringView value)
{
	if (!value.isEmpty())
		xml.writeAttribute(prefix->With(name), value.toString());
}

void Abstract::Write(QXmlStreamWriter &xml, ods::Prefix *prefix, QStringView name,
	const ods::Bool value)
{
	if (value != ods::Bool::None) {
		QString s = (value == ods::Bool::True) ? ods::str::True : ods::str::False;
		xml.writeAttribute(prefix->With(name), s);
	}
}

void Abstract::Write(QXmlStreamWriter &xml, ods::Prefix *prefix, QStringView name,
	const ods::Length *value)
{
	if (value != nullptr)
		xml.writeAttribute(prefix->With(name), value->toString());
}

void Abstract::Write(QXmlStreamWriter &xml, ods::Prefix *prefix,
	QStringView name, cint num, cint except)
{
	if (num != except)
		xml.writeAttribute(prefix->With(name), QString::number(num));
}

void Abstract::WriteNDFF(NsHash &h, Keywords &kw, QFileDevice *file, ByteArray *ba)
{
	CHECK_TRUE_VOID(ba != nullptr);
	WriteTag(kw, *ba);
	CloseBasedOnChildren(h, kw, file, ba);
}

void Abstract::WriteNdffProp(inst::Keywords &kw,
	ByteArray &ba, Prefix *p, QString key, QStringView value)
{
	if (value.isEmpty())
		return;
	
	ba.add_u8(ndff::Op::S4_PS);
	ba.add_u8(p->id());
	i32 id = kw[key].id;
	ba.add_unum(u32(id));
	ba.add_string(value, Pack::NDFF);
}

void Abstract::WriteNdffProp(inst::Keywords &kw, ByteArray &ba,
	ods::Prefix *prefix, QString name, const ods::Bool value)
{
	if (value != Bool::None)
	{
		QString s = (value == Bool::True) ? ods::str::True : ods::str::False;
		WriteNdffProp(kw, ba, prefix, name, s);
	}
}

void Abstract::WriteNdffProp(inst::Keywords &kw, ByteArray &ba,
	ods::Prefix *prefix, QString name, const ods::Length *value)
{
	if (value) {
		QString v = value->toString();
		WriteNdffProp(kw, ba, prefix, name, v);
	}
}

void Abstract::WriteNdffProp(inst::Keywords &kw, ByteArray &ba,
	Prefix *prefix, QString name,
	cint num, cint except)
{
	if (num != except)
		WriteNdffProp(kw, ba, prefix, name, QString::number(num));
}

void Abstract::WriteNodes(QXmlStreamWriter &xml)
{
	for (StringOrInst *node: nodes_)
	{
		if (node->is_string())
			xml.writeCharacters(node->as_string());
		else
			node->as_inst()->Write(xml);
	}
}

void Abstract::WriteNodes(NsHash &h, Keywords &kw, ByteArray &ba)
{
	QVector<StringOrInst*> vec;
	ListChildren(vec, Recursively::No);
	for (StringOrInst *sis: vec)
	{
		if (sis->is_inst()) {
			sis->as_inst()->WriteNDFF(h, kw, nullptr, &ba);
		} else {
			ba.add_string(sis->as_string(), Pack::NDFF);
		}
	}
}

void Abstract::WriteTag(Keywords &kw, ByteArray &ba)
{
	ci32 tag_id = kw.value(tag_name(), {0, 0}).id;
	if (tag_id == 0)
	{
		mtl_warn("No id for tag \"%s\"", qPrintable(tag_name()));
		return;
	}
	mtl_info("tag: \"%s\", id: %d", qPrintable(tag_name()), tag_id);
	//mtl_info("Writing tag %s", qPrintable(FullName()));
	ba.add_u8(ndff::Op::TS);
	ba.add_unum(prefix_->id());
	ba.add_unum(tag_id);
}

} // ods::inst::
