#include "TableCalculationSettings.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

TableCalculationSettings::TableCalculationSettings(Abstract *parent, Tag *tag)
: Abstract(parent, parent->ns(), id::TableCalculationSettings)
{
	if (tag != nullptr)
		Init(tag);
	else
		InitDefault();
}

TableCalculationSettings::TableCalculationSettings(const TableCalculationSettings &cloner)
: Abstract(cloner)
{}

TableCalculationSettings::~TableCalculationSettings() {}

Abstract*
TableCalculationSettings::Clone(Abstract *parent) const
{
	auto *p = new TableCalculationSettings(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->table_automatic_find_labels_ = table_automatic_find_labels_;
	p->table_use_regular_expressions_ = table_use_regular_expressions_;
	p->table_use_wildcards_ = table_use_wildcards_;
	
	return p;
}

void
TableCalculationSettings::Init(Tag *tag)
{
	tag->Copy(ns_->table(), ods::ns::kAutomaticFindLabels,
		table_automatic_find_labels_);
	tag->Copy(ns_->table(), ods::ns::kUseRegularExpressions,
		table_use_regular_expressions_);
	tag->Copy(ns_->table(), ods::ns::kUseWildcards, table_use_wildcards_);
}

void
TableCalculationSettings::InitDefault()
{
	table_automatic_find_labels_ = QLatin1String("false");
	table_use_regular_expressions_ = QLatin1String("false");
	table_use_wildcards_ = QLatin1String("true");
}

void
TableCalculationSettings::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->table(), ods::ns::kAutomaticFindLabels,
		table_automatic_find_labels_);
	Write(xml, ns_->table(), ods::ns::kUseRegularExpressions,
		table_use_regular_expressions_);
	Write(xml, ns_->table(), ods::ns::kUseWildcards, table_use_wildcards_);
}

} // ods::inst::
} // ods::
