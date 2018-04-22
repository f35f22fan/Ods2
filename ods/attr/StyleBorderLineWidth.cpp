#include "StyleBorderLineWidth.hpp"

#include "../Length.hpp"

namespace ods { // ods::
namespace attr { // ods::attr::

StyleBorderLineWidth::StyleBorderLineWidth() {}

StyleBorderLineWidth::~StyleBorderLineWidth() {}

StyleBorderLineWidth*
StyleBorderLineWidth::Clone() const
{
	auto *p = new StyleBorderLineWidth();
	
	for (auto *item: widths_)
		p->widths_.append(item->Clone());
	
	return p;
}

StyleBorderLineWidth*
StyleBorderLineWidth::FromString(const QString &str)
{
	if (str.isEmpty())
		return nullptr;
	
	QStringList list = str.split(' ');
	
	if (list.isEmpty())
		return nullptr;
	
	auto *p = new StyleBorderLineWidth();
	
	for (const auto &next: list)
	{
		if (next.isEmpty())
			continue;
		
		auto *length = ods::Length::FromString(next);
		
		if (length != nullptr)
			p->widths_.append(length);
	}
	
	return p;
}

QString
StyleBorderLineWidth::toString() const
{
	QString str;
	const int max = widths_.size();
	
	for (int i = 0; i < max; i++)
	{
		auto *item = widths_[i];
		
		if (item == nullptr)
			continue;
		
		str.append(item->toString());
		
		if (i < max - 1)
			str.append(' ');
	}
	
	return str;
}

} // ods::attr::
} // ods::
