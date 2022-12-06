#include "StyleStyle.hpp"

#include "NumberBooleanStyle.hpp"
#include "NumberCurrencyStyle.hpp"
#include "NumberDateStyle.hpp"
#include "NumberNumber.hpp"
#include "NumberPercentageStyle.hpp"
#include "NumberText.hpp"
#include "NumberTimeStyle.hpp"
#include "OfficeAutomaticStyles.hpp"
#include "OfficeDocumentContent.hpp"
#include "StyleParagraphProperties.hpp"
#include "StyleTableCellProperties.hpp"
#include "StyleTableColumnProperties.hpp"
#include "StyleTableProperties.hpp"
#include "StyleTableRowProperties.hpp"
#include "StyleTextProperties.hpp"

#include "../attr/Border.hpp"
#include "../attr/FoFontWeight.hpp"

#include "../Book.hpp"
#include "../LineStyle.hpp"
#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods::inst {

StyleStyle::StyleStyle(Abstract *parent, ods::Tag *tag, ndff::Container *cntr)
: Abstract (parent, parent->ns(), id::StyleStyle)
{
	if (cntr)
		Init(cntr);
	else if (tag)
		Init(tag);
}

StyleStyle::StyleStyle(const StyleStyle &cloner)
: Abstract(cloner)
{
}

StyleStyle::~StyleStyle()
{}

