#pragma once

#include "Abstract.hpp"
#include "decl.hxx"

#include "../err.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

class ODS_API DrawFrame : public Abstract
{
public:
	DrawFrame(Abstract *parent, ods::Tag *tag = nullptr);
	DrawFrame(const DrawFrame &cloner);
	
	virtual ~DrawFrame();
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	Length*
	height() const { return svg_height_; }
	
	void
	height(const Length *l);
	
	inst::DrawImage*
	NewDrawImage();
	
	inst::SvgDesc*
	NewSvgDesc();
	
	inst::SvgTitle*
	NewSvgTitle();
	
	Length*
	width() const { return svg_width_; }
	
	void
	width(const Length *l);
	
	Length*
	x() const { return svg_x_; }
	
	void
	x(const Length *l);
	
	Length*
	y() const { return svg_y_; }
	
	void
	y(const Length *l);
	
	void
	WriteData(QXmlStreamWriter &xml) override;

private:
	
	void Init(ods::Tag*);
	void Scan(ods::Tag*);
	
	Length *svg_x_ = nullptr;
	Length *svg_y_ = nullptr;
	Length *svg_height_ = nullptr;
	Length *svg_width_ = nullptr;
	
	QString draw_z_index_;
	QString draw_id_;
	QString draw_name_;
	QString style_rel_width_;
	QString style_rel_height_;
};

} // ods::inst::
} // ods::
