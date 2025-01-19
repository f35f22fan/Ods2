#pragma once

#include "ndff.hh"

namespace ods::ndff {

class ODS_API CellNote {
public:
	
	
private:
	NO_ASSIGN_COPY_MOVE(CellNote);
	
	Op last_op_ = Op::None;
	Op op_ = Op::None;
	QString description_;
};

}
