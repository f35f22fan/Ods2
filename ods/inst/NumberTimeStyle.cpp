#include "NumberTimeStyle.hpp"

#include "NumberDay.hpp"
#include "NumberHours.hpp"
#include "NumberMinutes.hpp"
#include "NumberMonth.hpp"
#include "NumberSeconds.hpp"
#include "NumberText.hpp"
#include "NumberYear.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods::inst {

NumberTimeStyle::NumberTimeStyle(Abstract *parent, Tag *tag) :
Abstract(parent, parent->ns(), id::NumberTimeStyle)
{
	if (tag != nullptr)
		Init(tag);
}

NumberTimeStyle::NumberTimeStyle(const NumberTimeStyle &cloner)
: Abstract(cloner)
{}

NumberTimeStyle::~NumberTimeStyle()
{}

Abstract*
NumberTimeStyle::Clone(Abstract *parent) const
{
	auto *p = new NumberTimeStyle(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->style_name_ = style_name_;
	p->CloneChildrenOf(this);
	
	return p;
}

void NumberTimeStyle::Init(ods::Tag *tag)
{
	tag->Copy(ns_->style(), ods::ns::kName, style_name_);
	Scan(tag);
}

void NumberTimeStyle::ListKeywords(inst::Keywords &list, const inst::LimitTo lt)
{
	inst::AddKeywords({tag_name(), ns::kName}, list);
}

void NumberTimeStyle::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->number(), list);
	Add(ns_->style(), list);
}

NumberDay*
NumberTimeStyle::NewDay()
{
	auto *p = new NumberDay(this);
	Append(p);
	return p;
}

NumberHours*
NumberTimeStyle::NewHours()
{
	auto *p = new NumberHours(this);
	Append(p);
	return p;
}

NumberMinutes*
NumberTimeStyle::NewMinutes()
{
	auto *p = new NumberMinutes(this);
	Append(p);
	return p;
}

NumberMonth*
NumberTimeStyle::NewMonth()
{
	auto *p = new NumberMonth(this);
	Append(p);
	return p;
}

NumberSeconds*
NumberTimeStyle::NewSeconds()
{
	auto *p = new NumberSeconds(this);
	Append(p);
	return p;
}

NumberText*
NumberTimeStyle::NewText(const QString &s)
{
	auto *p = new NumberText(this);
	p->SetFirstString(s);
	Append(p);
	return p;
}

NumberYear*
NumberTimeStyle::NewYear()
{
	auto *p = new NumberYear(this);
	Append(p);
	return p;
}

void NumberTimeStyle::Scan(ods::Tag *scan_tag)
{
	foreach (auto *x, scan_tag->nodes())
	{
		if (AddText(x))
			continue;
		
		auto *tag = x->as_tag();
		
		if (tag->Has(ns_->number()))
		{
			if (tag->Has(ods::ns::kDay))
				Append(new NumberDay(this, tag));
			else if (tag->Has(ods::ns::kHours))
				Append(new NumberHours(this, tag));
			else if (tag->Has(ods::ns::kMinutes))
				Append(new NumberMinutes(this, tag));
			else if (tag->Has(ods::ns::kMonth))
				Append(new NumberMonth(this, tag));
			else if (tag->Has(ods::ns::kSeconds))
				Append(new NumberSeconds(this, tag));
			else if (tag->Has(ods::ns::kText))
				Append(new NumberText(this, tag));
			else if (tag->Has(ods::ns::kYear))
				Append(new NumberYear(this, tag));
		} else {
			Scan(tag);
		}
	}
}

void NumberTimeStyle::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->style(), ods::ns::kName, style_name_);
	WriteNodes(xml);
}

void NumberTimeStyle::WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba)
{
	CHECK_TRUE_VOID(ba != nullptr);
	WriteTag(kw, *ba);
	WriteNdffProp(kw, *ba, ns_->style(), ods::ns::kName, style_name_);
	CloseBasedOnChildren(h, kw, file, ba);
}

} // ods::inst::
