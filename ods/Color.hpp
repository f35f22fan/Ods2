#pragma once

#include "decl.hxx"
#include "global.hxx"
#include "err.hpp"
#include "types.hxx"

#include <QColor>

namespace ods {

enum class ODS_API ColorSet: i8 {
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
	void Clear() { set_ = ColorSet::None; }
	const QColor& color() const { return color_; }
	void color(const QColor &c) {
		color_ = c;
		set_ = ColorSet::Color;
	}
	ColorSet color_set() const { return set_; }
	QString toString() const;
	bool transparent() const { return set_ == ColorSet::Transparent; }
	
private:
	
	QColor color_ = {};
	ColorSet set_ = ColorSet::None;
};


}
