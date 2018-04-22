#include "Prefix.hpp"

namespace ods { // ods::

Prefix::Prefix(const UriId id, const QString &prefix, const QString &uri) :
	id_(id),
	str_(prefix),
	uri_(uri)
{}

Prefix::~Prefix() {}

bool
Prefix::Is(const Prefix *prefix) const
{
	return id_ == prefix->id();
}

bool
Prefix::Is(const QString &s) const
{
	return str_ == s;
}

QString
Prefix::With(const char *name) const
{
	return str_ + QChar(':') + name;
}

QString
Prefix::With(const QString &name) const
{
	return str_ + QChar(':') + name;
}

void
Prefix::Write(QXmlStreamWriter &xml) const
{
	if (!str_.isEmpty())
		xml.writeNamespace(uri_, str_);
}

Prefix*
Prefix::Create(const ods::UriId id, const QString &s, const QString &uri)
{
	return new Prefix(id, s, uri);
}

} // ods::
