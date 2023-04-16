#pragma once

#include "Abstract.hpp"
#include "decl.hxx"

#include "../err.hpp"

#include <QFile>

namespace ods::inst {

class ODS_API DrawImage: public Abstract
{
public:
	DrawImage(Abstract *parent, ods::Tag *tag = 0, ndff::Container *cntr = 0);
	DrawImage(const DrawImage &cloner);
	virtual ~DrawImage();
	
	virtual Abstract* Clone(Abstract *parent = nullptr) const override;
	inst::DrawFrame *frame() { return (inst::DrawFrame*)parent_; }
	const QString& href() const { return xlink_href_; }
	
	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	bool LoadImage(const QString &full_path, QSize &sz);
	void WriteData(QXmlStreamWriter &xml) override;
	void WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba) override;
	
private:
	void Init(ndff::Container *cntr);
	void Init(ods::Tag*);
	
	QString xlink_href_;
	QString xlink_type_;
	QString xlink_show_;
	QString xlink_actuate_;
};

} // ods::inst::
