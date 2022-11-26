#pragma once

#include "Abstract.hpp"
#include "../decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API StyleFooterLeft : public Abstract
{
public:
	StyleFooterLeft(Abstract *parent, Tag *tag = 0, ndff::Container *cntr = 0);
	StyleFooterLeft(const StyleFooterLeft &cloner);
	virtual ~StyleFooterLeft();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	void WriteData(QXmlStreamWriter &xml) override;
	void WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba) override;
	
private:
	void Init(ndff::Container *cntr);
	void Init(Tag *tag);
	
	QString style_display_;
};

} // ods::inst::
