#pragma once

#include "decl.hxx"
#include "err.hpp"
#include "global.hxx"
#include "ods.hxx"
#include "Prefix.hpp"

#include <QMap>
#include <QXmlStreamAttributes>
#include <QXmlStreamReader>

namespace ods { // ods::

class ODS_API Tag
{
public:
	Tag(Ns *ns, ods::Prefix *prefix, const QString &name);
	virtual ~Tag();
	
	void
	Append(const QString &s);
	
	void
	Append(ods::Tag *tag);
	
	QVector<ods::Attr*>&
	attrs() { return attrs_; }
	
	void Copy(ods::Prefix *prefix, QStringView name, ods::Bool &into);
	void Copy(ods::Prefix *prefix, QStringView name, ods::Length **size);
	void Copy(ods::Prefix *prefix, QStringView name, QString &into);
	void Copy(ods::Prefix *prefix, QStringView name, i32 &into);
	void Copy(ods::Prefix *prefix, QStringView name, i8 &into);
	
	static Tag*
	From(QXmlStreamReader &xml, ods::Ns *ns);
	
	QString FullName() const;
	ods::Attr* GetAttr(ods::Prefix *prefix, QStringView name);
	bool Has(const ods::Prefix *prefix) const;
	bool Has(QStringView name) const { return name_ == name; }
	bool Is(const ods::Prefix *prefix, const QString &name) const;
	bool IsAnyCell() const;
	bool IsTextP() const;
	
	const QString& name() const { return name_; }
	
	QVector<StringOrTag*>&
	nodes() { return nodes_; }
	
	ods::Ns* ns() const { return ns_; }
	
	ods::Prefix*
	prefix() { return prefix_; }
	
	void SetAttributes(const QXmlStreamAttributes &attrs);
	
private:
	NO_ASSIGN_COPY_MOVE(Tag);
	
	QVector<ods::Attr*> attrs_;
	QVector<StringOrTag*> nodes_;
	
	ods::Prefix *prefix_ = nullptr;
	ods::Ns *ns_ = nullptr;
	QString name_;
	i32 current_tag_index_ = -1;
};

} // ods::
