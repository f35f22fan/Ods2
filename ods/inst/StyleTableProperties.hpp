#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API StyleTableProperties : public Abstract
{
public:
	StyleTableProperties(Abstract *parent, Tag *tag = nullptr);
	StyleTableProperties(const StyleTableProperties &cloner);
	virtual ~StyleTableProperties();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	void WriteData(QXmlStreamWriter &xml) override;
	void WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba) override;
private:
	
	void Init(Tag *tag);
	
	QString table_display_;
	QString style_writing_mode_;
};

} // ods::inst::
