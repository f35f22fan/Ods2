#include "MetaTemplate.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

MetaTemplate::MetaTemplate(Abstract *parent, Tag *tag)
: Abstract(parent, parent->ns(), id::MetaTemplate)
{
	if (tag != nullptr)
		Init(tag);
}

MetaTemplate::MetaTemplate(const MetaTemplate &cloner)
: Abstract(cloner)
{}


MetaTemplate::~MetaTemplate() {}

Abstract*
MetaTemplate::Clone(Abstract *parent) const
{
	auto *p = new MetaTemplate(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->xlink_type_ = xlink_type_;
	p->xlink_actuate_ = xlink_actuate_;
	p->xlink_title_ = xlink_title_;
	p->xlink_href_ = xlink_href_;
	p->meta_date_ = meta_date_;
	
	return p;
}

void
MetaTemplate::Init(Tag *tag)
{
	tag->Copy(ns_->xlink(), ods::ns::kType, xlink_type_);
	tag->Copy(ns_->xlink(), ods::ns::kActuate, xlink_actuate_);
	tag->Copy(ns_->xlink(), ods::ns::kTitle, xlink_title_);
	tag->Copy(ns_->xlink(), ods::ns::kHref, xlink_href_);
	tag->Copy(ns_->meta(), ods::ns::kDate, meta_date_);
}

void
MetaTemplate::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->xlink(), ods::ns::kType, xlink_type_);
	Write(xml, ns_->xlink(), ods::ns::kActuate, xlink_actuate_);
	Write(xml, ns_->xlink(), ods::ns::kTitle, xlink_title_);
	Write(xml, ns_->xlink(), ods::ns::kHref, xlink_href_);
	Write(xml, ns_->meta(), ods::ns::kDate, meta_date_);
}

} // ods::inst::
} // ods::
