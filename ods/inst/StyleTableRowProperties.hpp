#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"
#include "../ods.hxx"

namespace ods::inst {

class ODS_API StyleTableRowProperties : public Abstract
{
public:
	StyleTableRowProperties(inst::Abstract *parent, ods::Tag *tag = 0, ndff::Container *cntr = 0);
	StyleTableRowProperties(const StyleTableRowProperties &cloner);
	virtual ~StyleTableRowProperties();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	void SetOptimal(Length *size);
	void WriteData(QXmlStreamWriter &xml) override;
	void WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba) override;
private:
	void Init(ndff::Container *cntr);
	void Init(ods::Tag *tag);
	
	QString fo_break_before_;
	ods::Bool style_use_optimal_row_height_ = ods::Bool::None;
	Length *style_row_height_ = nullptr;
};

} // ods::inst::
