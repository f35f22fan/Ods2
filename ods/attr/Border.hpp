#pragma once

#include "../decl.hxx"
#include "../err.hpp"
#include "../global.hxx"
#include "../types.hxx"

#include <QColor>

namespace ods { // ods::

const u8 BorderTop = 1;
const u8 BorderLeft = 1 << 1;
const u8 BorderRight = 1 << 2;
const u8 BorderBottom = 1 << 3;
const u8 BorderAll = BorderTop | BorderLeft | BorderRight | BorderBottom;

namespace attr { // ods::attr::

class ODS_API Border
{
public:
	Border();
	virtual ~Border();
	
	Border*
	Clone() const;
	
	QColor*
	color() const { return color_; }
	
	void
	color(const QColor *c);
	
	static Border*
	FromString(const QString &str);
	
	ods::LineStyle*
	line_style() const { return line_style_; }
	
	void
	line_style(const ods::LineStyle *ls);
	
	QString
	toString() const;
	
	ods::Length*
	width() const { return width_; }
	
	void
	width(const Length *w);

private:
	NO_ASSIGN_COPY_MOVE(Border);
	
	QColor *color_ = nullptr;
	ods::LineStyle *line_style_ = nullptr;
	ods::Length *width_ = nullptr;
};

} // ods::attr::
} // ods::
