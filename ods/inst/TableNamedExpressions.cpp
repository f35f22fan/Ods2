#include "TableNamedExpressions.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

TableNamedExpressions::TableNamedExpressions(Abstract *parent, Tag *tag)
: Abstract(parent, parent->ns(), id::TableNamedExpressions)
{
	if (tag != nullptr)
		Init(tag);
	else
		InitDefault();
}

TableNamedExpressions::TableNamedExpressions(const TableNamedExpressions &cloner)
: Abstract(cloner)
{}

TableNamedExpressions::~TableNamedExpressions() {}

Abstract*
TableNamedExpressions::Clone(Abstract *parent) const
{
	auto *p = new TableNamedExpressions(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	return p;
}

void
TableNamedExpressions::Init(Tag *tag)
{
	ScanString(tag);
}

void
TableNamedExpressions::InitDefault()
{}

void
TableNamedExpressions::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
} // ods::
