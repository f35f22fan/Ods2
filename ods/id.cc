#include "id.hh"

#include "Ns.hpp"
#include "ns.hxx"
#include "ods.hxx"
#include "Prefix.hpp"

#include "inst/Abstract.hpp"

namespace ods { // ods::
namespace id { // ods::id::

void
DcDate(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->manifest());
	a->tag_name(QLatin1String(ods::ns::kManifest));
	a->func(DcDate);
	a->id(Id::DcDate);
}

void
DcTitle(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->dc());
	a->tag_name(QLatin1String(ods::ns::kTitle));
	a->func(DcTitle);
	a->id(Id::DcTitle);
}

void
DrawFrame(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->draw());
	a->tag_name(QLatin1String(ods::ns::kFrame));
	a->func(DrawFrame);
	a->id(Id::DrawFrame);
}

void
DrawImage(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->draw());
	a->tag_name(QLatin1String(ods::ns::kImage));
	a->func(DrawImage);
	a->id(Id::DrawImage);
}

void
ManifestFileEntry(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->manifest());
	a->tag_name(QLatin1String(ods::ns::kFileEntry));
	a->func(ManifestFileEntry);
	a->id(Id::ManifestFileEntry);
}

void
ManifestManifest(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->manifest());
    a->tag_name(QLatin1String(ods::ns::kManifest));
	a->func(ManifestManifest);
	a->id(Id::ManifestManifest);
}

void
MetaCreationDate(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->meta());
	a->tag_name(QLatin1String(ods::ns::kCreationDate));
	a->func(MetaCreationDate);
	a->id(Id::MetaCreationDate);
}

void
MetaDocumentStatistic(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->meta());
	a->tag_name(QLatin1String(ods::ns::kDocumentStatistic));
	a->func(MetaDocumentStatistic);
	a->id(Id::MetaDocumentStatistic);
}

void
MetaEditingCycles(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->meta());
	a->tag_name(QLatin1String(ods::ns::kEditingCycles));
	a->func(MetaEditingCycles);
	a->id(Id::MetaEditingCycles);
}

void
MetaEditingDuration(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->meta());
	a->tag_name(QLatin1String(ods::ns::kEditingDuration));
	a->func(MetaEditingDuration);
	a->id(Id::MetaEditingDuration);
}

void
MetaGenerator(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->meta());
	a->tag_name(QLatin1String(ods::ns::kGenerator));
	a->func(MetaGenerator);
	a->id(Id::MetaGenerator);
}

void
MetaTemplate(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->meta());
	a->tag_name(QLatin1String(ods::ns::kTemplate));
	a->func(MetaTemplate);
	a->id(Id::MetaTemplate);
}

void
NumberBoolean(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->number());
	a->tag_name(QLatin1String(ods::ns::kBoolean));
	a->func(NumberBoolean);
	a->id(Id::NumberBoolean);
}

void
NumberBooleanStyle(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->number());
	a->tag_name(QLatin1String(ods::ns::kBooleanStyle));
	a->func(NumberBooleanStyle);
	a->bits() |= ods::inst::Bits::Style;
	a->id(Id::NumberBooleanStyle);
}

void
NumberCurrencyStyle(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->number());
	a->tag_name(QLatin1String(ods::ns::kCurrencyStyle));
	a->func(NumberCurrencyStyle);
	a->bits() |= ods::inst::Bits::Style;
	a->id(Id::NumberCurrencyStyle);
}

void
NumberCurrencySymbol(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->number());
	a->tag_name(QLatin1String(ods::ns::kCurrencySymbol));
	a->func(NumberCurrencySymbol);
	a->id(Id::NumberCurrencySymbol);
}

void
NumberDateStyle(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->number());
	a->tag_name(QLatin1String(ods::ns::kDateStyle));
	a->func(NumberDateStyle);
	a->bits() |= ods::inst::Bits::Style;
	a->id(Id::NumberDateStyle);
}

void
NumberDay(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->number());
	a->tag_name(QLatin1String(ods::ns::kDay));
	a->func(NumberDay);
	a->id(Id::NumberDay);
}

