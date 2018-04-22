#include "TextP.hpp"

#include "TextDate.hpp"
#include "TextPageCount.hpp"
#include "TextPageNumber.hpp"
#include "TextS.hpp"
#include "TextSheetName.hpp"
#include "TextTime.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

TextP::TextP(Abstract *parent, ods::Tag *tag)
: Abstract(parent, parent->ns(), id::TextP)
{
	if (tag != nullptr)
		Init(tag);
}

TextP::TextP(const TextP &cloner) : Abstract(cloner)
{}

TextP::~TextP()
{}

void
TextP::AppendString(const QString &s)
{
	for (StringOrInst *next : nodes_)
	{
		if (next->is_string())
		{
			next->AppendString(s);
			return;
		}
	}
	
	Append(s);
}

Abstract*
TextP::Clone(Abstract *parent) const
{
	auto *p = new TextP(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	return p;
}

QString*
TextP::GetFirstString() const
{
	for (StringOrInst *x: nodes_)
	{
		if (x->is_string())
			return x->as_string();
	}
	
	return nullptr;
}

void
TextP::Init(ods::Tag *tag)
{
	Scan(tag);
}

void
TextP::Scan(ods::Tag *tag)
{
	for (auto *x: tag->nodes())
	{
		if (AddText(x))
			continue;
		
		auto *next = x->as_tag();
		
		if (next->Is(ns_->text(), ods::ns::kPageNumber)) {
			Append(new TextPageNumber(this, next));
		} else if (next->Is(ns_->text(), ods::ns::kS)) {
			Append(new TextS(this, next));
		} else if (next->Is(ns_->text(), ods::ns::kSheetName)) {
			Append(new TextSheetName(this, next));
		} else if (next->Is(ns_->text(), ods::ns::kPageCount)) {
			Append(new TextPageCount(this, next));
		} else if (next->Is(ns_->text(), ods::ns::kTime)) {
			Append(new TextTime(this, next));
		} else if (next->Is(ns_->text(), ods::ns::kDate)) {
			Append(new TextDate(this, next));
		} else {
			Scan(next);
		}
	}
}

void
TextP::SetFirstString(const QString &s)
{
	for (auto *next : nodes_)
	{
		if (next->is_string())
		{
			next->SetString(s);
			return;
		}
	}
	
	Append(s);
}

void
TextP::WriteData(QXmlStreamWriter &xml)
{
	WriteNodes(xml);
}

} // ods::inst::
} // ods::