Abstract*
StyleStyle::Clone(Abstract *parent) const
{
	auto *p = new StyleStyle(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->style_data_style_name_ = style_data_style_name_;
	p->style_family_ = style_family_;
	p->style_display_name_ = style_display_name_;
	p->style_master_page_name_ = style_master_page_name_;
	p->style_name_ = style_name_;
	p->style_parent_style_name_ = style_parent_style_name_;
	p->CloneChildrenOf(this);
	
	return p;
}

StyleStyle*
StyleStyle::DeriveCellStyle()
{
	auto *p = book_->NewCellStyle();
	p->SetParentStyle(this);
	return p;
}

NumberCurrencyStyle*
StyleStyle::FetchNumberCurrencyStyle()
{
	auto *tcp = (ods::inst::NumberCurrencyStyle*)
		Get(ods::Id::NumberCurrencyStyle);
	
	if (tcp == nullptr)
		tcp = NewCurrencyStyle();
	
	return tcp;
}

NumberPercentageStyle*
StyleStyle::FetchPercentageStyle()
{
	auto *p = (NumberPercentageStyle*) Get(Id::NumberPercentageStyle);
	
	if (p == nullptr)
		p = NewPercentageStyle();
	
	return p;
}

inst::StyleTableCellProperties*
StyleStyle::FetchStyleTableCellProperties()
{
	auto *p = (StyleTableCellProperties*)Get(Id::StyleTableCellProperties);
	
	if (p == nullptr) {
		p = new StyleTableCellProperties(this);
		Append(p, TakeOwnership::Yes);
	}
	
	return p;
}

inst::StyleTableColumnProperties*
StyleStyle::FetchTableColumnProperties()
{
	auto *p = (StyleTableColumnProperties*)Get(Id::StyleTableColumnProperties);
	
	if (p == nullptr) {
		p = new StyleTableColumnProperties(this);
		Append(p, TakeOwnership::Yes);
	}
	
	return p;
}

StyleTextProperties*
StyleStyle::FetchStyleTextProperties()
{
	auto *p = (StyleTextProperties*)Get(Id::StyleTextProperties);
	
	if (p == nullptr) {
		p = new StyleTextProperties(this);
		Append(p, TakeOwnership::Yes);
	}
	
	return p;
}

StyleTableCellProperties*
StyleStyle::FetchTableCellProperties()
{
	auto *tcp = (ods::inst::StyleTableCellProperties*)
		Get(ods::Id::StyleTableCellProperties);
	
	if (tcp == nullptr)
		tcp = NewTableCellProperties();
	
	return tcp;
}

NumberBooleanStyle*
StyleStyle::GetBooleanStyle() const
{
	auto *ds = GetDataStyle();
	
	if (ds == nullptr)
		return nullptr;
	
	if (ds->Is(Id::NumberBooleanStyle))
		return (inst::NumberBooleanStyle*) ds;
	
	return nullptr;
}

Abstract*
StyleStyle::GetDataStyle() const
{
	if (style_data_style_name_.isEmpty())
		return nullptr;
	
	return GetAnyStyle(style_data_style_name_);
}

NumberCurrencyStyle*
StyleStyle::GetCurrencyStyle() const
{
	auto *ds = GetDataStyle();
	
	if (ds == nullptr)
		return nullptr;
	
	if (ds->Is(Id::NumberCurrencyStyle))
		return (inst::NumberCurrencyStyle*) ds;
	
	return nullptr;
}

NumberDateStyle*
StyleStyle::GetDateStyle() const
{
	auto *ds = GetDataStyle();
	
	if (ds == nullptr)
		return nullptr;
	
	if (ds->Is(Id::NumberDateStyle))
		return (inst::NumberDateStyle*) ds;
	
	return nullptr;
}

NumberPercentageStyle*
StyleStyle::GetPercentageStyle() const
{
	auto *ds = GetDataStyle();
	CHECK_PTR_NULL(ds);

	if (ds->Is(Id::NumberPercentageStyle))
		return (inst::NumberPercentageStyle*) ds;
	
	return nullptr;
}

StyleTableRowProperties*
StyleStyle::GetTableRowProperties() const
{
	auto *p = Get(Id::StyleTableRowProperties);
	
	if (p == nullptr)
		return nullptr;
	
	return (inst::StyleTableRowProperties*)p;
}

NumberTimeStyle*
StyleStyle::GetTimeStyle() const
{
	auto *ds = GetDataStyle();
	
	if (ds == nullptr)
		return nullptr;
	
	if (ds->Is(Id::NumberTimeStyle))
		return (inst::NumberTimeStyle*) ds;
	
	return nullptr;
}

void StyleStyle::Init(ndff::Container *cntr)
{
	using Op = ndff::Op;
	ndff::Property prop;
	QHash<UriId, QVector<ndff::Property>> attrs;
	Op op = cntr->Next(prop, Op::TS, &attrs);
	CopyAttr(attrs, ns_->style(), ns::kDataStyleName, style_data_style_name_);
	QString style_family;
	CopyAttr(attrs, ns_->style(), ns::kFamily, style_family);
	style_family_ = style::FamilyFromString(style_family);
	CopyAttr(attrs, ns_->style(), ns::kMasterPageName, style_master_page_name_);
	CopyAttr(attrs, ns_->style(), ns::kName, style_name_);
	CopyAttr(attrs, ns_->style(), ns::kParentStyleName, style_parent_style_name_);
	CopyAttr(attrs, ns_->style(), ns::kDisplayName, style_display_name_);
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
				if (prop.name == ns::kTableColumnProperties) {
					Append(new StyleTableColumnProperties(this, 0, cntr), TakeOwnership::Yes);
				} else if (prop.name == ns::kTextProperties) {
					Append(new StyleTextProperties(this, 0, cntr), TakeOwnership::Yes);
				} else if (prop.name == ns::kTableCellProperties) {
					Append(new StyleTableCellProperties(this, 0, cntr), TakeOwnership::Yes);
				} else if (prop.name == ns::kTableProperties) {
					Append(new StyleTableProperties(this, 0, cntr), TakeOwnership::Yes);
				} else if (prop.name == ns::kTableRowProperties) {
					Append(new StyleTableRowProperties(this, 0, cntr), TakeOwnership::Yes);
				} else if (prop.name == ns::kParagraphProperties) {
					Append(new StyleParagraphProperties(this, 0, cntr), TakeOwnership::Yes);
				} else {
					mtl_trace();
				}
			} else {
				mtl_trace();
			}
		} else if (ndff::is_text(op)) {
			Append(cntr->NextString());
		} else {
			break;
		}
		
		op = cntr->Next(prop, op);
	}
	
	if (op != Op::SCT)
		mtl_trace("Unexpected op: %d", op);
}

void StyleStyle::Init(ods::Tag *tag)
{
	tag->Copy(ns_->style(), ns::kDataStyleName, style_data_style_name_);
	
	QString str;
	tag->Copy(ns_->style(), ns::kFamily, str);
	style_family_ = style::FamilyFromString(str);
	
	tag->Copy(ns_->style(), ns::kMasterPageName, style_master_page_name_);
	tag->Copy(ns_->style(), ns::kName, style_name_);
	tag->Copy(ns_->style(), ns::kParentStyleName, style_parent_style_name_);
	tag->Copy(ns_->style(), ns::kDisplayName, style_display_name_);
	
	Scan(tag);
}

void StyleStyle::ListKeywords(inst::Keywords &list, const inst::LimitTo lt)
{
	inst::AddKeywords({tag_name(), ns::kDataStyleName,
		ns::kFamily, ns::kMasterPageName, ns::kMasterPageName,
		ns::kName, ns::kParentStyleName, ns::kDisplayName}, list);
}

