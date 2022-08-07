#include "Prefix.hpp"

namespace ods { // ods::

Prefix::Prefix(UriId &id, const QString &prefix, const QString &uri) :
	id_(id),
	name_(prefix),
	uri_(uri)
{}

Prefix::~Prefix() {}

bool
Prefix::Is(const Prefix *prefix) const
{
	return id_ == prefix->id();
}

bool
Prefix::Is(QStringView s) const
{
	return name_ == s;
}

QString
Prefix::With(const char *name) const
{
	return name_ + QChar(':') + name;
}

QString
Prefix::With(const QString &name) const
{
	return name_ + QChar(':') + name;
}

void
Prefix::Write(QXmlStreamWriter &xml) const
{
	if (!name_.isEmpty())
		xml.writeNamespace(uri_, name_);
}

Prefix*
Prefix::Create(UriId &id, const QString &s, const QString &uri)
{
	return new Prefix(id, s, uri);
}

} // ods::
