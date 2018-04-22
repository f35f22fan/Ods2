#include "StylePageLayoutProperties.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

StylePageLayoutProperties::StylePageLayoutProperties(Abstract *parent, Tag *tag)
: Abstract(parent, parent->ns(), id::StylePageLayoutProperties)
{
	if (tag != nullptr)
		Init(tag);
}

StylePageLayoutProperties::StylePageLayoutProperties(const StylePageLayoutProperties &cloner)
: Abstract(cloner)
{}

StylePageLayoutProperties::~StylePageLayoutProperties() {}

Abstract*
StylePageLayoutProperties::Clone(Abstract *parent) const
{
	auto *p = new StylePageLayoutProperties(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->style_writing_mode_ = style_writing_mode_;
	
	return p;
}

void
StylePageLayoutProperties::Init(ods::Tag *tag)
{
	tag->Copy(ns_->style(), ods::ns::kWritingMode, style_writing_mode_);
	ScanString(tag);
}

void
StylePageLayoutProperties::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->style(), ods::ns::kWritingMode, style_writing_mode_);
	WriteNodes(xml);
}

} // ods::inst::
} // ods::