void StyleStyle::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->style(), list);
}

NumberBooleanStyle*
StyleStyle::NewBooleanStyle()
{
	auto *dc = book_->document_content();
	auto *aus = dc->automatic_styles();
	auto *p = aus->NewNumberBooleanStyle();
	style_data_style_name_ = *p->style_name();
	return p;
}

NumberCurrencyStyle*
StyleStyle::NewCurrencyStyle()
{
	auto *dc = book_->document_content();
	auto *aus = dc->automatic_styles();
	auto *p = aus->NewNumberCurrencyStyle();
	style_data_style_name_ = *p->style_name();
	return p;
}

NumberDateStyle*
StyleStyle::NewDateStyle()
{
	auto *dc = book_->document_content();
	auto *aus = dc->automatic_styles();
	auto *p = aus->NewNumberDateStyle();
	style_data_style_name_ = *p->style_name();
	return p;
}

inst::StyleParagraphProperties*
StyleStyle::NewParagraphProperties()
{
	auto *p = new StyleParagraphProperties(this);
	Append(p, TakeOwnership::Yes);
	return p;
}

NumberPercentageStyle*
StyleStyle::NewPercentageStyle()
{
	auto *dc = book_->document_content();
	auto *aus = dc->automatic_styles();
	auto *p = aus->NewNumberPercentageStyle();
	style_data_style_name_ = *p->style_name();
	return p;
}

StyleTableCellProperties*
StyleStyle::NewTableCellProperties()
{
	auto *p = new StyleTableCellProperties(this);
	Append(p, TakeOwnership::Yes);
	return p;
}

StyleTableColumnProperties*
StyleStyle::NewTableColumnProperties()
{
	auto *tcp = new StyleTableColumnProperties(this);
	Append(tcp, TakeOwnership::Yes);
	return tcp;
}

StyleTableRowProperties*
StyleStyle::NewTableRowProperties()
{
	auto *p = new StyleTableRowProperties(this);
	Append(p, TakeOwnership::Yes);
	return p;
}

NumberTimeStyle*
StyleStyle::NewTimeStyle()
{
	auto *dc = book_->document_content();
	auto *aus = dc->automatic_styles();
	auto *p = aus->NewNumberTimeStyle();
	style_data_style_name_ = *p->style_name();
	return p;
}

StyleFontFace*
StyleStyle::QueryFontFace() const
{
	auto *tp = (inst::StyleTextProperties*)Get(Id::StyleTextProperties);
	
	if (tp == nullptr)
		return nullptr;
	
	return tp->font_face();
}

void StyleStyle::Scan(ods::Tag *tag)
{
	for (auto *x: tag->nodes())
	{
		if (AddText(x))
			continue;

		auto *next = x->as_tag();
		
		if (next->Is(ns_->style(), ns::kTextProperties))
		{
			Append(new StyleTextProperties(this, next), TakeOwnership::Yes);
		} else if (next->Is(ns_->style(), ns::kTableCellProperties)) {
			Append(new StyleTableCellProperties(this, next), TakeOwnership::Yes);
		} else if (next->Is(ns_->style(), ns::kTableColumnProperties)) {
			Append(new StyleTableColumnProperties(this, next), TakeOwnership::Yes);
		} else if (next->Is(ns_->style(), ns::kTableProperties)) {
			Append(new StyleTableProperties(this, next), TakeOwnership::Yes);
		} else if (next->Is(ns_->style(), ns::kTableRowProperties)) {
			Append(new StyleTableRowProperties(this, next), TakeOwnership::Yes);
		} else if (next->Is(ns_->style(), ns::kParagraphProperties)) {
			Append(new StyleParagraphProperties(this, next), TakeOwnership::Yes);
		} else {
			Scan(next);
		}
	}
}

void StyleStyle::SetBackgroundColor(const QColor &color)
{
	// set background color:
	auto *stcp = FetchStyleTableCellProperties();
	stcp->SetBackgroundColor(color);
}

void StyleStyle::SetBoldText(const bool bold)
{
	ods::inst::StyleTextProperties *tp = FetchStyleTextProperties();

	// set font weight:
	auto *font_weight = new ods::attr::FoFontWeight();
	
	if (bold)
		font_weight->SetBold();
	else
		font_weight->SetNormal();
	
	tp->SetFontWeight(font_weight);
}

