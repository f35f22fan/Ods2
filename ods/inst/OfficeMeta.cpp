#include "OfficeMeta.hpp"

#include "DcDate.hpp"
#include "DcTitle.hpp"
#include "MetaCreationDate.hpp"
#include "MetaDocumentStatistic.hpp"
#include "MetaEditingCycles.hpp"
#include "MetaEditingDuration.hpp"
#include "MetaGenerator.hpp"
#include "MetaTemplate.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

#include "../ndff/Container.hpp"
#include "../ndff/Property.hpp"

namespace ods::inst {

OfficeMeta::OfficeMeta(Abstract *parent, Tag *tag, ndff::Container *cntr)
: Abstract(parent, parent->ns(), id::OfficeMeta)
{
	if (cntr)
		Init(cntr);
	else if (tag)
		Init(tag);
	else
		InitDefault();
}

OfficeMeta::OfficeMeta(const OfficeMeta &cloner)
: Abstract(cloner)
{}

OfficeMeta::~OfficeMeta() {}

Abstract*
OfficeMeta::Clone(Abstract *parent) const
{
	auto *p = new OfficeMeta(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->CloneChildrenOf(this);
	
	return p;
}

void OfficeMeta::Init(ndff::Container *cntr)
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
			if (prop.is(ns_->meta()))
			{
				if (prop.name == ns::kCreationDate)
					Append(new MetaCreationDate(this, 0, cntr), TakeOwnership::Yes);
				if (prop.name == ns::kEditingDuration)
					Append(new MetaEditingDuration(this, 0, cntr), TakeOwnership::Yes);
				if (prop.name == ns::kEditingCycles)
					Append(new MetaEditingCycles(this, 0, cntr), TakeOwnership::Yes);
				if (prop.name == ns::kGenerator)
					Append(new MetaGenerator(this, 0, cntr), TakeOwnership::Yes);
				if (prop.name == ns::kDocumentStatistic)
					Append(new MetaDocumentStatistic(this, 0, cntr), TakeOwnership::Yes);
				if (prop.name == ns::kTemplate)
					Append(new MetaTemplate(this, 0, cntr), TakeOwnership::Yes);
			} else if (prop.is(ns_->dc())) {
					if (prop.name == ns::kTitle)
						Append(new DcTitle(this, 0, cntr), TakeOwnership::Yes);
					else if (prop.name == ns::kDate)
						Append(new DcDate(this, 0, cntr), TakeOwnership::Yes);
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

void OfficeMeta::Init(Tag *tag)
{
	Scan(tag);
}

void OfficeMeta::InitDefault()
{
/*
	<office:meta>
		<meta:creation-date>2018-01-10T17:12:48.972737017</meta:creation-date>
		<dc:title>CustomShit</dc:title>
		<meta:editing-duration>PT9M2S</meta:editing-duration>
		<meta:editing-cycles>7</meta:editing-cycles>
		<meta:generator>LibreOffice/5.4.2.2$Linux_X86_64 LibreOffice_project/40m0$Build-2</meta:generator>
		<manifest:manifest>2018-02-17T20:19:35.616451379</manifest:manifest>
		<meta:document-statistic meta:table-count="1" meta:cell-count="16"
			meta:object-count="0"/>
		<meta:template xlink:type="simple" xlink:actuate="onRequest" xlink:title="CustomShit"
			xlink:href="../../.config/libreoffice/4/user/template/CustomShit.ots"
			meta:date="2018-01-10T17:12:48.543249213"/>
	</office:meta>
*/
}

void OfficeMeta::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name()}, list);
}

void OfficeMeta::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->office(), list);
}

void OfficeMeta::Scan(Tag *tag)
{
	for (auto *x: tag->nodes())
	{
		if (!x->is_tag())
			continue;
		
		auto *next = x->as_tag();
		
		if (next->Is(ns_->meta(), ns::kCreationDate))
		{
			Append(new MetaCreationDate(this, next), TakeOwnership::Yes);
		} else if (next->Is(ns_->dc(), ns::kTitle)) {
			Append(new DcTitle(this, next), TakeOwnership::Yes);
		} else if (next->Is(ns_->meta(), ns::kEditingDuration)) {
			Append(new MetaEditingDuration(this, next), TakeOwnership::Yes);
		} else if (next->Is(ns_->meta(), ns::kEditingCycles)) {
			Append(new MetaEditingCycles(this, next), TakeOwnership::Yes);
		} else if (next->Is(ns_->meta(), ns::kGenerator)) {
			Append(new MetaGenerator(this, next), TakeOwnership::Yes);
		} else if (next->Is(ns_->dc(), ns::kDate)) {
			Append(new DcDate(this, next), TakeOwnership::Yes);
		} else if (next->Is(ns_->meta(), ns::kDocumentStatistic)) {
			Append(new MetaDocumentStatistic(this, next), TakeOwnership::Yes);
		} else if (next->Is(ns_->meta(), ns::kTemplate)) {
			Append(new MetaTemplate(this, next), TakeOwnership::Yes);
		} else {
			Scan(next);
		}
	}
}

void OfficeMeta::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
