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

namespace ods {
namespace inst {

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
	
	for (auto *next: cloner.nodes())
		nodes_.append(next->Clone());
}

Abstract::~Abstract()
{
	if (is_root())
		delete ns_;
	
	for (auto *next: nodes_)
		delete next;
}

bool
Abstract::AddText(StringOrTag *tot)
{
	if (tot->is_string())
	{
		Append(*tot->as_string());
		return true;
	}
	
	return false;
}

void
Abstract::Append(const QString &s)
{
	nodes_.append(new StringOrInst(s));
}

void
Abstract::Append(Abstract *a)
{
	nodes_.append(new StringOrInst(a));
}

void
Abstract::DeleteNodes()
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
	for (auto *x: nodes_)
	{
		if (x->Is(id))
			return x->as_inst();
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
		auto *n = style_name();
		
		if ((n != nullptr) && (*n == name))
			return this;
	}
	
	for (auto *x: nodes_)
	{
		if (!x->is_inst())
			continue;
		
		auto *inst = x->as_inst();
		
		if (!inst->IsStyle())
			continue;
		
		auto *n = inst->style_name();
		
		if ((n != nullptr) && (*n == name))
			return inst;
	}
	
	return nullptr;
}

inst::StyleTableCellProperties*
Abstract::GetStyleTableCellProperties(const AddIfNeeded ain)
{
	auto *p = (StyleTableCellProperties*)Get(Id::StyleTableCellProperties);
	
	if (p != nullptr || ain == AddIfNeeded::No)
		return p;
	
	p = new StyleTableCellProperties(this);
	Append(p);
	return p;
}

StyleTextProperties*
Abstract::GetStyleTextProperties(const AddIfNeeded ain)
{
	auto *p = (StyleTextProperties*)Get(Id::StyleTextProperties);
	
	if (p != nullptr || ain == AddIfNeeded::No)
		return p;
	
	p = new StyleTextProperties(this);
	Append(p);
	return p;
}

bool
Abstract::Is(const Id id1, const Id id2) const
{
	if (id_ != id1)
		return false;
	
	return (id2 == Id::None || id_ == id2);
}

bool
Abstract::IsTextP() const
{
	return id_ == Id::TextP;
}

void
Abstract::ScanString(Tag *tag)
{
	for (StringOrTag *x: tag->nodes())
	{
		if (x->is_string())
			Append(*x->as_string());
	}
}

void
Abstract::Write(QXmlStreamWriter &xml)
{
	xml.writeStartElement(FullName());
	
	if (parent_ == nullptr)
		ns_->WriteNamespaces(xml, this);
	
	WriteData(xml);
	xml.writeEndElement();
}

void
Abstract::Write(QXmlStreamWriter &xml, Abstract *a)
{
	if (a != nullptr)
		a->Write(xml);
}

void
Abstract::Write(QXmlStreamWriter &xml, QString &str)
{
	if (!str.isEmpty())
		xml.writeCharacters(str);
}

void
Abstract::Write(QXmlStreamWriter &xml, ods::Prefix *prefix,
	const char *name, const QString &value)
{
	if (!value.isEmpty())
		xml.writeAttribute(prefix->With(name), value);
}

void
Abstract::Write(QXmlStreamWriter &xml, ods::Prefix *prefix, const char *name,
	const ods::Bool value)
{
	if (value != ods::Bool::None) {
		QString s = (value == ods::Bool::True) ? ods::str::True : ods::str::False;
		xml.writeAttribute(prefix->With(name), s);
	}
}

void
Abstract::Write(QXmlStreamWriter &xml, ods::Prefix *prefix, const char *name,
	const ods::Length *value)
{
	if (value != nullptr)
		xml.writeAttribute(prefix->With(name), value->toString());
}

void
Abstract::Write(QXmlStreamWriter &xml, ods::Prefix *prefix,
	const char *name, const int num, const int except)
{
	if (num != except)
		xml.writeAttribute(prefix->With(name), QString::number(num));
}

void
Abstract::WriteNodes(QXmlStreamWriter &xml)
{
	for (ods::StringOrInst *next: nodes_)
	{
		if (next->is_string())
			xml.writeCharacters(*next->as_string());
		else
			next->as_inst()->Write(xml);
	}
}

}} // ods::inst::
