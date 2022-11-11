#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API NumberSeconds: public Abstract
{
public:
	NumberSeconds(Abstract *parent, ods::Tag *tag = 0, ndff::Container *cntr = 0);
	NumberSeconds(const NumberSeconds &cloner);
	virtual ~NumberSeconds();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	i8 decimal_places() const { return number_decimal_places_; }
	void decimal_places(const i8 n) { number_decimal_places_ = n; }
	
	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	
	const QString&
	style() const { return number_style_; }
	
	void style(const QString &s) { number_style_ = s; }
	
	void WriteData(QXmlStreamWriter &xml) override;
	void WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba) override;
private:
	
	void Init(ndff::Container *cntr);
	void Init(ods::Tag *tag);
	
	QString number_style_;
	i8 number_decimal_places_ = -1;
};

} // ods::inst::
