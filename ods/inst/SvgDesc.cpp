#include "SvgDesc.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

SvgDesc::SvgDesc(Abstract *parent, Tag * tag)
: Abstract (parent, parent->ns(), id::SvgDesc)
{
	if (tag != nullptr)
		Init(tag);
}

SvgDesc::SvgDesc(const SvgDesc &cloner)
: Abstract (cloner)
{}

SvgDesc::~SvgDesc() {}

Abstract*
SvgDesc::Clone(Abstract *parent) const
{
	auto *p = new SvgDesc(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	return p;
}

QString*
SvgDesc::GetFirstString() const
{
	for (StringOrInst *x: nodes_)
	{
		if (x->is_string())
			return x->as_string();
	}
	
	return nullptr;
}

void
SvgDesc::Init(ods::Tag *tag)
{
	ScanString(tag);
}

void
SvgDesc::SetFirstString(const QString &s)
{
	for (auto *next : nodes_)
	{
		if (next->is_string())
		{
			next->SetString(s);
			return;
		}
	}
	
	Append(s);
}

void
SvgDesc::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
} // ods::
