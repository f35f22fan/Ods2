#include "Node.hpp"

#include "CellRef.hpp"

#include "../ods.hh"

namespace ods { // ods::
namespace formula { // ods::formula::

Node::Node() {}

Node::~Node()
{
	Clear();
}

void
Node::Clear()
{
	if (!is_any())
		return;
	
	if (is_cell_ref())
		delete as_cell_ref();
	else if (is_op())
		delete as_op();
	else if (is_value())
		delete as_value();
	else if (is_grouping())
		delete as_grouping();
	else
		it_happened();
}

QString
Node::debug_type() const
{
	if (is_cell_ref())
		return QLatin1String("[cell ref]");
	
	if (is_grouping())
	{
		ods::Grouping *g = as_grouping();
		
		if (*g == ods::Grouping::Open)
			return QLatin1String("[Open]");
		
		return QLatin1String("[Close]");
	}
	
	if (is_op())
	{
		QChar c = ods::CharFromOp(*as_op());
		auto s = QString("[Op ");
		s.append(c).append(']');
		return s;
	}
	
	if (is_value())
		return QLatin1String("[Value]");
	
	return QLatin1String("[Something else]");
}

bool
Node::is_open_grouping() const
{
	ods::Grouping g = *as_grouping();
	return (ods::Grouping::Open == g);
}

Node*
Node::New(formula::CellRef *cell_ref)
{
	auto *p = new Node();
	p->ptr_ = cell_ref;
	p->type_ = node::Type::CellRef;
	
	return p;
}

Node*
Node::New(const ods::Grouping g)
{
	auto *p = new Node();
	p->type_ = node::Type::Grouping;
	p->ptr_ = new Grouping {g};
	
	if (!p->is_grouping() || (*p->as_grouping() != g))
	{
		mtl_warn();
	}
	
	return p;
}

Node*
Node::New(const ods::Op op)
{
	auto *p = new Node();
	p->ptr_ = new ods::Op {op};
	p->type_ = node::Type::Op;
	
	return p;
}

Node*
Node::New(const formula::Value &value)
{
	auto *p = new Node();
	p->ptr_ = value.Clone();
	p->type_ = node::Type::Value;
	
	return p;
}

} // ods::formula::
} // ods::
