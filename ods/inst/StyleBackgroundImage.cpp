#include "StyleBackgroundImage.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

StyleBackgroundImage::StyleBackgroundImage(Abstract *parent, Tag *tag)
: Abstract(parent, parent->ns(), id::StyleBackgroundImage)
{
	if (tag != nullptr)
		Init(tag);
}

StyleBackgroundImage::StyleBackgroundImage(const StyleBackgroundImage &cloner)
: Abstract(cloner)
{}

StyleBackgroundImage::~StyleBackgroundImage() {}

Abstract*
StyleBackgroundImage::Clone(Abstract *parent) const
{
	auto *p = new StyleBackgroundImage(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	return p;
}

void
StyleBackgroundImage::Init(Tag *tag) {}

void
StyleBackgroundImage::WriteData(QXmlStreamWriter &xml)
{}

} // ods::inst::
} // ods::
