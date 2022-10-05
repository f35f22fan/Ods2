#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API StylePageLayout : public Abstract
{
public:
	StylePageLayout(Abstract *parent, ods::Tag *tag = nullptr);
	StylePageLayout(const StylePageLayout &cloner);
	virtual ~StylePageLayout();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	void WriteData(QXmlStreamWriter &xml) override;
	void WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba) override;
private:
	
	void Init(ods::Tag *tag);
	void Scan(ods::Tag *tag);
	
	QString style_name_;
};

} // ods::inst::

