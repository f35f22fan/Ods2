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
	a->tag_name(ods::ns::kManifest);
	a->func(DcDate);
}

void
DcTitle(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->dc());
	a->tag_name(ods::ns::kTitle);
	a->func(DcTitle);
}

void
DrawFrame(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->draw());
	a->tag_name(ods::ns::kFrame);
	a->func(DrawFrame);
}

void
DrawImage(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->draw());
	a->tag_name(ods::ns::kImage);
	a->func(DrawImage);
}

void
ManifestFileEntry(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->manifest());
	a->tag_name(ods::ns::kFileEntry);
	a->func(ManifestFileEntry);
}

void
ManifestManifest(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->manifest());
	a->tag_name(ods::ns::kManifest);
	a->func(ManifestManifest);
}

void
MetaCreationDate(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->meta());
	a->tag_name(ods::ns::kCreationDate);
	a->func(MetaCreationDate);
}

void
MetaDocumentStatistic(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->meta());
	a->tag_name(ods::ns::kDocumentStatistic);
	a->func(MetaDocumentStatistic);
}

void
MetaEditingCycles(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->meta());
	a->tag_name(ods::ns::kEditingCycles);
	a->func(MetaEditingCycles);
}

void
MetaEditingDuration(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->meta());
	a->tag_name(ods::ns::kEditingDuration);
	a->func(MetaEditingDuration);
}

void
MetaGenerator(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->meta());
	a->tag_name(ods::ns::kGenerator);
	a->func(MetaGenerator);
}

void
MetaTemplate(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->meta());
	a->tag_name(ods::ns::kTemplate);
	a->func(MetaTemplate);
}

void
NumberBoolean(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->number());
	a->tag_name(ods::ns::kBoolean);
	a->func(NumberBoolean);
}

void
NumberBooleanStyle(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->number());
	a->tag_name(ods::ns::kBooleanStyle);
	a->func(NumberBooleanStyle);
	a->bits() |= ods::inst::Bits::Style;
}

void
NumberCurrencyStyle(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->number());
	a->tag_name(ods::ns::kCurrencyStyle);
	a->func(NumberCurrencyStyle);
	a->bits() |= ods::inst::Bits::Style;
}

void
NumberCurrencySymbol(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->number());
	a->tag_name(ods::ns::kCurrencySymbol);
	a->func(NumberCurrencySymbol);
}

void
NumberDateStyle(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->number());
	a->tag_name(ods::ns::kDateStyle);
	a->func(NumberDateStyle);
	a->bits() |= ods::inst::Bits::Style;
}

void
NumberDay(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->number());
	a->tag_name(ods::ns::kDay);
	a->func(NumberDay);
}

void
NumberHours(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->number());
	a->tag_name(ods::ns::kHours);
	a->func(NumberHours);
}

void
NumberMinutes(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->number());
	a->tag_name(ods::ns::kMinutes);
	a->func(NumberMinutes);
}

void
NumberMonth(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->number());
	a->tag_name(ods::ns::kMonth);
	a->func(NumberMonth);
}

void
NumberNumber(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->number());
	a->tag_name(ods::ns::kNumber);
	a->func(NumberNumber);
}

void
NumberNumberStyle(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->number());
	a->tag_name(ods::ns::kNumberStyle);
	a->func(NumberNumberStyle);
	a->bits() |= ods::inst::Bits::Style;
}

void
NumberPercentageStyle(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->number());
	a->tag_name(ods::ns::kPercentageStyle);
	a->func(NumberPercentageStyle);
	a->bits() |= ods::inst::Bits::Style;
}

void
NumberSeconds(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->number());
	a->tag_name(ods::ns::kSeconds);
	a->func(NumberSeconds);
}

void
NumberText(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->number());
	a->tag_name(ods::ns::kText);
	a->func(NumberText);
}

void
NumberTextContent(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->number());
	a->tag_name(ods::ns::kTextContent);
	a->func(NumberTextContent);
}

void
NumberTextStyle(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->number());
	a->tag_name(ods::ns::kTextStyle);
	a->func(NumberTextStyle);
	a->bits() |= ods::inst::Bits::Style;
}

void
NumberTimeStyle(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->number());
	a->tag_name(ods::ns::kTimeStyle);
	a->func(NumberTimeStyle);
	a->bits() |= ods::inst::Bits::Style;
}

void
NumberYear(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->number());
	a->tag_name(ods::ns::kYear);
	a->func(NumberYear);
}

void
OfficeAutomaticStyles(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->office());
	a->tag_name(ods::ns::kAutomaticStyles);
	a->func(OfficeAutomaticStyles);
}

void
OfficeBody(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->office());
	a->tag_name(ods::ns::kBody);
	a->func(OfficeBody);
}

void
OfficeDocumentContent(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->office());
	a->tag_name(ods::ns::kDocumentContent);
	a->func(OfficeDocumentContent);
}

void
OfficeDocumentMeta(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->office());
	a->tag_name(ods::ns::kDocumentMeta);
	a->func(OfficeDocumentMeta);
}

void
OfficeDocumentStyles(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->office());
	a->tag_name(ods::ns::kDocumentStyles);
	a->func(OfficeDocumentStyles);
}

void
OfficeFontFaceDecls(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->office());
	a->tag_name(ods::ns::kFontFaceDecls);
	a->func(OfficeFontFaceDecls);
}

void
OfficeMasterStyles(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->office());
	a->tag_name(ods::ns::kMasterStyles);
	a->func(OfficeMasterStyles);
}

void
OfficeMeta(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->office());
	a->tag_name(ods::ns::kMeta);
	a->func(OfficeMeta);
}

void
OfficeScripts(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->office());
	a->tag_name(ods::ns::kScripts);
	a->func(OfficeScripts);
}

