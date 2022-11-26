#include "OfficeBody.hpp"

#include "OfficeSpreadsheet.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

#include "../ndff/Container.hpp"
#include "../ndff/Property.hpp"

namespace ods::inst {

OfficeBody::OfficeBody(Abstract *parent, Tag *tag, ndff::Container *cntr)
: Abstract(parent, parent->ns(), id::OfficeBody)
{
	if (cntr)
		Init(cntr);
	else if (tag)
		Init(tag);
	else
		InitDefault();
}

OfficeBody::OfficeBody(const OfficeBody &cloner)
: Abstract(cloner)
{}

OfficeBody::~OfficeBody()
{
	delete office_spreadsheet_;
}

Abstract*
OfficeBody::Clone(Abstract *parent) const
{
	auto *p = new OfficeBody(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	if (office_spreadsheet_ != nullptr)
		p->office_spreadsheet_ = (OfficeSpreadsheet*)office_spreadsheet_->Clone();
	
	return p;
}

void OfficeBody::Init(ndff::Container *cntr)
{
	using Op = ndff::Op;
	ndff::Property prop;
	Op op = cntr->Next(prop, Op::TS);
	if (op == Op::N32_TE)
		return;

	if (op == Op::TCF_CMS)
		op = cntr->Next(prop, op);

	while (true)
	{
		if (op == Op::TS)
		{
			if (prop.is(ns_->office()))
			{
				if (prop.name == ns::kSpreadsheet)
					office_spreadsheet_ = new inst::OfficeSpreadsheet(this, 0, cntr);
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

void OfficeBody::Init(Tag *tag)
{
	Scan(tag);
}

void OfficeBody::InitDefault()
{
	office_spreadsheet_ = new inst::OfficeSpreadsheet(this);
}

void OfficeBody::ListChildren(QVector<StringOrInst*> &vec,
	const Recursively r)
{
	if (office_spreadsheet_)
	{
		vec.append(new StringOrInst(office_spreadsheet_, TakeOwnership::No));
		if (r == Recursively::Yes)
			office_spreadsheet_->ListChildren(vec, r);
	}
}

void OfficeBody::ListKeywords(inst::Keywords &list, const inst::LimitTo lt)
{
	inst::AddKeywords({tag_name()}, list);
}

void OfficeBody::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->office(), list);
}

void OfficeBody::Scan(Tag *tag)
{
	for (auto *x: tag->nodes())
	{
		if (AddText(x))
			continue;
		
		auto *next = x->as_tag();
		
		if (next->Is(ns_->office(), ods::ns::kSpreadsheet)) {
			office_spreadsheet_ = new OfficeSpreadsheet(this, next);
		} else {
			Scan(next);
		}
	}
}

void OfficeBody::WriteData(QXmlStreamWriter &xml)
{
	if (office_spreadsheet_)
		office_spreadsheet_->Write(xml);
}

} // ods::inst::
