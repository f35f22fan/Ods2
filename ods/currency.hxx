#pragma once

#include <QtGlobal>
#include <QString>

namespace ods { // ods::

struct Currency {
	QString str;
	QString symbol;
};

namespace currency {
const auto USD = Currency { QLatin1String("USD"), "$" };
const auto EUR = Currency { QLatin1String("EUR"), "€" };
const auto GB_POUND = Currency { QLatin1String("GBP"), "₤" };
const auto RENMINBI = Currency { QLatin1String("CNY"), "¥" };
const auto RUS_RUBLE = Currency { QLatin1String("RUB"), "₽" };
}}
