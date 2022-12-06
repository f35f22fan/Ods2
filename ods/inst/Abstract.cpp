#include "Abstract.hpp"

#include "OfficeAutomaticStyles.hpp"
#include "OfficeDocumentContent.hpp"
#include "StyleTableCellProperties.hpp"
#include "StyleTextProperties.hpp"

#include "../Book.hpp"
#include "../Length.hpp"
#include "../Ns.hpp"
#include "../ns.hxx"
#include "../ods.hh"
#include "../Prefix.hpp"
#include "../Tag.hpp"

namespace ods::inst {

void AddKeywords(const QVector<QString> &words, Keywords &list)
{
	for (const QString &word: words)
	{
		if (list.contains(word)) {
			list[word].count++;
		} else {
			ci32 id = list.count() + 1;
			IdAndCount ac = { .count = 1, .id = id };
			list.insert(word, ac);
		}
	}
}

Abstract::Abstract(Abstract *parent, ods::Ns *ns, ods::id::func f) : parent_(parent)
{
	if (parent_)
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
	for (auto *node: nodes_)
		delete node;
}

bool Abstract::AddText(StringOrTag *sot)
{
	if (sot->is_string())
	{
		QString s = *sot->as_string();
		mtl_printq(s);
		Append(s);
		return true;
	}
	
	return false;
}

void Abstract::Append(QStringView s)
{
	if (!s.isEmpty())
		nodes_.append(new StringOrInst(s));
}

void Abstract::Append(Abstract *a, const TakeOwnership to)
{
	nodes_.append(new StringOrInst(a, to));// TakeOwnership::Yes));
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
				Append(node->as_inst()->Clone(), TakeOwnership::Yes);
		}
	}
}

void Abstract::CopyAttr(NdffAttrs &attrs,
	ods::Prefix *prefix, QStringView attr_name, QString &result)
{
	result.clear(); // must clear first, so that on multiple reuse of the
	// save variable one can see if nothing was found.
	
	if (!attrs.contains(prefix->id()))
		return;
	
	QVector<ndff::Property> &props = attrs[prefix->id()];
	
	for (const ndff::Property &prop: props)
	{
		if (prop.name == attr_name)
		{
			result = prop.value;
			break;
		}
	}
}

void Abstract::CopyAttr(NdffAttrs &attrs,
	ods::Prefix *prefix, QStringView attr_name, ods::Bool &result)
{
	QString s;
	CopyAttr(attrs, prefix, attr_name, s);
	ods::ApplyBool(s, result);
}

void Abstract::CopyAttr(NdffAttrs &attrs,
	ods::Prefix *prefix, QStringView attr_name, ods::Length **size)
{
	QString s;
	CopyAttr(attrs, prefix, attr_name, s);
	ods::Length *l = ods::Length::FromString(s);
	if (l)
		*size = l;
}

void Abstract::CopyAttrI8(QHash<UriId, QVector<ndff::Property> > &attrs,
	Prefix *prefix, QStringView attr_name, i8 &result)
{
	QString s;
	CopyAttr(attrs, prefix, attr_name, s);
	
	if (!s.isEmpty())
	{
		bool ok;
		ci16 k = s.toShort(&ok);
		if (ok)
			result = k;
	}
}

void Abstract::CopyAttrI32(QHash<UriId, QVector<ndff::Property> > &attrs,
	Prefix *prefix, QStringView attr_name, i32 &result)
{
	QString s;
	CopyAttr(attrs, prefix, attr_name, s);
	
	if (!s.isEmpty())
	{
		bool ok;
		ci32 k = s.toInt(&ok);
		if (ok)
			result = k;
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

const QString* Abstract::GetString() const
{
	for (StringOrInst *node: nodes_)
	{
		if (node->is_string())
			return node->as_str_ptr();
	}
	
	return nullptr;
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

void Abstract::ReadStrings(ndff::Container *cntr)
{
	using Op = ndff::Op;
	ndff::Property prop;
	Op op = cntr->Next(prop, Op::TS);
	ReadStrings(cntr, op);
}

void Abstract::ReadStrings(ndff::Container *cntr, ndff::Op op)
{
	using Op = ndff::Op;
	if (op == Op::N32_TE)
		return;
	
	ndff::Property prop;
	if (op == Op::TCF_CMS)
		op = cntr->Next(prop, op);
	
	while (true)
	{
		if (op == Op::TS)
		{
			mtl_trace();
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

void Abstract::ReadStrings(Tag *tag)
{
	for (StringOrTag *x: tag->nodes())
	{
		if (x->is_string())
			Append(*x->as_string());
	}
}

void Abstract::SetString(QStringView s, const ClearTheRest ctr)
{
	for (StringOrInst *node: nodes_)
	{
		if (ctr == ClearTheRest::Yes)
		{
			delete node;
		} else if (node->is_string()) {
			node->SetString(s);
			return;
		}
	}
	
	if (ctr == ClearTheRest::Yes)
		nodes_.clear();
	
	Append(s);
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
		QString s = (value == Bool::True) ? ns::True : ns::False;
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
	ByteArray &ba, Prefix *prefix, QString key, QStringView value)
{
	if (value.isEmpty())
		return;
	
	ba.add_u8(ndff::Op::S32_PS);
	ba.add_unum(prefix->id());
	ci32 string_id = kw[key].id;
	ba.add_inum(string_id);
	ba.add_string(value, Pack::NDFF);
}

void Abstract::WriteNdffProp(inst::Keywords &kw, ByteArray &ba,
	ods::Prefix *prefix, QString name, const ods::Bool value)
{
	if (value != Bool::None)
	{
		QString s = (value == Bool::True) ? ns::True : ns::False;
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

void Abstract::WriteNodes(NsHash &h, Keywords &kw, ByteArray &ba, const PrintText pt)
{
	QVector<StringOrInst*> vec;
	ListChildren(vec, Recursively::No);
	bool found = false;
	for (StringOrInst *soi: vec)
	{
		if (soi->is_inst()) {
			soi->as_inst()->WriteNDFF(h, kw, nullptr, &ba);
		} else {
			found = true;
			QString s = soi->as_string();
			if (pt == PrintText::Yes) {
				auto ba = s.toLocal8Bit();
				mtl_info("=============\"%s\"", ba.data());
			}
			ba.add_string(s, Pack::NDFF);
		}
	}
	
	if (pt == PrintText::Yes && !found)
		mtl_info("=============");
}

void Abstract::WriteTag(Keywords &kw, ByteArray &ba)
{
	ci32 name_id = kw.value(tag_name(), {0, 0}).id;
	if (name_id == 0)
	{
		mtl_warn("No id for tag \"%s\"", qPrintable(tag_name()));
		return;
	}
	//mtl_info("tag: \"%s\", id: %d", qPrintable(tag_name()), name_id);
	ba.add_u8(ndff::Op::TS);
	ba.add_unum(prefix_->id());
	ba.add_inum(name_id);
}

} // ods::inst::
