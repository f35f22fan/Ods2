#include "FormulaError.hpp"

#include <QString>

namespace ods {

QString
FormulaError::as_cell_text() const
{
	switch (code_) {
	case FormError::None: {
		mtl_trace();
		return QString();
	}
	case FormError::Name: return QLatin1String("#NAME?");
	case FormError::Ref: return QLatin1String("#REF");
	case FormError::Value: return QLatin1String("#VALUE");
	case FormError::Div0: return QLatin1String("#DIV0!");
	case FormError::Num: return QLatin1String("#NUM!");
	case FormError::MissingOp: return QString::number(int(code_));
	case FormError::MissingVar: return QString::number(int(code_));
	case FormError::Other: return QLatin1String("Other");
	default: {
	mtl_trace();
	return QString();
	}
	}
}

QString
FormulaError::description() const
{
	switch (code_) {
	case FormError::None: {
		mtl_trace();
		return QString();
	}
	case FormError::Name: return QLatin1String("No valid reference exists for the argument");
	case FormError::Ref: return QLatin1String("The column, row or sheet for the referenced cell is missing.");
	case FormError::Value: return QLatin1String("The value for one of the \
		arguments is not the type that the argument requires. The value may \
		be entered incorrectly; for example, double-quotation marks may \
		be missing around the value. At other times, a cell or range \
		used may have the wrong format, such as text instead of numbers");
	case FormError::Div0: return QLatin1String("Division by zero");
	case FormError::Num: return QLatin1String("A calculation results in an overflow of the defined value range");
	case FormError::MissingOp: return QLatin1String("An operator such \
		as an equal sign is missing from the formula");
	case FormError::Other: return QLatin1String("A variable is missing \
		from the formula");
	default: {
	mtl_trace();
	return QString();
	}
	}
}

}
