#pragma once

#include "err.hpp"
#include "global.hxx"
#include "types.hxx"
#include <cstdint>

namespace ods {

enum class FormError: i16 {
	None,
	Name = 525,
	Ref = 524,
	Value = 519,
	Div0 = 532,
	Num,
	MissingOp = 509,
	MissingVar = 510,
	Other = INT16_MAX,
};

class ODS_API FormulaError {
public:
	
	QString as_cell_text() const;
	QString description() const;
	
private:
	
	FormError code_ = FormError::None;
};
}
