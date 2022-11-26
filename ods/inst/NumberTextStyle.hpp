#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API NumberTextStyle : public Abstract
{
public:
	NumberTextStyle(Abstract *parent, ods::Tag *tag = 0, ndff::Container *cntr = 0);
	NumberTextStyle(const NumberTextStyle &cloner);
	virtual ~NumberTextStyle();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	void WriteData(QXmlStreamWriter &xml) override;
	void WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba) override;

private:
	void Init(ndff::Container *cntr);
	void Init(ods::Tag *tag);
	void Scan(ods::Tag *tag);
	
	QString style_name_;
};

} // ods::inst::
