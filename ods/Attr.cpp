#include "Attr.hpp"

namespace ods { // ods::

Attr::Attr(Prefix *prefix, const QString &name, const QString &value)
 : prefix_(prefix), name_(name), value_(value)
{}

Attr::~Attr() {}

QString
Attr::FullName() const
{
	return prefix_->With(name_);
}

bool
Attr::Is(ods::Prefix *prefix, const char *name) const
{
	return (prefix_->id() == prefix->id()) && (name_ == name);
}

bool
Attr::ToDouble(double &n)
{
	bool ok;
	double out = value_.toDouble(&ok);
	
	if (ok)
		n = out;
	
	return ok;
}

bool
Attr::ToInt32(int32_t &n)
{
	bool ok;
	int32_t out = value_.toInt(&ok);
	
	if (ok)
		n = out;
	
	return ok;
}

void
Attr::Write(QXmlStreamWriter &xml)
{
	xml.writeAttribute(FullName(), value_);
}

} // ods::
