#include "OfficeBody.hpp"

#include "OfficeSpreadsheet.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods::inst {

OfficeBody::OfficeBody(Abstract *parent, Tag *tag)
: Abstract(parent, parent->ns(), id::OfficeBody)
{
	if (tag != nullptr)
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
