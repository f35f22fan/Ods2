#pragma once

#include "decl.hxx"
#include "global.hxx"

#include <QString>

namespace ods {

enum class StringOrTagType : quint8
{
	None,
	String,
	Tag
};

class ODS_API StringOrTag
{
public:
	StringOrTag(const QString &s);
	
	StringOrTag(ods::Tag *tag);
	
	virtual ~StringOrTag();
	
	bool
	is_tag() const { return type_ == StringOrTagType::Tag; }
	
	bool
	is_string() const { return type_ == StringOrTagType::String; }
	
	ods::Tag*
	as_tag() const { return (ods::Tag*)data_; }
	
	QString*
	as_string() const { return (QString*)data_; }
	
private:
	void DeleteData();
	
	void *data_ = nullptr;
	StringOrTagType type_ = StringOrTagType::None;
};


} // ods::