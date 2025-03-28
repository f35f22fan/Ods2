#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../attr/decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API StyleTableColumnProperties : public Abstract
{
public:
	StyleTableColumnProperties(Abstract *parent, ods::Tag *tag = 0,
		ndff::Container *cntr = 0);
	StyleTableColumnProperties(const StyleTableColumnProperties &cloner);
	virtual ~StyleTableColumnProperties();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	Length* column_width() const { return style_column_width_; }
	
	attr::VisualBreak* visual_break() const { return visual_break_; }
	void SetVisualBreak(attr::VisualBreak *visual_break) { visual_break_ = visual_break; }
	
	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	void SetColumnWidth(Length *length);
	void WriteData(QXmlStreamWriter &xml) override;
	void WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba) override;
private:
	
	void Init(ndff::Container *cntr);
	void Init(ods::Tag *tag);
	
	attr::VisualBreak *visual_break_ = nullptr;
	Length *style_column_width_ = nullptr;
};

} // ods::inst::
