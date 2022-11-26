#include "StyleTextUnderlineColor.hpp"

namespace ods::attr {

static const auto FontColorStr = QStringLiteral("font-color");

StyleTextUnderlineColor::StyleTextUnderlineColor() {}

StyleTextUnderlineColor::~StyleTextUnderlineColor()
{
	delete color_;
}

StyleTextUnderlineColor*
StyleTextUnderlineColor::Clone() const
{
	auto *p = new StyleTextUnderlineColor();
	p->tuc_ = tuc_;
	
	if (color_ != nullptr)
		p->color_ = new QColor(*color_);
	
	return p;
}

StyleTextUnderlineColor*
StyleTextUnderlineColor::FromString(const QString &s)
{
	if (s.isEmpty())
		return nullptr;
	
	if (s == FontColorStr)
	{
		auto *p = new StyleTextUnderlineColor();
		p->SetFontColor();
		return p;
	} else {
		if (QColor::isValidColor(s))
		{
			auto *color = new QColor(s);
			auto *p = new StyleTextUnderlineColor();
			p->SetColor(color);
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
	
	if (is_color() && color_ != nullptr)
		return color_->name();
	
	mtl_it_happened();
	return QLatin1String();
}

} // ods::attr::
