#include "SvgTitle.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

SvgTitle::SvgTitle(Abstract *parent, Tag *tag)
: Abstract (parent, parent->ns(), id::SvgTitle)
{
	if (tag != nullptr)
		Init(tag);
}

SvgTitle::SvgTitle(const SvgTitle &cloner)
: Abstract (cloner)
{}

SvgTitle::~SvgTitle() {}

Abstract*
SvgTitle::Clone(Abstract *parent) const
{
	auto *p = new SvgTitle(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	return p;
}

QString*
SvgTitle::GetFirstString() const
{
	for (StringOrInst *x: nodes_)
	{
		if (x->is_string())
			return x->as_string();
	}
	
	return nullptr;
}

void
SvgTitle::Init(ods::Tag *tag)
{
	ScanString(tag);
}

void
SvgTitle::SetFirstString(const QString &s)
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
SvgTitle::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
} // ods::
