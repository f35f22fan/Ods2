#include "Prefix.hpp"

namespace ods { // ods::

Prefix::Prefix(UriId &id, const QString &prefix, const QString &uri) :
	id_(id),
	name_(prefix),
	uri_(uri)
{}

Prefix::~Prefix() {}

bool Prefix::Is(const Prefix *prefix) const
{
	return id_ == prefix->id();
}

bool Prefix::Is(QStringView s) const
{
	return name_ == s;
}

QString Prefix::With(QStringView name) const
{
	return QString(name_).append(QChar(':')).append(name);
}

void Prefix::Write(QXmlStreamWriter &xml) const
{
	if (!name_.isEmpty())
		xml.writeNamespace(uri_, name_);
}

Prefix* Prefix::Create(UriId &id, const QString &s, const QString &uri)
{
	return new Prefix(id, s, uri);
}

} // ods::
