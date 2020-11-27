#pragma once

#include "err.hpp"
#include "global.hxx"
#include "ods.hxx"
#include "types.hxx"
#include <cstdint>

namespace ods {
class ODS_API FormulaError {
public:
	FormulaError(FormError e);
	virtual ~FormulaError();
	
	FormulaError* Clone();
	
	QString toString() const; // displayed in cell text
	QString description() const;
	void PrintError();
	void more_info(const QString &s) { more_info_ = s; }
private:
	
	FormError code_ = FormError::None;
	QString more_info_;
};
}
