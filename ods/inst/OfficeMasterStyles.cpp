#include "OfficeMasterStyles.hpp"

#include "StyleMasterPage.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

OfficeMasterStyles::OfficeMasterStyles(Abstract *parent, Tag *tag)
: Abstract(parent, parent->ns(), id::OfficeMasterStyles)
{
	if (tag != nullptr)
		Init(tag);
}

OfficeMasterStyles::OfficeMasterStyles(const OfficeMasterStyles &cloner)
: Abstract(cloner)
{}

OfficeMasterStyles::~OfficeMasterStyles()
{}

Abstract*
OfficeMasterStyles::Clone(Abstract *parent) const
{
	auto *p = new OfficeMasterStyles(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	return p;
}

void
OfficeMasterStyles::Init(Tag *tag)
{
	Scan(tag);
}

void
OfficeMasterStyles::Scan(Tag *tag)
{
	for (auto *x: tag->nodes())
	{
		if (!x->is_tag())
			continue;
		
		auto *next = x->as_tag();
		
		if (next->Is(ns_->style(), ods::ns::kMasterPage)) {
			Append(new StyleMasterPage(this, next));
		} else {
			Scan(next);
		}
	}
}

void
OfficeMasterStyles::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
} // ods::
