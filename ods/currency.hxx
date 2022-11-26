#pragma once

#include <QString>

#include "global.hxx"
#include "types.hxx"

namespace ods {

enum class CurrencyId : u16 {
	None,
	USD,
	EUR,
	GBP,
	CNY,
	RUB,
	JPY,
	XBT,
	ETH,
	SEK,
};

struct ODS_API Currency {
	CurrencyId id;
	double qtty;

	Currency* Clone();
	bool valid() const { return id != CurrencyId::None; }
};

struct ODS_API CurrencyInfo {
	QString str;
	QString symbol;
};

}
