#include "TableCalculationSettings.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods::inst {

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

void TableCalculationSettings::Init(Tag *tag)
{
	tag->Copy(ns_->table(), ns::kAutomaticFindLabels,
		table_automatic_find_labels_);
	tag->Copy(ns_->table(), ns::kUseRegularExpressions,
		table_use_regular_expressions_);
	tag->Copy(ns_->table(), ns::kUseWildcards, table_use_wildcards_);
}

void TableCalculationSettings::InitDefault()
{
	table_automatic_find_labels_ = QLatin1String("false");
	table_use_regular_expressions_ = QLatin1String("false");
	table_use_wildcards_ = QLatin1String("true");
}

void TableCalculationSettings::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name(), ns::kAutomaticFindLabels,
		ns::kUseRegularExpressions, ns::kUseWildcards}, list);
}

void TableCalculationSettings::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->table(), list);
}

void TableCalculationSettings::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->table(), ns::kAutomaticFindLabels,
		table_automatic_find_labels_);
	Write(xml, ns_->table(), ns::kUseRegularExpressions,
		table_use_regular_expressions_);
	Write(xml, ns_->table(), ns::kUseWildcards, table_use_wildcards_);
}

void TableCalculationSettings::WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba)
{
	CHECK_TRUE_VOID(ba != nullptr);
	WriteTag(kw, *ba);
	WriteNdffProp(kw, *ba, ns_->table(), ns::kAutomaticFindLabels,
		table_automatic_find_labels_);
	WriteNdffProp(kw, *ba, ns_->table(), ns::kUseRegularExpressions,
		table_use_regular_expressions_);
	WriteNdffProp(kw, *ba, ns_->table(), ns::kUseWildcards, table_use_wildcards_);
	CloseBasedOnChildren(h, kw, file, ba);
}

} // ods::inst::