void
OfficeSpreadsheet(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->office());
	a->tag_name(ods::ns::kSpreadsheet);
	a->func(OfficeSpreadsheet);
}

void
OfficeStyles(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->office());
	a->tag_name(ods::ns::kStyles);
	a->func(OfficeStyles);
}

void
StyleBackgroundImage(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->style());
	a->tag_name(ods::ns::kBackgroundImage);
	a->func(StyleBackgroundImage);
}

void
StyleDefaultStyle(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->style());
	a->tag_name(ods::ns::kDefaultStyle);
	a->func(StyleDefaultStyle);
}

void
StyleFontFace(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->style());
	a->tag_name(ods::ns::kFontFace);
	a->func(StyleFontFace);
}

void
StyleFooter(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->style());
	a->tag_name(ods::ns::kFooter);
	a->func(StyleFooter);
}

void
StyleFooterLeft(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->style());
	a->tag_name(ods::ns::kFooterLeft);
	a->func(StyleFooterLeft);
}

void
StyleFooterStyle(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->style());
	a->tag_name(ods::ns::kFooterStyle);
	a->func(StyleFooterStyle);
}

void
StyleHeader(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->style());
	a->tag_name(ods::ns::kHeader);
	a->func(StyleHeader);
}

void
StyleHeaderFooterProperties(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->style());
	a->tag_name(ods::ns::kHeaderFooterProperties);
	a->func(StyleHeaderFooterProperties);
}

void
StyleHeaderLeft(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->style());
	a->tag_name(ods::ns::kHeaderLeft);
	a->func(StyleHeaderLeft);
}

void
StyleHeaderStyle(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->style());
	a->tag_name(ods::ns::kHeaderStyle);
	a->func(StyleHeaderStyle);
}

void
StyleMap(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->style());
	a->tag_name(ods::ns::kMap);
	a->func(StyleMap);
}

void
StyleMasterPage(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->style());
	a->tag_name(ods::ns::kMasterPage);
	a->func(StyleMasterPage);
}

void
StylePageLayout(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->style());
	a->tag_name(ods::ns::kPageLayout);
	a->func(StylePageLayout);
}

void
StylePageLayoutProperties(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->style());
	a->tag_name(ods::ns::kPageLayoutProperties);
	a->func(StylePageLayoutProperties);
}

void
StyleParagraphProperties(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->style());
	a->tag_name(ods::ns::kParagraphProperties);
	a->func(StyleParagraphProperties);
}

void
StyleRegionLeft(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->style());
	a->tag_name(ods::ns::kRegionLeft);
	a->func(StyleRegionLeft);
}

void
StyleRegionRight(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->style());
	a->tag_name(ods::ns::kRegionRight);
	a->func(StyleRegionRight);
}

void
StyleStyle(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->style());
	a->tag_name(ods::ns::kStyle);
	a->func(StyleStyle);
	a->bits() |= ods::inst::Bits::Style;
}

void
StyleTableCellProperties(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->style());
	a->tag_name(ods::ns::kTableCellProperties);
	a->func(StyleTableCellProperties);
}

void
StyleTableColumnProperties(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->style());
	a->tag_name(ods::ns::kTableColumnProperties);
	a->func(StyleTableColumnProperties);
}

void
StyleTableProperties(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->style());
	a->tag_name(ods::ns::kTableProperties);
	a->func(StyleTableProperties);
}

void
StyleTableRowProperties(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->style());
	a->tag_name(ods::ns::kTableRowProperties);
	a->func(StyleTableRowProperties);
}

void
StyleTextProperties(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->style());
	a->tag_name(ods::ns::kTextProperties);
	a->func(StyleTextProperties);
}

void
SvgDesc(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->svg());
	a->tag_name(ods::ns::kDesc);
	a->func(SvgDesc);
}

void
SvgTitle(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->svg());
	a->tag_name(ods::ns::kTitle);
	a->func(SvgTitle);
}

void
TableCalculationSettings(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->table());
	a->tag_name(ods::ns::kCalculationSettings);
	a->func(TableCalculationSettings);
}

void
TableNamedExpressions(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->table());
	a->tag_name(ods::ns::kNamedExpressions);
	a->func(TableNamedExpressions);
}

void
TableTable(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->table());
	a->tag_name(ods::ns::kTable);
	a->func(TableTable);
}

void
TableTableCell(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->table());
	a->tag_name(ods::ns::kTableCell);
	a->func(TableTableCell);
}

void
TableTableColumn(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->table());
	a->tag_name(ods::ns::kTableColumn);
	a->func(TableTableColumn);
}

void
TableTableRow(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->table());
	a->tag_name(ods::ns::kTableRow);
	a->func(TableTableRow);
}

void
TextDate(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->text());
	a->tag_name(ods::ns::kDate);
	a->func(TextDate);
}

void
TextP(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->text());
	a->tag_name(ods::ns::kP);
	a->func(TextP);
}

void
TextPageCount(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->text());
	a->tag_name(ods::ns::kPageCount);
	a->func(TextPageCount);
}

void
TextPageNumber(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->text());
	a->tag_name(ods::ns::kPageNumber);
	a->func(TextPageNumber);
}

void
TextS(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->text());
	a->tag_name(ods::ns::kS);
	a->func(TextS);
}

void
TextSheetName(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->text());
	a->tag_name(ods::ns::kSheetName);
	a->func(TextSheetName);
}

void
TextTime(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->text());
	a->tag_name(ods::ns::kTime);
	a->func(TextTime);
}

void
TextTitle(Ns *ns, inst::Abstract *a)
{
	a->ns(ns);
	a->prefix(ns->text());
	a->tag_name(ods::ns::kTitle);
	a->func(TextTitle);
}

}} // ods::id::