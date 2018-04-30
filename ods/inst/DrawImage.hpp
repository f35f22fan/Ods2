#pragma once

#include "Abstract.hpp"
#include "decl.hxx"

#include "../err.hpp"

#include <QFile>

namespace ods { // ods::
namespace inst { // ods::inst::

class ODS_API DrawImage: public Abstract
{
public:
	DrawImage(Abstract *parent, ods::Tag *tag = nullptr);
	DrawImage(const DrawImage &cloner);
	virtual ~DrawImage();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	const QString&
	href() const { return xlink_href_; }
	
	void
	LoadImage(const QString &full_path, QSize &sz);
	
	void
	WriteData(QXmlStreamWriter &xml) override;

private:
	
	void Init(ods::Tag*);
	
	QString xlink_href_;
	QString xlink_type_;
	QString xlink_show_;
	QString xlink_actuate_;
};

} // ods::inst::
} // ods::
