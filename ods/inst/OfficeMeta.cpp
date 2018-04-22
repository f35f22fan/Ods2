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

namespace ods { // ods::
namespace inst { // ods::inst::

OfficeMeta::OfficeMeta(Abstract *parent, Tag *tag)
: Abstract(parent, parent->ns(), id::OfficeMeta)
{
	if (tag != nullptr)
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
	
	return p;
}

void
OfficeMeta::Init(Tag *tag)
{
	Scan(tag);
}

void
OfficeMeta::InitDefault()
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

void
OfficeMeta::Scan(Tag *tag)
{
	for (auto *x: tag->nodes())
	{
		if (!x->is_tag())
			continue;
		
		auto *next = x->as_tag();
		
		if (next->Is(ns_->meta(), ods::ns::kCreationDate))
		{
			Append(new MetaCreationDate(this, next));
		} else if (next->Is(ns_->dc(), ods::ns::kTitle)) {
			Append(new DcTitle(this, next));
		} else if (next->Is(ns_->meta(), ods::ns::kEditingDuration)) {
			Append(new MetaEditingDuration(this, next));
		} else if (next->Is(ns_->meta(), ods::ns::kEditingCycles)) {
			Append(new MetaEditingCycles(this, next));
		} else if (next->Is(ns_->meta(), ods::ns::kGenerator)) {
			Append(new MetaGenerator(this, next));
		} else if (next->Is(ns_->dc(), ods::ns::kDate)) {
			Append(new DcDate(this, next));
		} else if (next->Is(ns_->meta(), ods::ns::kDocumentStatistic)) {
			Append(new MetaDocumentStatistic(this, next));
		} else if (next->Is(ns_->meta(), ods::ns::kTemplate)) {
			Append(new MetaTemplate(this, next));
		} else {
			Scan(next);
		}
	}
}

void
OfficeMeta::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
} // ods::
