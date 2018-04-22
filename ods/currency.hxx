#pragma once

#include <QtGlobal>
#include <QString>

namespace ods { // ods::
namespace currency { // ods::currency::

namespace str { // ods::currency::str::
const auto USD = QStringLiteral("USD");
const auto EUR = QStringLiteral("EUR");
const auto GB_POUND = QStringLiteral("GBP");
const auto RENMINBI = QStringLiteral("CNY");
const auto RUS_RUBLE = QStringLiteral("RUB");
} // ods::currency::str::

namespace symbol { // ods::currency::symbol::
const auto USD = QStringLiteral("$");
const auto EUR = QStringLiteral("€");
const auto GBP = QStringLiteral("₤");
const auto CNY = QStringLiteral("¥");
const auto RUB = QStringLiteral("₽");
} // ods::currency::symbol::

} // ods::currency::
} // ods::