void
NumberHours(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->number());
	a->tag_name(QLatin1String(ods::ns::kHours));
	a->func(NumberHours);
	a->id(Id::NumberHours);
}

void
NumberMinutes(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->number());
	a->tag_name(QLatin1String(ods::ns::kMinutes));
	a->func(NumberMinutes);
	a->id(Id::NumberMinutes);
}

void
NumberMonth(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->number());
	a->tag_name(QLatin1String(ods::ns::kMonth));
	a->func(NumberMonth);
	a->id(Id::NumberMonth);
}

void
NumberNumber(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->number());
	a->tag_name(QLatin1String(ods::ns::kNumber));
	a->func(NumberNumber);
	a->id(Id::NumberNumber);
}

void
NumberNumberStyle(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->number());
	a->tag_name(QLatin1String(ods::ns::kNumberStyle));
	a->func(NumberNumberStyle);
	a->bits() |= ods::inst::Bits::Style;
	a->id(Id::NumberNumberStyle);
}

void
NumberPercentageStyle(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->number());
	a->tag_name(QLatin1String(ods::ns::kPercentageStyle));
	a->func(NumberPercentageStyle);
	a->bits() |= ods::inst::Bits::Style;
	a->id(Id::NumberPercentageStyle);
}

void
NumberSeconds(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->number());
	a->tag_name(QLatin1String(ods::ns::kSeconds));
	a->func(NumberSeconds);
	a->id(Id::NumberSeconds);
}

void
NumberText(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->number());
	a->tag_name(QLatin1String(ods::ns::kText));
	a->func(NumberText);
	a->id(Id::NumberText);
}

void
NumberTextContent(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->number());
	a->tag_name(QLatin1String(ods::ns::kTextContent));
	a->func(NumberTextContent);
	a->id(Id::NumberTextContent);
}

void
NumberTextStyle(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->number());
	a->tag_name(QLatin1String(ods::ns::kTextStyle));
	a->func(NumberTextStyle);
	a->bits() |= ods::inst::Bits::Style;
	a->id(Id::NumberTextStyle);
}

void
NumberTimeStyle(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->number());
	a->tag_name(QLatin1String(ods::ns::kTimeStyle));
	a->func(NumberTimeStyle);
	a->bits() |= ods::inst::Bits::Style;
	a->id(Id::NumberTimeStyle);
}

void
NumberYear(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->number());
	a->tag_name(QLatin1String(ods::ns::kYear));
	a->func(NumberYear);
	a->id(Id::NumberYear);
}

void
OfficeAutomaticStyles(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->office());
	a->tag_name(QLatin1String(ods::ns::kAutomaticStyles));
	a->func(OfficeAutomaticStyles);
	a->id(Id::OfficeAutomaticStyles);
}

void
OfficeBody(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->office());
	a->tag_name(ods::ns::kBody);
	a->func(OfficeBody);
	a->id(Id::OfficeBody);
}

void
OfficeDocumentContent(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->office());
	a->tag_name(QLatin1String(ods::ns::kDocumentContent));
	a->func(OfficeDocumentContent);
	a->id(Id::OfficeDocumentContent);
}

void
OfficeDocumentMeta(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->office());
	a->tag_name(ods::ns::kDocumentMeta);
	a->func(OfficeDocumentMeta);
	a->id(Id::OfficeDocumentMeta);
}

void
OfficeDocumentStyles(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->office());
	a->tag_name(ods::ns::kDocumentStyles);
	a->func(OfficeDocumentStyles);
	a->id(Id::OfficeDocumentStyles);
}

void
OfficeFontFaceDecls(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->office());
	a->tag_name(ods::ns::kFontFaceDecls);
	a->func(OfficeFontFaceDecls);
	a->id(Id::OfficeFontFaceDecls);
}

void
OfficeMasterStyles(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->office());
	a->tag_name(ods::ns::kMasterStyles);
	a->func(OfficeMasterStyles);
	a->id(Id::OfficeMasterStyles);
}

