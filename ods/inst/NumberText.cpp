#include "NumberText.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

NumberText::NumberText(ods::inst::Abstract *parent, Tag *tag)
: Abstract(parent, parent->ns(), id::NumberText)
{
	if (tag != nullptr)
		Init(tag);
}

NumberText::NumberText(const NumberText &cloner)
: Abstract(cloner)
{}

NumberText::~NumberText() {}

Abstract*
NumberText::Clone(Abstract *parent) const
{
	auto *p = new NumberText(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	return p;
}

QString*
NumberText::GetFirstString() const
{
	for (StringOrInst *x: nodes_)
	{
		if (x->is_string())
			return x->as_string();
	}
	
	return nullptr;
}

void
NumberText::Init(ods::Tag *tag)
{
	ScanString(tag);
}

void
NumberText::SetFirstString(const QString &s)
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
NumberText::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
} // ods::
