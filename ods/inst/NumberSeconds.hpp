#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

class ODS_API NumberSeconds: public Abstract
{
public:
	NumberSeconds(Abstract *parent, ods::Tag *tag = nullptr);
	NumberSeconds(const NumberSeconds &cloner);
	virtual ~NumberSeconds();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	int8_t
	decimal_places() const { return number_decimal_places_; }
	
	void
	decimal_places(const int8_t n) { number_decimal_places_ = n; }
	
	const QString&
	style() const { return number_style_; }
	
	void
	style(const QString &s) { number_style_ = s; }
	
	void
	WriteData(QXmlStreamWriter &xml) override;

private:
	
	void Init(ods::Tag *tag);
	
	QString number_style_;
	int8_t number_decimal_places_ = -1;
};

} // ods::inst::
} // ods::
