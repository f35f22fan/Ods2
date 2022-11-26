#include "currency.hh"

#include "err.hpp"

namespace ods {

const QString StrEUR = QStringLiteral("EUR");
const QString StrUSD = QStringLiteral("USD");
const QString StrGBP = QStringLiteral("GBP");
const QString StrCNY = QStringLiteral("CNY");
const QString StrRUB = QStringLiteral("RUB");
const QString StrJPY = QStringLiteral("JPY");
const QString StrXBT = QStringLiteral("XBT");
const QString StrETH = QStringLiteral("ETH");
const QString StrSEK = QStringLiteral("SEK"); // Swedish krona

Currency*
Currency::Clone() {
	auto *p = new Currency();
	p->id = id;
	p->qtty = qtty;
	return p;
}

namespace currency {

static const QString SYMBOL_EUR = QStringLiteral("€");
static const QString SYMBOL_USD = QStringLiteral("$");
static const QString SYMBOL_GBP = QStringLiteral("₤");
static const QString SYMBOL_CNY = QStringLiteral("¥");
static const QString SYMBOL_RUB = QStringLiteral("₽");
static const QString SYMBOL_JPY = QStringLiteral("¥");
static const QString SYMBOL_XBT = QStringLiteral("₿");
static const QString SYMBOL_ETH = QStringLiteral("Ξ");
static const QString SYMBOL_SEK = QStringLiteral("kr"); // Swedish krona

CurrencyInfo info(const CurrencyId &id)
{
	switch (id) {
	case CurrencyId::EUR: return CurrencyInfo{StrEUR, SYMBOL_EUR};
	case CurrencyId::USD: return CurrencyInfo{StrUSD, SYMBOL_USD};
	case CurrencyId::GBP: return CurrencyInfo{StrGBP, SYMBOL_GBP};
	case CurrencyId::CNY: return CurrencyInfo{StrCNY, SYMBOL_CNY};
	case CurrencyId::RUB: return CurrencyInfo{StrRUB, SYMBOL_RUB};
	case CurrencyId::JPY: return CurrencyInfo{StrJPY, SYMBOL_JPY};
	case CurrencyId::XBT: return CurrencyInfo{StrXBT, SYMBOL_XBT};
	case CurrencyId::ETH: return CurrencyInfo{StrETH, SYMBOL_ETH};
	case CurrencyId::SEK: return CurrencyInfo{StrSEK, SYMBOL_SEK};
	default: {
		mtl_trace();
		return {QString(), QString()};
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
//	auto cba = country.toLocal8Bit();
//	auto sba = symbol.toLocal8Bit();
//	mtl_info("Country: \"%s\", symbol: \"%s\"", cba.data(), sba.data());

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

	mtl_it_happened();
	return nullptr;
}

}}
