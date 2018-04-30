#pragma once

#include "decl.hxx"
#include "node.hxx"

#include "../err.hpp"
#include "../ods.hxx"

namespace ods { // ods::
namespace formula { // ods::formula::

class  ODS_API Node
{
public:
	virtual ~Node();
	
	void
	Clear();
	
	CellRef*
	as_cell_ref() const { return (CellRef*) ptr_; }
	
	ods::Grouping*
	as_grouping() const { return (ods::Grouping*) ptr_; }
	
	ods::Op*
	as_op() const { return (ods::Op*) ptr_; }
	
	ods::formula::Value*
	as_value() const { return (ods::formula::Value*) ptr_; }
	
	QString
	debug_type() const;
	
	bool
	is_any() const { return type_ != node::Type::None; }
	
	bool
	is_cell_ref() const { return type_ == node::Type::CellRef; }
	
	bool
	is_grouping() const { return type_ == node::Type::Grouping; }
	
	bool
	is_op() const { return type_ == node::Type::Op; }
	
	bool
	is_close_grouping() const { return !is_open_grouping(); }
	
	bool
	is_open_grouping() const;
	
	bool
	is_value() const { return type_ == node::Type::Value; }
	
	static Node*
	New(formula::CellRef*);
	
	static Node*
	New(const ods::Grouping g);
	
	static Node*
	New(const ods::Op op);
	
	static Node*
	New(const formula::Value &value);
	
private:
	NO_ASSIGN_COPY_MOVE(Node);
	
	Node();
	
	node::Type type_ = node::Type::None;
	void *ptr_ = nullptr;
};

} // ods::formula::
} // ods::
