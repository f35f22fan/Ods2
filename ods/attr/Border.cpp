#include "Border.hpp"

#include "../Length.hpp"
#include "../LineStyle.hpp"

namespace ods::attr {

Border::Border() {}

Border::~Border()
{
	delete line_style_;
	delete width_;
}

Border*
Border::Clone() const
{
	auto *p = new Border();
	p->color_ = color_;
	
	if (line_style_ != nullptr)
		p->line_style_ = line_style_->Clone();
	
	if (width_ != nullptr)
		p->width_ = width_->Clone();
	
	return p;
}

void Border::color(const ods::Color &c)
{
	color_ = c;
}

Border*
Border::FromString(const QString &str)
{
	if (str.isEmpty())
		return nullptr;
	
	QStringList list = str.split(' ');
	
	if (list.isEmpty())
		return nullptr;
	
	auto *border = new ods::attr::Border();
	
	for (const QString &next: list)
	{
		if (next.isEmpty())
			continue;
		
		if (next.startsWith('#'))
		{
			border->color_ = ods::Color::FromString(next);
			continue;
		}
		
		if (next.at(0).isNumber())
		{
			border->width_ = ods::Length::FromString(next);
		} else {
			border->line_style_ = ods::LineStyle::FromString(next);
		}
	}
	
	return border;
}

void
Border::line_style(const ods::LineStyle *ls)
{
	delete line_style_;
	
	if (ls == nullptr)
		line_style_ = nullptr;
	else
		line_style_ = ls->Clone();
}

QString
Border::toString() const
{
	QString str;
	
	if (width_ != nullptr)
		str.append(width_->toString());
	
	if (line_style_ != nullptr)
	{
		if (width_ != nullptr)
			str.append(' ');
		
		str.append(line_style_->toString());
	}
	
	if (color_.any())
	{
		if (width_ != nullptr || line_style_ != nullptr)
			str.append(' ');
		
		str.append(color_.toString());
	}
	
	return str;
}

void
Border::width(const Length *w)
{
	delete width_;
	
	if (w == nullptr)
		width_ = nullptr;
	else
		width_ = w->Clone();
}

} // ods::attr::
