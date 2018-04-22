#include "FoFontStyle.hpp"

namespace ods { // ods::
namespace attr { // ods::attr::

static const auto ItalicStr = QStringLiteral("italic");
static const auto NormalStr = QStringLiteral("normal");
static const auto ObliqueStr = QStringLiteral("oblique");

FoFontStyle::FoFontStyle() {}
FoFontStyle::FoFontStyle(const FontStyle fs)
: font_style_(fs) {}
FoFontStyle::FoFontStyle(const FoFontStyle &fs)
: font_style_(fs.font_style())
{}

FoFontStyle::~FoFontStyle() {}

FoFontStyle*
FoFontStyle::Clone() const
{
	return new FoFontStyle(font_style_);
}

FoFontStyle*
FoFontStyle::FromString(const QString &s)
{
	if (s.isEmpty())
		return nullptr;
	
	if (s == ItalicStr)
		return new FoFontStyle(FontStyle::Italic);
	if (s == NormalStr)
		return new FoFontStyle(FontStyle::Normal);
	if (s == ObliqueStr)
		return new FoFontStyle(FontStyle::Oblique);
	
	return nullptr;
}

QString
FoFontStyle::toString()
{
	switch (font_style_)
	{
	case FontStyle::Normal: return NormalStr;
	case FontStyle::Italic: return ItalicStr;
	case FontStyle::Oblique: return ObliqueStr;
	default: mtl_warn(); return QLatin1String();
	}
}

} // ods::attr::
} // ods::