void
OfficeMeta(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->office());
	a->tag_name(ods::ns::kMeta);
	a->func(OfficeMeta);
	a->id(Id::OfficeMeta);
}

void
OfficeScripts(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->office());
	a->tag_name(ods::ns::kScripts);
	a->func(OfficeScripts);
	a->id(Id::OfficeScripts);
}

void
OfficeSpreadsheet(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->office());
	a->tag_name(ods::ns::kSpreadsheet);
	a->func(OfficeSpreadsheet);
	a->id(Id::OfficeSpreadsheet);
}

void
OfficeStyles(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->office());
	a->tag_name(ods::ns::kStyles);
	a->func(OfficeStyles);
	a->id(Id::OfficeStyles);
}

void
StyleBackgroundImage(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->style());
	a->tag_name(ods::ns::kBackgroundImage);
	a->func(StyleBackgroundImage);
	a->id(Id::StyleBackgroundImage);
}

void
StyleDefaultStyle(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->style());
	a->tag_name(ods::ns::kDefaultStyle);
	a->func(StyleDefaultStyle);
	a->id(Id::StyleDefaultStyle);
}

void
StyleFontFace(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->style());
	a->tag_name(ods::ns::kFontFace);
	a->func(StyleFontFace);
	a->id(Id::StyleFontFace);
}

void
StyleFooter(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->style());
	a->tag_name(ods::ns::kFooter);
	a->func(StyleFooter);
	a->id(Id::StyleFooter);
}

void
StyleFooterLeft(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->style());
	a->tag_name(ods::ns::kFooterLeft);
	a->func(StyleFooterLeft);
	a->id(Id::StyleFooterLeft);
}

void
StyleFooterStyle(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->style());
	a->tag_name(ods::ns::kFooterStyle);
	a->func(StyleFooterStyle);
	a->id(Id::StyleFooterStyle);
}

void
StyleHeader(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->style());
	a->tag_name(ods::ns::kHeader);
	a->func(StyleHeader);
	a->id(Id::StyleHeader);
}

void
StyleHeaderFooterProperties(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->style());
	a->tag_name(ods::ns::kHeaderFooterProperties);
	a->func(StyleHeaderFooterProperties);
	a->id(Id::StyleHeaderFooterProperties);
}

void
StyleHeaderLeft(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->style());
	a->tag_name(ods::ns::kHeaderLeft);
	a->func(StyleHeaderLeft);
	a->id(Id::StyleHeaderLeft);
}

void
StyleHeaderStyle(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->style());
	a->tag_name(ods::ns::kHeaderStyle);
	a->func(StyleHeaderStyle);
	a->id(Id::StyleHeaderStyle);
}

void
StyleMap(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->style());
	a->tag_name(ods::ns::kMap);
	a->func(StyleMap);
	a->id(Id::StyleMap);
}

void
StyleMasterPage(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->style());
	a->tag_name(ods::ns::kMasterPage);
	a->func(StyleMasterPage);
	a->id(Id::StyleMasterPage);
}

void
StylePageLayout(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->style());
	a->tag_name(ods::ns::kPageLayout);
	a->func(StylePageLayout);
	a->id(Id::StylePageLayout);
}

void
StylePageLayoutProperties(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->style());
	a->tag_name(ods::ns::kPageLayoutProperties);
	a->func(StylePageLayoutProperties);
	a->id(Id::StylePageLayoutProperties);
}

void
StyleParagraphProperties(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->style());
	a->tag_name(ods::ns::kParagraphProperties);
	a->func(StyleParagraphProperties);
	a->id(Id::StyleParagraphProperties);
}

void
StyleRegionLeft(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->style());
	a->tag_name(ods::ns::kRegionLeft);
	a->func(StyleRegionLeft);
	a->id(Id::StyleRegionLeft);
}

void
StyleRegionRight(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->style());
	a->tag_name(ods::ns::kRegionRight);
	a->func(StyleRegionRight);
	a->id(Id::StyleRegionRight);
}

