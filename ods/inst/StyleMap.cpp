#include "StyleMap.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

StyleMap::StyleMap(Abstract *parent, ods::Tag *tag)
: Abstract(parent, parent->ns(), id::StyleMap)
{
	if (tag != nullptr)
		Init(tag);
}

StyleMap::StyleMap(const StyleMap &cloner)
: Abstract(cloner)
{}

StyleMap::~StyleMap() {}

Abstract*
StyleMap::Clone(Abstract *parent) const
{
	auto *p = new StyleMap(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->style_condition_ = style_condition_;
	p->style_apply_style_name_ = style_apply_style_name_;
	
	return p;
}

void
StyleMap::Init(ods::Tag *tag)
{
	tag->Copy(ns_->style(), ods::ns::kCondition, style_condition_);
	tag->Copy(ns_->style(), ods::ns::kApplyStyleName, style_apply_style_name_);
	ScanString(tag);
}

void
StyleMap::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->style(), ods::ns::kCondition, style_condition_);
	Write(xml, ns_->style(), ods::ns::kApplyStyleName, style_apply_style_name_);
	WriteNodes(xml);
}

} // ods::inst::
} // ods::
