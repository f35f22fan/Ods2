#pragma once

#include "decl.hxx"
#include "global.hxx"
#include "err.hpp"
#include "types.hxx"

#include <QColor>

namespace ods {

enum class ColorSet: i8 {
	Color,
	Transparent,
	None
};

class ODS_API Color {
public:
	Color();
	Color(const QColor &c);
	Color(const ColorSet cs);
	Color(const ods::Color &rhs);
	static Color FromString(QStringView s);
	
	bool any() const { return set_ != ColorSet::None; }
	QString toString() const;
	
private:
	
	QColor color_ = {};
	ColorSet set_ = ColorSet::None;
};


}
