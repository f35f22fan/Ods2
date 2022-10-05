#pragma once

#include "Abstract.hpp"
#include "decl.hxx"

#include "../err.hpp"

namespace ods::inst {

class ODS_API StyleTableColumnProperties : public Abstract
{
public:
	StyleTableColumnProperties(Abstract *parent, ods::Tag *tag = nullptr);
	StyleTableColumnProperties(const StyleTableColumnProperties &cloner);
	virtual ~StyleTableColumnProperties();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	Length*
	column_width() const { return style_column_width_; }
	
	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	void SetColumnWidth(Length *length);
	void WriteData(QXmlStreamWriter &xml) override;
	void WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba) override;
private:
	
	void Init(ods::Tag *tag);
	
	QString fo_break_before_;
	Length *style_column_width_ = nullptr;
};

} // ods::inst::
