#pragma once

#include "Abstract.hpp"
#include "decl.hxx"

#include "../decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API OfficeSpreadsheet : public Abstract
{
public:
	OfficeSpreadsheet(Abstract *parent, Tag *tag = nullptr);
	OfficeSpreadsheet(const OfficeSpreadsheet &cloner);
	virtual ~OfficeSpreadsheet();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	ods::Sheet*
	GetSheet(const int index) const;
	
	ods::Sheet*
	GetSheet(QStringView name) const;
	
	bool has_children(const IncludingText itx) const override
	{
		return  (tables_.size() > 0) ||table_calculation_settings_ ||
			named_expressions_;
	}
	void ListChildren(QVector<StringOrInst *> &vec, const Recursively r) override;
	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	
	TableNamedExpressions*
	named_expressions() const { return named_expressions_; }
	
	ods::Sheet*
	NewSheet(const QString &name);
	
	int sheet_count() const { return tables_.size(); }
	QVector<ods::Sheet*>& tables() { return tables_; }
	
	void WriteData(QXmlStreamWriter &xml) override;
	
private:
	
	void Init(Tag *tag);
	void InitDefault();
	void Scan(Tag *tag);
	
	TableCalculationSettings *table_calculation_settings_ = nullptr;
	QVector<ods::Sheet*> tables_;
	TableNamedExpressions *named_expressions_ = nullptr;
};

} // ods::inst::
