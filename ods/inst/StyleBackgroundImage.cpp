#include "StyleBackgroundImage.hpp"

#include "../Ns.hpp"

#include "../ndff/Container.hpp"
#include "../ndff/Property.hpp"

namespace ods::inst {

StyleBackgroundImage::StyleBackgroundImage(Abstract *parent, Tag *tag, ndff::Container *cntr)
: Abstract(parent, parent->ns(), id::StyleBackgroundImage)
{
	if (cntr)
		Init(cntr);
	else if (tag)
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

void StyleBackgroundImage::Init(ndff::Container *cntr)
{
	ndff(true);
	using Op = ndff::Op;
	ndff::Property prop;
	QHash<UriId, QVector<ndff::Property>> attrs;
	Op op = cntr->Next(prop, Op::TS, &attrs);
	if (op == Op::N32_TE)
		return;
	
	if (op == Op::TCF_CMS)
		op = cntr->Next(prop, op);
	
	while (true)
	{
		if (op == Op::TS)
		{
			if (prop.is(ns_->style()))
			{
//				if (prop.name == ns::kBackgroundImage)
//					Append(new StyleBackgroundImage(this, 0, cntr), TakeOwnership::Yes);
			}
		} else if (ndff::is_text(op)) {
			Append(cntr->NextString());
		} else {
			break;
		}
		op = cntr->Next(prop, op);
	}
	
	if (op != Op::SCT)
		mtl_trace("op: %d", op);
}

void StyleBackgroundImage::Init(Tag *tag) {}

void StyleBackgroundImage::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name()}, list);
}

void StyleBackgroundImage::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->style(), list);
}

void StyleBackgroundImage::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
