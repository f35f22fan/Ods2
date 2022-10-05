#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API StyleHeaderLeft : public Abstract
{
public:
	StyleHeaderLeft(Abstract *parent, Tag *tag = nullptr);
	StyleHeaderLeft(const StyleHeaderLeft &cloner);
	virtual ~StyleHeaderLeft();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;

	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	void WriteData(QXmlStreamWriter &xml) override;
	void WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba) override;
private:
	
	void Init(ods::Tag *tag);
	
	QString style_display_;
};

} // ods::inst::
