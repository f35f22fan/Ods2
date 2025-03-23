#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../attr/decl.hxx"
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
	void SetRowHeight(Length *size);
	void WriteData(QXmlStreamWriter &xml) override;
	void WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba) override;
	
	Length* GetRowHeight() { return style_row_height_; }
	
	attr::VisualBreak* visual_break() const { return visual_break_; }
	void SetVisualBreak(attr::VisualBreak *visual_break) { visual_break_ = visual_break; }
	
private:
	void Init(ndff::Container *cntr);
	void Init(ods::Tag *tag);
	
	ods::Bool style_use_optimal_row_height_ = ods::Bool::None;
	Length *style_row_height_ = nullptr;
	attr::VisualBreak *visual_break_ = nullptr;
};

} // ods::inst::