void StyleStyle::SetBorder(const ods::Length &width, const QColor &color,
	const ods::line::Style &line_style, const u8 sides)
{
	auto *tcp = FetchTableCellProperties();
	ods::attr::Border border;
	border.width(&width);
	border.color(&color);
	ods::LineStyle line_style_class;
	line_style_class.Set(line_style);
	border.line_style(&line_style_class);
	
	if (sides == ods::BorderAll) {
		tcp->border(&border);
		return;
	}
	
	if (sides & ods::BorderLeft)
		tcp->border_left(&border);
	if (sides & ods::BorderRight)
		tcp->border_right(&border);
	if (sides & ods::BorderTop)
		tcp->border_top(&border);
	if (sides & ods::BorderBottom)
		tcp->border_bottom(&border);
}

void StyleStyle::SetDataStyle(NumberCurrencyStyle *p) {
	style_data_style_name_ = *p->style_name();
}

void StyleStyle::SetFamily(const style::Family f)
{
	style_family_ = f;
}

void StyleStyle::SetFontStyle(const ods::attr::FontStyle font_style)
{
	ods::inst::StyleTextProperties *tp = FetchStyleTextProperties();
	auto *fo_font_style = new ods::attr::FoFontStyle(font_style);
	tp->SetFontStyle(fo_font_style);
}

void StyleStyle::SetHAlignment(const HAlignSide place)
{
	auto *spp = (ods::inst::StyleParagraphProperties*)
		Get(ods::Id::StyleParagraphProperties);
	
	if (spp == nullptr)
		spp = NewParagraphProperties();
	
	ods::HAlign ha(place);
	spp->text_align(&ha);
}

void StyleStyle::SetVAlignment(const VAlignSide place)
{
	auto *tcp = (ods::inst::StyleTableCellProperties*)
		Get(ods::Id::StyleTableCellProperties);
	
	if (tcp == nullptr)
		tcp = NewTableCellProperties();
	
	ods::VAlign va(place);
	tcp->vertical_align(&va);
}

void StyleStyle::SetMasterPageName(const QString &s)
{
	style_master_page_name_ = s;
}

void StyleStyle::SetName(const QString &s)
{
	style_name_ = s;
}

void StyleStyle::SetParentStyle(StyleStyle *s)
{
	SetParentStyleName(*s->style_name());
}

void StyleStyle::SetParentStyleName(const QString &s)
{
	style_parent_style_name_ = s;
}

void StyleStyle::SetPercentage(const int min_integer_digits, const int decimal_places)
{
	ods::inst::NumberPercentageStyle *percent_style = FetchPercentageStyle();
	auto *ns = percent_style->FetchNumberStyle();
	ns->min_integer_digits(min_integer_digits);
	ns->decimal_places(decimal_places);
	// For example:
	// min_integer_digits=3 and decimal_places=4 makes 0.83 display as "083.0000%"
	// Note: Calligra Sheets doesn't fully support percentage formatting
	
	percent_style->FetchNumberText()->SetString(QString("%"));
}

void StyleStyle::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->style(), ns::kDataStyleName, style_data_style_name_);
	
	if (style_family_ != style::Family::None)
	{
		auto str = style::FamilyToString(style_family_);
		Write(xml, ns_->style(), ns::kFamily, str);
	}
	
	Write(xml, ns_->style(), ns::kMasterPageName, style_master_page_name_);
	Write(xml, ns_->style(), ns::kName, style_name_);
	Write(xml, ns_->style(), ns::kParentStyleName, style_parent_style_name_);
	Write(xml, ns_->style(), ns::kDisplayName, style_display_name_);
	
	WriteNodes(xml);
}

void StyleStyle::WriteNDFF(inst::NsHash &h, inst::Keywords &kw, QFileDevice *file, ByteArray *ba)
{
	CHECK_TRUE_VOID(ba != nullptr);
	WriteTag(kw, *ba);
	
	WriteNdffProp(kw, *ba, ns_->style(), ns::kDataStyleName, style_data_style_name_);
	
	if (style_family_ != style::Family::None)
	{
		auto str = style::FamilyToString(style_family_);
		WriteNdffProp(kw, *ba, ns_->style(), ns::kFamily, str);
	}
	
	WriteNdffProp(kw, *ba, ns_->style(), ns::kMasterPageName, style_master_page_name_);
	WriteNdffProp(kw, *ba, ns_->style(), ns::kName, style_name_);
	WriteNdffProp(kw, *ba, ns_->style(), ns::kParentStyleName, style_parent_style_name_);
	WriteNdffProp(kw, *ba, ns_->style(), ns::kDisplayName, style_display_name_);
	CloseBasedOnChildren(h, kw, file, ba);
}

} // ods::inst::
