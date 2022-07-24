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

static const char * SYMBOL_EUR = "€";
static const char * SYMBOL_USD = "$";
static const char * SYMBOL_GBP = "₤";
static const char * SYMBOL_CNY = "¥";
static const char * SYMBOL_RUB = "₽";
static const char * SYMBOL_JPY = "¥";
static const char * SYMBOL_XBT = "₿";
static const char * SYMBOL_ETH = "Ξ";
static const char * SYMBOL_SEK = "kr";

CurrencyInfo info(const CurrencyId &id)
{
	switch (id) {
	case CurrencyId::EUR: return CurrencyInfo{"EUR", SYMBOL_EUR};
	case CurrencyId::USD: return CurrencyInfo{"USD", SYMBOL_USD};
	case CurrencyId::GBP: return CurrencyInfo{"GBP", SYMBOL_GBP};
	case CurrencyId::CNY: return CurrencyInfo{"CNY", SYMBOL_CNY};
	case CurrencyId::RUB: return CurrencyInfo{"RUB", SYMBOL_RUB};
	case CurrencyId::JPY: return CurrencyInfo{"JPY", SYMBOL_JPY};
	case CurrencyId::XBT: return CurrencyInfo{"XBT", SYMBOL_XBT};
	case CurrencyId::ETH: return CurrencyInfo{"ETH", SYMBOL_ETH};
	case CurrencyId::SEK: return CurrencyInfo{"SEK", SYMBOL_SEK};
	default: {
		mtl_trace();
		return {"", ""};
	}
	}
}

Currency USD(double d) { return Currency { CurrencyId::USD, d }; }
Currency EUR(double d) { return Currency { CurrencyId::EUR, d }; }
Currency GBP(double d) { return Currency { CurrencyId::GBP, d }; }
Currency CNY(double d) { return Currency { CurrencyId::CNY, d }; }
Currency RUB(double d) { return Currency { CurrencyId::RUB, d }; }
Currency JPY(double d) { return Currency { CurrencyId::JPY, d }; }
Currency XBT(double d) { return Currency { CurrencyId::XBT, d }; }
Currency ETH(double d) { return Currency { CurrencyId::ETH, d }; }
Currency SEK(double d) { return Currency { CurrencyId::SEK, d }; }

ods::Currency*
Query(const QString &country, const QString &symbol)
{
	auto cba = country.toLocal8Bit();
	auto sba = symbol.toLocal8Bit();
	mtl_info("Country: \"%s\", symbol: \"%s\"", cba.data(), sba.data());

	if (symbol == SYMBOL_EUR)
		return new Currency(EUR(0));
	if (symbol == SYMBOL_USD)
		return new Currency(USD(0));
	if (symbol == SYMBOL_GBP)
		return new Currency(GBP(0));
	if (symbol == SYMBOL_CNY)
		return new Currency(CNY(0));
	if (symbol == SYMBOL_RUB)
		return new Currency(RUB(0));
	if (symbol == SYMBOL_JPY)
		return new Currency(JPY(0));
	if (symbol == SYMBOL_XBT)
		return new Currency(XBT(0));
	if (symbol == SYMBOL_ETH)
		return new Currency(ETH(0));
	if (symbol == SYMBOL_SEK)
		return new Currency(SEK(0));

	it_happened();
	return nullptr;
}

}}
