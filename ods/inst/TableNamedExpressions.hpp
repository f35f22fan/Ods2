#pragma once

#include "Abstract.hpp"
#include "decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API TableNamedExpressions : public Abstract
{
public:
	TableNamedExpressions(Abstract *parent, Tag *tag = 0, ndff::Container *cntr = 0);
	TableNamedExpressions(const TableNamedExpressions &cloner);
	virtual ~TableNamedExpressions();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	void CopyNamedRangesTo(QVector<TableNamedRange*> *v);
	
	void ListChildren(QVector<StringOrInst *> &vec, const Recursively r) override;
	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	
	const QVector<TableNamedRange*>&
	named_ranges() const { return named_ranges_; }

	void WriteData(QXmlStreamWriter &xml) override;
private:
	
	void Init(ndff::Container *cntr);
	void Init(ods::Tag *tag);
	void InitDefault();
	void Scan(ods::Tag *tag);
	
	QVector<TableNamedRange*> named_ranges_;
};

} // ods::inst::
