#pragma once

#include "Abstract.hpp"
#include "../decl.hxx"
#include "../err.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

class ODS_API TextP : public Abstract
{
public:
	TextP(Abstract *parent, ods::Tag *tag = nullptr);
	TextP(const TextP &cloner);
	virtual ~TextP();
	
	void
	AppendString(const QString &s);
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	QString*
	GetFirstString() const;
	
	// sets first string or appends
	void
	SetFirstString(const QString &s);
	
	void
	WriteData(QXmlStreamWriter &xml) override;

private:
	
	void Init(ods::Tag *tag);
	void Scan(ods::Tag *tag);
};

} // ods::inst::
} // ods::
