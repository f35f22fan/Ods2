#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

class ODS_API NumberHours : public Abstract
{
public:
	NumberHours(Abstract *parent, ods::Tag *tag = nullptr);
	NumberHours(const NumberHours &cloner);
	virtual ~NumberHours();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	const QString&
	style() const { return number_style_; }
	
	void
	style(const QString &s) { number_style_ = s; }
	
	void
	WriteData(QXmlStreamWriter &xml) override;
	
private:
	
	void Init(ods::Tag *tag);
	
	QString number_style_;
};

} // ods::inst::
} // ods::
