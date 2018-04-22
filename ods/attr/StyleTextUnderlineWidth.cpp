#include "StyleTextUnderlineWidth.hpp"

namespace ods { // ods::
namespace attr { // ods::attr::

static const auto AutoStr = QStringLiteral("auto");
static const auto BoldStr = QStringLiteral("bold");

StyleTextUnderlineWidth::StyleTextUnderlineWidth() {}

StyleTextUnderlineWidth::~StyleTextUnderlineWidth() {}

StyleTextUnderlineWidth*
StyleTextUnderlineWidth::Clone() const
{
	auto *p = new StyleTextUnderlineWidth();
	p->tuw_ = tuw_;
	p->data_ = data_;
	return p;
}

StyleTextUnderlineWidth*
StyleTextUnderlineWidth::FromString(const QString &s)
{
	if (s.isEmpty())
		return nullptr;
	
	if (s == AutoStr)
	{
		auto *p = new StyleTextUnderlineWidth();
		p->SetAuto();
		return p;
	} else if (s == BoldStr) {
		auto *p = new StyleTextUnderlineWidth();
		p->SetBold();
		return p;
	} else {
		tbd();
	}
	
	return nullptr;
}

QString
StyleTextUnderlineWidth::toString()
{
	switch (tuw_)
	{
	case TextUnderlineWidth::Auto: return AutoStr;
	case TextUnderlineWidth::Bold: return BoldStr;
	default: tbd(); return QLatin1String();
	}
}

} // ods::attr::
} // ods::
