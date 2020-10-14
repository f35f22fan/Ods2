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
	
	for (auto *a: attrs_)
		delete a;
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

void
Tag::Copy(ods::Prefix *prefix, const char *name, ods::Bool &into)
{
	QString s;
	Copy(prefix, name, s);
	ods::ApplyBool(s, into);
}

void
Tag::Copy(ods::Prefix *prefix, const char *name, ods::Length **size)
{
	QString value;
	Copy(prefix, name, value);
	ods::Length *l = ods::Length::FromString(value);
	
	if (l != nullptr)
		*size = l;
}

void
Tag::Copy(ods::Prefix *prefix, const char *name, QString &into)
{
	auto *a = Get(prefix, name);

	if (a != nullptr)
		into = a->value();
	else
		into.clear();
}

void
Tag::Copy(ods::Prefix *prefix, const char *name, int32_t &into)
{
	auto *a = Get(prefix, name);
	
	if (a != nullptr)
		a->ToInt32(into);
}

void
Tag::Copy(ods::Prefix *prefix, const char *name, int8_t &into)
{
	auto *a = Get(prefix, name);
	
	if (a != nullptr)
	{
		QString v = a->value();
		bool ok;
		auto n = v.toShort(&ok);
		
		if (ok)
			into = n;
		
	}
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
Tag::Get(ods::Prefix *prefix, const char *name)
{
	foreach (auto *attr, attrs_)
	{
		if (attr->Is(prefix, name))
			return attr;
	}
	
	return nullptr;
}

bool
Tag::Has(const ods::Prefix *prefix) const
{
	return prefix_->id() == prefix->id();
}

bool
Tag::Is(const ods::Prefix *prefix, const QString &name) const
{
	return prefix_->id() == prefix->id() && name_ == name;
}

bool
Tag::IsAnyCell() const
{
	if (prefix_->id() != ns_->table()->id())
		return false;
	
	return (name_ == ods::ns::kTableCell) ||
		(name_ == ods::ns::kCoveredTableCell);
}

bool
Tag::IsTextP() const
{
	return Is(ns_->text(), ods::ns::kP);
}

void
Tag::SetAttributes(const QXmlStreamAttributes &attrs)
{
	for (auto &attr: attrs)
	{
		auto *prefix = ns_->GetPrefix(attr.prefix().toString());
		
		// stuff like "calcext" not supported, will return nullptr
		if (prefix == nullptr)
			continue;
		
		auto *p = new ods::Attr(prefix, attr.name().toString(),
			attr.value().toString());
		attrs_.append(p);
	}
}

} // ods::
