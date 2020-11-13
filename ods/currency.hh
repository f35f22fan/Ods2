#pragma once

#include "global.hxx"
#include "currency.hxx"

namespace ods {
namespace currency {
ODS_API CurrencyInfo info(const Currency &c);

ODS_API Currency USD(double d);
ODS_API Currency EUR(double d);
ODS_API Currency GBP(double d);
ODS_API Currency CNY(double d);
ODS_API Currency RUB(double d);
ODS_API Currency JPY(double d);

}
}
