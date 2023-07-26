#include "TableCalculationSettings.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"
#include "../ndff/Container.hpp"
#include "../ndff/Property.hpp"

namespace ods::inst {

TableCalculationSettings::TableCalculationSettings(Abstract *parent, Tag *tag, ndff::Container *cntr)
: Abstract(parent, parent->ns(), id::TableCalculationSettings)
{
	if (cntr)
		Init(cntr);
	else if (tag)
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

void TableCalculationSettings::Init(ndff::Container *cntr)
{
	using Op = ndff::Op;
	ndff::Property prop;
	QHash<UriId, QVector<ndff::Property>> attrs;
	Op op = cntr->Next(prop, Op::TS, &attrs);
	CopyAttr(attrs, ns_->table(), ns::kAutomaticFindLabels,
			 table_automatic_find_labels_);
	CopyAttr(attrs, ns_->table(), ns::kUseRegularExpressions,
			 table_use_regular_expressions_);
	CopyAttr(attrs, ns_->table(), ns::kUseWildcards, table_use_wildcards_);
	ReadStrings(cntr, op);
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
	MTL_CHECK_VOID(ba);
	WriteTag(kw, *ba);
	WriteNdffProp(kw, *ba, ns_->table(), ns::kAutomaticFindLabels,
		table_automatic_find_labels_);
	WriteNdffProp(kw, *ba, ns_->table(), ns::kUseRegularExpressions,
		table_use_regular_expressions_);
	WriteNdffProp(kw, *ba, ns_->table(), ns::kUseWildcards, table_use_wildcards_);
	CloseBasedOnChildren(h, kw, file, ba);
}

} // ods::inst::
