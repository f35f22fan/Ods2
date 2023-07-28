#include "StyleTextUnderlineColor.hpp"

namespace ods::attr {

static const auto FontColorStr = QStringLiteral("font-color");

StyleTextUnderlineColor::StyleTextUnderlineColor() {}

StyleTextUnderlineColor::~StyleTextUnderlineColor() {}

StyleTextUnderlineColor*
StyleTextUnderlineColor::Clone() const
{
	auto *p = new StyleTextUnderlineColor();
	p->tuc_ = tuc_;
	p->color_ = color_;
	
	return p;
}

StyleTextUnderlineColor*
StyleTextUnderlineColor::FromString(const QString &str)
{
	if (str.isEmpty())
		return nullptr;
	
	if (str == FontColorStr)
	{
		auto *p = new StyleTextUnderlineColor();
		p->SetFontColor();
		return p;
	} else {
		ods::Color c = ods::Color::FromString(str);
		if (c.any())
		{
			auto *p = new StyleTextUnderlineColor();
			p->SetColor(c);
			return p;
		}
	}
	
	return nullptr;
}

QString
StyleTextUnderlineColor::toString()
{
	if (is_font_color())
		return FontColorStr;
	
	if (is_color() && color_.any())
		return color_.toString();
	
	mtl_it_happened();
	return QLatin1String();
}

} // ods::attr::
