#include "Tag.hpp"

#include "Attr.hpp"
#include "inst/Abstract.hpp"
#include "Length.hpp"
#include "Ns.hpp"
#include "ns.hxx"
#include "ods.hh"
#include "StringOrTag.hpp"

namespace ods { // ods::

Tag::Tag(ods::Ns *ns, ods::Prefix *prefix, const QString &name) :
	ns_(ns), prefix_(prefix), name_(name)
{}

Tag::~Tag() {
	
	for (auto *attr: attrs_)
		delete attr;
	
	for (auto *node: nodes_)
		delete node;
}

void
Tag::Append(const QString &s)
{
	nodes_.append(new StringOrTag(s));
}

void
Tag::Append(ods::Tag *tag)
{
	nodes_.append(new StringOrTag(tag));
}

void Tag::Copy(ods::Prefix *prefix, QStringView name, ods::Bool &into)
{
	QString s;
	Copy(prefix, name, s);
	ods::ApplyBool(s, into);
}

void Tag::Copy(ods::Prefix *prefix, QStringView name, ods::Length **size)
{
	QString value;
	Copy(prefix, name, value);
	ods::Length *l = ods::Length::FromString(value);
	if (l)
		*size = l;
}

void Tag::Copy(ods::Prefix *prefix, QStringView name, QString &into)
{
	ods::Attr *attr = GetAttr(prefix, name);

	if (attr != nullptr)
		into = attr->value();
	else
		into.clear();
}

void Tag::Copy(ods::Prefix *prefix, QStringView name, i32 &into)
{
	ods::Attr *a = GetAttr(prefix, name);
	
	if (a != nullptr)
		a->ToInt32(into);
}

void Tag::Copy(ods::Prefix *prefix, QStringView name, i8 &into)
{
	ods::Attr *a = GetAttr(prefix, name);
	if (!a)
		return;
	
	QString v = a->value();
	bool ok;
	auto n = v.toShort(&ok);
	if (ok)
		into = n;
}

Tag*
Tag::From(QXmlStreamReader &xml, ods::Ns *ns)
{
	auto *prefix = ns->GetPrefix(xml.prefix().toString());
	auto *tag = new ods::Tag(ns, prefix, xml.name().toString());
	tag->SetAttributes(xml.attributes());
	
	while (!xml.atEnd() && !xml.hasError())
	{
		auto token = xml.readNext();
		
		if (token == QXmlStreamReader::Characters)
		{
			tag->Append(xml.text().toString());
		} else if (token == QXmlStreamReader::EndElement) {
			break;
		} else if (token == QXmlStreamReader::StartElement) {
			tag->Append(Tag::From(xml, ns));
		}
	}
	
	return tag;
}

QString
Tag::FullName() const
{
	return prefix_->With(name_);
}

ods::Attr*
Tag::GetAttr(ods::Prefix *prefix, QStringView name)
{
	foreach (auto *attr, attrs_)
	{
		if (attr->Is(prefix, name))
			return attr;
	}
	
	return nullptr;
}

bool Tag::Has(const ods::Prefix *prefix) const
{
	return prefix_->id() == prefix->id();
}

bool Tag::Is(const ods::Prefix *prefix, const QString &name) const
{
	return prefix_->id() == prefix->id() && name_ == name;
}

bool Tag::IsAnyCell() const
{
	if (prefix_->id() != ns_->table()->id())
		return false;
	
	return ods::IsAnyCell(name_);
}

bool Tag::IsTextP() const
{
	return Is(ns_->text(), ods::ns::kP);
}

void Tag::SetAttributes(const QXmlStreamAttributes &attrs)
{
	for (auto &attr: attrs)
	{
		auto *prefix = ns_->GetPrefix(attr.prefix().toString());
		
		if (prefix == nullptr) {
			auto ba =attr.prefix().toString().toLocal8Bit();
			mtl_trace("\"%s\"", ba.data());
			continue;
		}
		
		auto *p = new ods::Attr(prefix, attr.name().toString(),
			attr.value().toString());
		attrs_.append(p);
	}
}

} // ods::
