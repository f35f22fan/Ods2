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

void AddKeywords(const QVector<QString> &words, Keywords &words_hash)
{
	for (const QString &word: words)
	{
		if (words_hash.contains(word)) {
			words_hash[word].count++;
		} else {
			ci32 id = words_hash.count() + 1;
			IdAndCount ac;
			ac.count = 1;
			ac.id = id;
			words_hash.insert(word, ac);
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
	nodes_.clear();
}

bool Abstract::AddText(StringOrTag *sot)
{
	if (sot->is_string())
	{
		QString s = *sot->as_string();
		//mtl_printq(s);
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
		
		if (sn && (*sn == name))
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

void Abstract::ListChildren(QVector<StringOrInst*> &output,
	const Recursively r)
{
	for (StringOrInst *node: nodes_)
	{
		output.append(node);
		
		if (r == Recursively::Yes && node->is_inst())
		{
			node->as_inst()->ListChildren(output, r);
		}
	}
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

} // ods::inst::
