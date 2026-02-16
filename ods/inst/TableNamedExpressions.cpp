#include "TableNamedExpressions.hpp"

#include "TableNamedRange.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods::inst {

TableNamedExpressions::TableNamedExpressions(Abstract *parent, Tag *tag)
: Abstract(parent, parent->ns(), id::TableNamedExpressions)
{
	if (tag)
		Init(tag);
	else
		InitDefault();
}

TableNamedExpressions::TableNamedExpressions(const TableNamedExpressions &cloner)
: Abstract(cloner)
{}

TableNamedExpressions::~TableNamedExpressions() {}

Abstract*
TableNamedExpressions::Clone(Abstract *parent) const
{
	auto *p = new TableNamedExpressions(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	return p;
}

void TableNamedExpressions::CopyNamedRangesTo(QVector<TableNamedRange*> *v)
{
	MTL_CHECK_VOID(v);
	for (auto *item : named_ranges_)
		v->append(item);
}

void TableNamedExpressions::Init(Tag *tag)
{
	Scan(tag);
}

void TableNamedExpressions::InitDefault()
{}

void TableNamedExpressions::ListChildren(QVector<StringOrInst*> &vec,
	const Recursively r)
{
	for (auto *item: named_ranges_)
	{
		vec.append(new StringOrInst(item, TakeOwnership::No));
		if (r == Recursively::Yes)
			item->ListChildren(vec, r);
	}
}

void TableNamedExpressions::ListKeywords(inst::Keywords &list, const inst::LimitTo lt)
{
	inst::AddKeywords({tag_name()}, list);
}

void TableNamedExpressions::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->table(), list);
}

void TableNamedExpressions::Scan(ods::Tag *tag)
{
	foreach (auto *x, tag->nodes())
	{
		if (AddText(x))
			continue;
		
		auto *next = x->as_tag();
		
		if (next->Is(ns_->table(), ns::kNamedRange)) {
			named_ranges_.append(new TableNamedRange(this, next));
		} else {
			Scan(next);
		}
	}
}

void TableNamedExpressions::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
	
	for (TableNamedRange *item: named_ranges_)
	{
		item->Write(xml);
	}
}

} // ods::inst::
