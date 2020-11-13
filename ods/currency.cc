#include "currency.hh"

#include "err.hpp"

namespace ods {

Currency*
Currency::Clone() {
	auto *p = new Currency();
	p->id = id;
	p->qtty = qtty;
	return p;
}

namespace currency {

CurrencyInfo info(const Currency &c) {
	const auto id = c.id;
	
	switch (id) {
	case CurrencyId::EUR: return {"EUR", "€"};
	case CurrencyId::USD: return {"USD", "$"};
	case CurrencyId::GBP: return {"GBP", "₤"};
	case CurrencyId::CNY: return {"CNY", "¥"};
	case CurrencyId::RUB: return {"RUB", "₽"};
	case CurrencyId::JPY: return {"JPY", "¥"};
	case CurrencyId::XBT: return {"XBT", "₿"};
	case CurrencyId::ETH: return {"ETH", "Ξ"};
	default: {
		mtl_trace();
		return {"", ""};
	}
	}
}

Currency USD(double d) { return ods::Currency { CurrencyId::USD, d }; }
Currency EUR(double d) { return Currency { CurrencyId::EUR, d }; }
Currency GBP(double d) { return Currency { CurrencyId::GBP, d }; }
Currency CNY(double d) { return Currency { CurrencyId::CNY, d }; }
Currency RUB(double d) { return Currency { CurrencyId::RUB, d }; }
Currency JPY(double d) { return Currency { CurrencyId::JPY, d }; }
Currency XBT(double d) { return Currency { CurrencyId::XBT, d }; }
Currency ETH(double d) { return Currency { CurrencyId::ETH, d }; }
}}
