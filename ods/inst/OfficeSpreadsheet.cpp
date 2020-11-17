#include "OfficeSpreadsheet.hpp"

#include "TableCalculationSettings.hpp"
#include "../Sheet.hpp"
#include "TableNamedExpressions.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

OfficeSpreadsheet::OfficeSpreadsheet(Abstract *parent, Tag *tag)
: Abstract(parent, parent->ns(), id::OfficeSpreadsheet)
{
	if (tag != nullptr)
		Init(tag);
	else
		InitDefault();
}

OfficeSpreadsheet::OfficeSpreadsheet(const OfficeSpreadsheet &cloner)
: Abstract(cloner)
{}

OfficeSpreadsheet::~OfficeSpreadsheet()
{
	delete table_calculation_settings_;
	delete table_named_expressions_;
	
	for (auto *next: tables_)
		delete next;
}

Abstract*
OfficeSpreadsheet::Clone(Abstract *parent) const
{
	auto *p = new OfficeSpreadsheet(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	if (table_calculation_settings_ != nullptr)
	{
		p->table_calculation_settings_ = (TableCalculationSettings*)
			table_calculation_settings_->Clone(p);
	}
	
	if (table_named_expressions_ != nullptr)
	{
		p->table_named_expressions_ = (TableNamedExpressions*)
			table_named_expressions_->Clone(p);
	}
	
	for (auto *next: tables_)
	{
		p->tables_.append((ods::Sheet*)next->Clone(p));
	}
	
	return p;
}

ods::Sheet*
OfficeSpreadsheet::GetSheet(const int index) const
{
	if (index >= tables_.size())
		return nullptr;
	
	return tables_[index];
}

ods::Sheet*
OfficeSpreadsheet::GetSheet(const QStringRef name) const
{
	if (name.isEmpty())
		return nullptr;
	
	for (auto *sheet: tables_)
	{
		if (sheet->name() == name)
			return sheet;
	}
	
	return nullptr;
}

void
OfficeSpreadsheet::Init(Tag *tag)
{
	Scan(tag);
}

void
OfficeSpreadsheet::InitDefault()
{
	table_calculation_settings_ = new TableCalculationSettings(this);
	table_named_expressions_ = new TableNamedExpressions(this);
}

ods::Sheet*
OfficeSpreadsheet::NewSheet(const QString &name)
{
	if (name.contains('\'')) {
		mtl_warn("Sheet names can't contain \'");
		return nullptr;
	}
	
	if (GetSheet(name.midRef(0)) != nullptr)
		return nullptr;
	
	auto *sheet = new ods::Sheet(this);
	sheet->name(name);
	tables_.append(sheet);
	
	return sheet;
}

void
OfficeSpreadsheet::Scan(Tag *tag)
{
	for (auto *x: tag->nodes())
	{
		if (!x->is_tag())
			continue;
		
		auto *next = x->as_tag();
		
		if (next->Is(ns_->table(), ods::ns::kCalculationSettings)) {
			table_calculation_settings_ = new TableCalculationSettings(this, next);
		} else if (next->Is(ns_->table(), ods::ns::kTable)) {
			tables_.append(new ods::Sheet(this, next));
		} else if (next->Is(ns_->table(), ods::ns::kNamedExpressions)) {
			table_named_expressions_ = new TableNamedExpressions(this, next);
		} else {
			Scan(next);
		}
	}
}

void
OfficeSpreadsheet::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, table_calculation_settings_);
	
	for (auto *next: tables_)
		next->Write(xml);
	
	Write(xml, table_named_expressions_);
}

} // ods::inst::
} // ods::
