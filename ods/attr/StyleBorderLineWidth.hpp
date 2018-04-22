#pragma once

#include "../decl.hxx"
#include "../err.hpp"

namespace ods { // ods::
namespace attr { // ods::attr::

class StyleBorderLineWidth
{
public:
	StyleBorderLineWidth();
	virtual ~StyleBorderLineWidth();
	
	StyleBorderLineWidth*
	Clone() const;
	
	static StyleBorderLineWidth*
	FromString(const QString &str);
	
	QString
	toString() const;
	
	QVector<Length*>&
	widths() { return widths_; }

private:
	NO_ASSIGN_COPY_MOVE(StyleBorderLineWidth);
	
	QVector<Length*> widths_;
};

} // ods::attr::
} // ods::
