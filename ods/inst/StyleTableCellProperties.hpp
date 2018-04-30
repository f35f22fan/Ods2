#pragma once

#include "Abstract.hpp"
#include "../attr/decl.hxx"
#include "decl.hxx"
#include "../decl.hxx"
#include "../err.hpp"

#include <QColor>

namespace ods { // ods::
namespace inst { // ods::inst::

class ODS_API StyleTableCellProperties : public Abstract
{
public:
	StyleTableCellProperties(Abstract *parent, ods::Tag *tag = nullptr);
	StyleTableCellProperties(const StyleTableCellProperties &cloner);
	virtual ~StyleTableCellProperties();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	QColor*
	background_color() const { return fo_background_color_; }
	
	ods::attr::Border*
	border() const { return fo_border_; }
	
	void
	border(ods::attr::Border *border);
	
	ods::attr::Border*
	border_left() const { return fo_border_left_; }
	
	void
	border_left(ods::attr::Border *border);
	
	ods::attr::Border*
	border_top() const { return fo_border_top_; }
	
	void
	border_top(ods::attr::Border *border);
	
	ods::attr::Border*
	border_right() const { return fo_border_right_; }
	
	void
	border_right(ods::attr::Border *border);
	
	ods::attr::Border*
	border_bottom() const { return fo_border_bottom_; }
	
	void
	border_bottom(ods::attr::Border *border);
	
	void
	SetBackgroundColor(const QColor &c);
	
	void
	SetWrapOption(const bool flag);
	
	ods::attr::StyleBorderLineWidth*
	style_border_line_width() const { return style_border_line_width_; }
	
	void
	style_border_line_width(const ods::attr::StyleBorderLineWidth *p);
	
	VAlign*
	vertical_align() const { return style_vertical_align_; }
	
	void
	vertical_align(VAlign *p);
	
	bool
	wrap_option() const { return fo_wrap_option_ == QLatin1String("wrap"); }
	
	void
	WriteData(QXmlStreamWriter &xml) override;
	
private:
	
	void Init(ods::Tag *tag);
	
	QColor *fo_background_color_ = nullptr;
	ods::attr::Border *fo_border_ = nullptr;
	ods::attr::Border *fo_border_left_ = nullptr;
	ods::attr::Border *fo_border_top_ = nullptr;
	ods::attr::Border *fo_border_right_ = nullptr;
	ods::attr::Border *fo_border_bottom_ = nullptr;
	ods::attr::StyleBorderLineWidth *style_border_line_width_ = nullptr;
	// fo:wrap-option="wrap"
	QString fo_wrap_option_;
	QString style_diagonal_bl_tr_;
	QString style_diagonal_tl_br_;
	
	VAlign *style_vertical_align_ = nullptr;
};

} // ods::inst::
} // ods::
