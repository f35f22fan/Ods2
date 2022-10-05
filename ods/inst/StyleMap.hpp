#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API StyleMap : public Abstract
{
public:
	StyleMap(Abstract *parent, ods::Tag *tag = nullptr);
	StyleMap(const StyleMap &cloner);
	virtual ~StyleMap();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	void WriteData(QXmlStreamWriter &xml) override;
	void WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba) override;
private:
	
	void Init(ods::Tag *tag);
	
	// <style:map style:condition="value()&gt;=0"
	// style:apply-style-name="N104P0"/>
	
	QString style_condition_;
	QString style_apply_style_name_;
};

} // ods::inst::
