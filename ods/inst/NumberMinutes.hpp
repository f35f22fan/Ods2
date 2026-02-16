#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API NumberMinutes : public Abstract
{
public:
	NumberMinutes(Abstract *parent, ods::Tag *tag = 0);
	NumberMinutes(const NumberMinutes &cloner);
	virtual ~NumberMinutes();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	
	const QString&
	style() const { return number_style_; }
	
	void style(const QString &s) { number_style_ = s; }
	void WriteData(QXmlStreamWriter &xml) override;
private:
	void Init(Tag *tag);
	
	QString number_style_;
};

} // ods::inst::
