#include "StringOrTag.hpp"

#include "Tag.hpp"

namespace ods {

StringOrTag::StringOrTag(const QString &s)
: type_(StringOrTagType::String)
{
	data_ = new QString(s);
}

StringOrTag::StringOrTag(ods::Tag *tag)
: data_(tag), type_(StringOrTagType::Tag) {}

StringOrTag::~StringOrTag()
{
	DeleteData();
}

void
StringOrTag::DeleteData()
{
	if (is_string())
		delete as_string();
	else
		delete as_tag();
}

} // ods::