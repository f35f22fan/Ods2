#include "VisualBreak.hpp"

namespace ods::attr {

const QString auto_str = QLatin1String("auto");
const QString column_str = QLatin1String("column");
const QString page_str = QLatin1String("page");

VisualBreak::VisualBreak() {}

VisualBreak::~VisualBreak()
{
}

VisualBreak*
VisualBreak::Clone() const
{
	auto *p = new VisualBreak();
	p->break_type_ = break_type_;
	p->before_ = before_;
	
	return p;
}

VisualBreak*
VisualBreak::FromString(const QString &str)
{
	if (str.isEmpty())
		return nullptr;
	
	auto *vb = new VisualBreak();
	if (str == auto_str) {
		vb->break_type(BreakType::Auto);
	} else if (str == page_str) {
		vb->break_type(BreakType::Page);
	} else if (str == column_str) {
		vb->break_type(BreakType::Column);
	}
	
	return vb;
}

QString
VisualBreak::toString() const
{
	switch (break_type_) {
	case BreakType::Auto: return auto_str;
	case BreakType::Column: return column_str;
	case BreakType::Page: return page_str;
	}
	
	mtl_trace();
	return QString();
}

} // ods::attr::
