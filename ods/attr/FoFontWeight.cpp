#include "FoFontWeight.hpp"

namespace ods { // ods::
namespace attr { // ods::attr::

static const auto NormalStr = QStringLiteral("normal");
static const auto BoldStr = QStringLiteral("bold");
static const auto N100Str = QStringLiteral("100");
static const auto N200Str = QStringLiteral("200");
static const auto N300Str = QStringLiteral("300");
static const auto N400Str = QStringLiteral("400");
static const auto N500Str = QStringLiteral("500");
static const auto N600Str = QStringLiteral("600");
static const auto N700Str = QStringLiteral("700");
static const auto N800Str = QStringLiteral("800");
static const auto N900Str = QStringLiteral("900");

FoFontWeight::FoFontWeight() {}
FoFontWeight::FoFontWeight(const FontWeight fw)
: font_weight_(fw) {}

FoFontWeight::~FoFontWeight() {}

FoFontWeight*
FoFontWeight::Clone() const
{
	return new FoFontWeight(font_weight_);
}

FoFontWeight*
FoFontWeight::FromString(const QString &s)
{
	if (s.isEmpty())
		return nullptr;
	
	if (s == NormalStr)
		return new FoFontWeight(FontWeight::Normal);
	if (s == BoldStr)
		return new FoFontWeight(FontWeight::Bold);
	if (s == N100Str)
		return new FoFontWeight(FontWeight::N100);
	if (s == N200Str)
		return new FoFontWeight(FontWeight::N200);
	if (s == N300Str)
		return new FoFontWeight(FontWeight::N300);
	if (s == N400Str)
		return new FoFontWeight(FontWeight::N400);
	if (s == N500Str)
		return new FoFontWeight(FontWeight::N500);
	if (s == N600Str)
		return new FoFontWeight(FontWeight::N600);
	if (s == N700Str)
		return new FoFontWeight(FontWeight::N700);
	if (s == N800Str)
		return new FoFontWeight(FontWeight::N800);
	if (s == N900Str)
		return new FoFontWeight(FontWeight::N900);
	
	return nullptr;
}

QString
FoFontWeight::toString()
{
	switch (font_weight_)
	{
	case FontWeight::Normal: return NormalStr;
	case FontWeight::Bold: return BoldStr;
	case FontWeight::N100: return N100Str;
	case FontWeight::N200: return N200Str;
	case FontWeight::N300: return N300Str;
	case FontWeight::N400: return N400Str;
	case FontWeight::N500: return N500Str;
	case FontWeight::N600: return N600Str;
	case FontWeight::N700: return N700Str;
	case FontWeight::N800: return N800Str;
	case FontWeight::N900: return N900Str;
	default: mtl_warn(); return QLatin1String();
	}
}

} // ods::attr::
} // ods::
