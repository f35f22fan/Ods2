#include "TableNamedExpressions.hpp"

#include "TableNamedRange.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

#include "../ndff/Container.hpp"
#include "../ndff/Property.hpp"

namespace ods::inst {

TableNamedExpressions::TableNamedExpressions(Abstract *parent, Tag *tag, ndff::Container *cntr)
: Abstract(parent, parent->ns(), id::TableNamedExpressions)
{
	if (cntr)
		Init(cntr);
	else if (tag)
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
	CHECK_PTR_VOID(v);
	for (auto *item : named_ranges_)
		v->append(item);
}

void TableNamedExpressions::Init(ndff::Container *cntr)
{
	using Op = ndff::Op;
	ndff::Property prop;
	//QHash<UriId, QVector<ndff::Property>> attrs;
	Op op = cntr->Next(prop, Op::TS, 0);//&attrs);

	if (op == Op::N32_TE)
		return;

	if (op == Op::TCF_CMS)
		op = cntr->Next(prop, op);

	while (true)
	{
		if (op == Op::TS)
		{
			if (prop.is(ns_->table()))
			{
				if (prop.name == ns::kNamedRange)
					Append(new inst::TableNamedRange(this, 0, cntr), TakeOwnership::Yes);
			}
		} else if (ndff::is_text(op)) {
			Append(cntr->NextString());
		} else {
			break;
		}
		op = cntr->Next(prop, op);
	}

	if (op != Op::SCT)
		mtl_trace("Unexpected op: %d", op);
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