void
StyleStyle(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->style());
	a->tag_name(ods::ns::kStyle);
	a->func(StyleStyle);
	a->bits() |= ods::inst::Bits::Style;
	a->id(Id::StyleStyle);
}

void
StyleTableCellProperties(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->style());
	a->tag_name(ods::ns::kTableCellProperties);
	a->func(StyleTableCellProperties);
	a->id(Id::StyleTableCellProperties);
}

void
StyleTableColumnProperties(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->style());
	a->tag_name(ods::ns::kTableColumnProperties);
	a->func(StyleTableColumnProperties);
	a->id(Id::StyleTableColumnProperties);
}

void
StyleTableProperties(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->style());
	a->tag_name(ods::ns::kTableProperties);
	a->func(StyleTableProperties);
	a->id(Id::StyleTableProperties);
}

void
StyleTableRowProperties(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->style());
	a->tag_name(ods::ns::kTableRowProperties);
	a->func(StyleTableRowProperties);
	a->id(Id::StyleTableRowProperties);
}

void
StyleTextProperties(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->style());
	a->tag_name(ods::ns::kTextProperties);
	a->func(StyleTextProperties);
	a->id(Id::StyleTextProperties);
}

void
SvgDesc(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->svg());
	a->tag_name(ods::ns::kDesc);
	a->func(SvgDesc);
	a->id(Id::SvgDesc);
}

void
SvgTitle(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->svg());
	a->tag_name(ods::ns::kTitle);
	a->func(SvgTitle);
	a->id(Id::SvgTitle);
}

void
TableCalculationSettings(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->table());
	a->tag_name(ods::ns::kCalculationSettings);
	a->func(TableCalculationSettings);
	a->id(Id::TableCalculationSettings);
}

void
TableNamedExpressions(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->table());
	a->tag_name(ods::ns::kNamedExpressions);
	a->func(TableNamedExpressions);
	a->id(Id::TableNamedExpressions);
}

void
TableNamedRange(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->table());
	a->tag_name(ods::ns::kNamedRange);
	a->func(TableNamedRange);
	a->id(Id::TableNamedRange);
}

void
TableTable(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->table());
	a->tag_name(ods::ns::kTable);
	a->func(TableTable);
	a->id(Id::TableTable);
}

void
TableTableCell(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->table());
	a->tag_name(ods::ns::kTableCell);
	a->func(TableTableCell);
	a->id(Id::TableTableCell);
}

void
TableTableColumn(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->table());
	a->tag_name(ods::ns::kTableColumn);
	a->func(TableTableColumn);
	a->id(Id::TableTableColumn);
}

void
TableTableRow(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->table());
	a->tag_name(ods::ns::kTableRow);
	a->func(TableTableRow);
	a->id(Id::TableTableRow);
}

void
TextDate(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->text());
	a->tag_name(ods::ns::kDate);
	a->func(TextDate);
	a->id(Id::TextDate);
}

void
TextP(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->text());
	a->tag_name(ods::ns::kP);
	a->func(TextP);
	a->id(Id::TextP);
}

void
TextPageCount(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->text());
	a->tag_name(ods::ns::kPageCount);
	a->func(TextPageCount);
	a->id(Id::TextPageCount);
}

void
TextPageNumber(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->text());
	a->tag_name(ods::ns::kPageNumber);
	a->func(TextPageNumber);
	a->id(Id::TextPageNumber);
}

void
TextS(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->text());
	a->tag_name(ods::ns::kS);
	a->func(TextS);
	a->id(Id::TextS);
}

void
TextSheetName(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->text());
	a->tag_name(ods::ns::kSheetName);
	a->func(TextSheetName);
	a->id(Id::TextSheetName);
}

void
TextTime(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->text());
	a->tag_name(ods::ns::kTime);
	a->func(TextTime);
	a->id(Id::TextTime);
}

void
TextTitle(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->text());
	a->tag_name(ods::ns::kTitle);
	a->func(TextTitle);
	a->id(Id::TextTitle);
}

}} // ods::id::
