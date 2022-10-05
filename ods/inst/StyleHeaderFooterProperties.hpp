#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API StyleHeaderFooterProperties : public Abstract
{
public:
	StyleHeaderFooterProperties(Abstract *parent, ods::Tag *tag = nullptr);
	StyleHeaderFooterProperties(const StyleHeaderFooterProperties &cloner);
	virtual ~StyleHeaderFooterProperties();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;

	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	void WriteData(QXmlStreamWriter &xml) override;
	void WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba) override;
private:
	
	void Init(Tag *tag);
	void Scan(Tag *tag);
	
	QString fo_background_color_;
	QString fo_border_;
	QString fo_padding_;
	QString fo_min_height_;
	QString fo_margin_left_;
	QString fo_margin_right_;
	QString fo_margin_top_;
	QString fo_margin_bottom_;
};

} // ods::inst::

