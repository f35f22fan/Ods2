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

bool Prefix::Is(QStringView name) const
{
	return name_ == name;
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

Prefix* Prefix::Create(UriId &id, const QString &name, const QString &uri)
{
	return new Prefix(id, name, uri);
}

} // ods::
