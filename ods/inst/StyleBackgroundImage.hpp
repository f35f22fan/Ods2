#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API StyleBackgroundImage : public Abstract
{
public:
	StyleBackgroundImage(Abstract *parent, Tag *tag = 0);
	StyleBackgroundImage(const StyleBackgroundImage &cloner);
	virtual ~StyleBackgroundImage();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;

	virtual void
	ListChildren(QVector<StringOrInst*> &vec, const Recursively r) override {}
	
	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	void WriteData(QXmlStreamWriter &xml) override;
	
private:
};

} // ods::inst::
