#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

class MetaEditingDuration : public Abstract
{
public:
	MetaEditingDuration(Abstract *parent, Tag *tag = nullptr);
	MetaEditingDuration(const MetaEditingDuration &cloner);
	virtual ~MetaEditingDuration();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	void
	WriteData(QXmlStreamWriter &xml) override;

private:
	
	void Init(Tag *tag);
};

} // ods::inst::
} // ods::
