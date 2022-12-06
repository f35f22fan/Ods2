#pragma once

#include "err.hpp"
#include "global.hxx"
#include "Ns.hpp"

#include <QXmlStreamWriter>

namespace ods {

class ODS_API Prefix {
public:
	
	virtual ~Prefix();
	
	static Prefix*
	Create(UriId &id, const QString &name, const QString &uri);
	
	UriId id() const { return id_; }
	bool Is(QStringView name) const;
	bool Is(const Prefix *prefix) const;
	void set_id(const UriId id) { id_ = id; }
	void set_name(const QString &str) { name_ = str; }
	void set_uri(const QString &uri) { uri_ = uri; }
	
	const QString&
	name() const { return name_; }
	
	const QString&
	uri() const { return uri_; }
	
	QString With(QStringView name) const;
	void Write(QXmlStreamWriter &xml) const;
	
private:
	NO_ASSIGN_COPY_MOVE(Prefix);
	Prefix(UriId &id, const QString &prefix, const QString &uri);
	
	UriId &id_;
	QString name_;
	QString uri_;
};

} // ods::
