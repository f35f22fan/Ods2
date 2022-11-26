#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API NumberMinutes : public Abstract
{
public:
	NumberMinutes(Abstract *parent, ods::Tag *tag = 0, ndff::Container *cntr = 0);
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
	void WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba) override;
	
private:
	void Init(ndff::Container *cntr);
	void Init(Tag *tag);
	
	QString number_style_;
};

} // ods::inst::
