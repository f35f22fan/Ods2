#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API NumberPercentageStyle : public Abstract
{
public:
	NumberPercentageStyle(Abstract *parent, ods::Tag *tag = 0, ndff::Container *cntr = 0);
	NumberPercentageStyle(const NumberPercentageStyle &cloner);
	virtual ~NumberPercentageStyle();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	//==> Style Interface
	virtual QString* style_name() override { return &style_name_; }
	//<== Style Interface
	
	inst::NumberNumber* FetchNumberStyle();
	inst::NumberText* FetchNumberText();
	inst::NumberNumber* GetNumberStyle() const;
	inst::NumberText* GetNumberText() const;

	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	
	inst::NumberNumber* NewNumberStyle();
	NumberText* NewNumberText();
	
	void style_name(const QString &s) { style_name_ = s; }
	
	void WriteData(QXmlStreamWriter &xml) override;
	void WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba) override;
private:
	void Init(ndff::Container *cntr);
	void Init(ods::Tag*);
	void Scan(ods::Tag*);
	
	QString style_name_;
};

} // ods::inst::
