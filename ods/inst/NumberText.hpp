#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

class ODS_API NumberText : public Abstract
{
public:
	NumberText(Abstract *parent, ods::Tag *tag = nullptr);
	NumberText(const NumberText &cloner);
	virtual ~NumberText();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	QString*
	GetFirstString() const;
	
	void
	SetFirstString(const QString &s);
	
	void
	WriteData(QXmlStreamWriter &xml) override;
	
private:
	
	void Init(ods::Tag *tag);
};

} // ods::inst::
} // ods::
