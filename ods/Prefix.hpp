#pragma once

#include "err.hpp"
#include "global.hxx"

#include <QXmlStreamWriter>

namespace ods { // ods::

enum class UriId : quint8
{
	None = 0,
	Animation,
	Calcext,
	Chart,
	Config,
	Database,
	Dc,
	Dr3,
	Draw,
	Fo,
	Form,
	Loext,
	Manifest,
	Math,
	Meta,
	Number,
	Of,
	Office,
	Presentation,
	Script,
	Smil,
	StyleStyle,
	Svg,
	Text,
	Table,
	Xlink
};

class ODS_API Prefix {
public:
	
	virtual ~Prefix();
	
	static Prefix*
	Create(const ods::UriId id, const QString &s, const QString &uri);
	
	UriId
	id() const { return id_; }
	
	bool
	Is(const QString &s) const;
	
	bool
	Is(const Prefix *prefix) const;
	
	void
	set_id(const UriId id) { id_ = id; }
	
	void
	set_str(const QString &str) { str_ = str; }
	
	void
	set_uri(const QString &uri) { uri_ = uri; }
	
	const QString&
	str() const { return str_; }
	
	const QString&
	uri() const { return uri_; }
	
	QString
	With(const char *name) const;
	
	QString
	With(const QString &name) const;
	
	void
	Write(QXmlStreamWriter &xml) const;
	
private:
	NO_ASSIGN_COPY_MOVE(Prefix);
	Prefix(const UriId id, const QString &prefix, const QString &uri);
	
	UriId id_;
	QString str_;
	QString uri_;
};

} // ods::
