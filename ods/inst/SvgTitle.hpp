#pragma once

#include "Abstract.hpp"
#include "decl.hxx"

#include "../err.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

class ODS_API SvgTitle : public Abstract
{
public:
	SvgTitle(Abstract *parent, ods::Tag *tag = nullptr);
	SvgTitle(const SvgTitle &cloner);
	virtual ~SvgTitle();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	QString*
	GetFirstString() const;
	
	void
	SetFirstString(const QString &s);
	
	void
	WriteData(QXmlStreamWriter &xml) override;

private:
	void Init(ods::Tag*);
};

} // ods::inst::
} // ods::
