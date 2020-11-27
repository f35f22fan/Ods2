#include "FormulaError.hpp"

#include <QString>

namespace ods {

FormulaError::FormulaError(FormError e) : code_(e)
{}
FormulaError::~FormulaError() {}

FormulaError*
FormulaError::Clone()
{
	auto *p = new FormulaError(code_);
	p->more_info_ = more_info_;
	return p;
}

QString
FormulaError::toString() const
{
	switch (code_) {
	case FormError::None: {
		mtl_trace();
		return QString();
	}
	case FormError::WrongArgType: return QString::number(int(code_));
	case FormError::Name: return QLatin1String("#NAME?");
	case FormError::Ref: return QLatin1String("#REF");
	case FormError::Value: return QLatin1String("#VALUE");
	case FormError::Div0: return QLatin1String("#DIV0!");
	case FormError::Num: return QLatin1String("#NUM!");
	case FormError::MissingOp: return QString::number(int(code_));
	case FormError::MissingVar: return QString::number(int(code_));
	case FormError::NotImplemented: return QLatin1String("#NotImplemented");
	case FormError::Other: return QLatin1String("#Other");
	default: {
	mtl_trace();
	return QString();
	}
	}
}

QString
FormulaError::description() const
{
	QString s;
	switch (code_) {
		case FormError::None: {
			mtl_trace();
			return QString();
		}
		case FormError::Name: s = QLatin1String("No valid reference exists for the argument"); break;
		case FormError::Ref: s = QLatin1String("The column, row or sheet for the referenced cell is missing."); break;
		case FormError::Value: s = QLatin1String("The value for one of the \
			arguments is not the type that the argument requires. The value may \
			be entered incorrectly; for example, double-quotation marks may \
			be missing around the value. At other times, a cell or range \
			used may have the wrong format, such as text instead of numbers"); break;
		case FormError::Div0: s = QLatin1String("Division by zero"); break;
		case FormError::Num: s = QLatin1String("A calculation results in an overflow of the defined value range"); break;
		case FormError::MissingOp: s = QLatin1String("An operator such \
			as an equal sign is missing from the formula"); break;
		case FormError::NotImplemented: s = QLatin1String("Feature not implemented yet"); break;
		case FormError::Other: s = QLatin1String("Unspecified Error"); break;
		case FormError::WrongArgType: s = QLatin1String("Wrong argument type"); break;
		default: {
		mtl_trace();
		s = QString();
		}
	}
	
	if (!more_info_.isEmpty()) {
		s.append(QLatin1String(". ") + more_info_);
	}
	
	return s;
}

void
FormulaError::PrintError() {
	auto ba = description().toLocal8Bit();
	mtl_info("%s", ba.data());
}


}
