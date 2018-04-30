#pragma once

#include "../decl.hxx"
#include "../err.hpp"
#include "../global.hxx"

#include <QColor>

namespace ods { // ods::
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
