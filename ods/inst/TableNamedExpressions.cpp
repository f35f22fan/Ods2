#include "TableNamedExpressions.hpp"

#include "TableNamedRange.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

TableNamedExpressions::TableNamedExpressions(Abstract *parent, Tag *tag)
: Abstract(parent, parent->ns(), id::TableNamedExpressions)
{
	if (tag != nullptr)
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

void
TableNamedExpressions::CopyNamedRangesTo(QVector<TableNamedRange*> *v)
{
	CHECK_PTR_VOID(v);
	for (auto *item : named_ranges_)
		v->append(item);
}

void
TableNamedExpressions::Init(Tag *tag)
{
	Scan(tag);
}

void
TableNamedExpressions::InitDefault()
{}

void
TableNamedExpressions::Scan(ods::Tag *tag)
{
	foreach (auto *x, tag->nodes())
	{
		if (AddText(x))
			continue;
		
		auto *next = x->as_tag();
		
		if (next->Is(ns_->table(), ods::ns::kNamedRange)) {
			named_ranges_.append(new TableNamedRange(this, next));
		} else {
			Scan(next);
		}
	}
}

void
TableNamedExpressions::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
	
	for (TableNamedRange *item: named_ranges_)
	{
		item->Write(xml);
	}
}

} // ods::inst::
} // ods::
