#include "Color.hpp"

#include "ns.hxx"

namespace ods {

Color::Color(): set_(ColorSet::None) {}

Color::Color(const QColor &c)
{
	color_ = c;
	if(color_.isValid() && color_ != QColor())
		set_ = ColorSet::Color;
}

Color::Color(const ColorSet cs)
{
	set_ = cs;
}

Color::Color(const ods::Color &rhs)
{
	set_ = rhs.set_;
	color_ = rhs.color_;
}

Color Color::FromString(QStringView s)
{
	if (QColor::isValidColorName(s)) {
		return Color(QColor(s));
	} else if (s == ns::kTransparent) {
		return Color(ColorSet::Transparent);
	} else {
		return Color(ColorSet::None);
	}
}

QString Color::toString() const
{
	switch (set_)
	{
	case ColorSet::Color:
		return color_.name();
	case ColorSet::Transparent:
		return ns::kTransparent;
	default: return QString();
	}
}


}
