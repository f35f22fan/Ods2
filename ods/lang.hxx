#pragma once

#include <QString>

namespace ods { // ods::

struct Lang {
	lang::Id id;
	QString str;
};

namespace lang {

enum class Id : quint16 {
	NotSet = 0,
	Arabic,
	Chinese,
	English,
	French,
	German,
	Italian,
	Portuguese,
	Russian,
	Spanish,
	Turkish
};

const Lang Arabic { lang::Id::Arabic, QLatin1String("ar") };
const Lang Chinese { lang::Id::Chinese, QLatin1String("zh") };
const Lang English { lang::Id::English, QLatin1String("en") };
const Lang French { lang::Id::French, QLatin1String("fr") };
const Lang German { lang::Id::German, QLatin1String("de") };
const Lang Italian { lang::Id::Italian, QLatin1String("it") };
const Lang Portuguese { lang::Id::Portuguese, QLatin1String("pt") };
const Lang Russian { lang::Id::Russian, QLatin1String("ru") };
const Lang Spanish { lang::Id::Spanish, QLatin1String("es") };
const Lang Turkish { lang::Id::Turkish, QLatin1String("tr") };

} // ods::lang::
} // ods::