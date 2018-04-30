#pragma once

#include "global.hxx"

namespace ods { // ods::
class Ns;

namespace inst { // ods::inst::
class Abstract;
} // ods::inst::

enum class Id : int16_t
{
	None = 0,
	DcDate,
	DcTitle,
	DrawFrame,
	DrawImage,
	ManifestFileEntry,
	ManifestManifest,
	MetaCreationDate,
	MetaDocumentStatistic,
	MetaEditingCycles,
	MetaEditingDuration,
	MetaGenerator,
	MetaTemplate,
	NumberBoolean,
	NumberBooleanStyle,
	NumberCurrencyStyle,
	NumberCurrencySymbol,
	NumberDateStyle,
	NumberDay,
	NumberHours,
	NumberMinutes,
	NumberSeconds,
	NumberMonth,
	NumberNumber,
	NumberNumberStyle,
	NumberPercentageStyle,
	NumberText,
	NumberTextContent,
	NumberTextStyle,
	NumberTimeStyle,
	NumberYear,
	OfficeAutomaticStyles,
	OfficeBody,
	OfficeDocumentContent,
	OfficeDocumentMeta,
	OfficeDocumentStyles,
	OfficeFontFaceDecls,
	OfficeMasterStyles,
	OfficeMeta,
	OfficeScripts,
	OfficeSpreadsheet,
	OfficeStyles,
	StyleBackgroundImage,
	StyleDefaultStyle,
	StyleFontFace,
	StyleFooter,
	StyleFooterLeft,
	StyleFooterStyle,
	StyleHeader,
	StyleHeaderFooterProperties,
	StyleHeaderLeft,
	StyleHeaderStyle,
	StyleMap,
	StyleMasterPage,
	StylePageLayout,
	StylePageLayoutProperties,
	StyleParagraphProperties,
	StyleRegionLeft,
	StyleRegionRight,
	StyleStyle,
	StyleTableCellProperties,
	StyleTableColumnProperties,
	StyleTableProperties,
	StyleTableRowProperties,
	StyleTextProperties,
	SvgDesc,
	SvgTitle,
	TableCalculationSettings,
	TableNamedExpressions,
	TableTable,
	TableTableCell,
	TableTableColumn,
	TableTableRow,
	TextDate,
	TextP,
	TextPageCount,
	TextPageNumber,
	TextS,
	TextSheetName,
	TextTime,
	TextTitle,
};

namespace id { // ods::id::

void ODS_API DcDate(Ns *ns, inst::Abstract *a);
void ODS_API DcTitle(Ns *ns, inst::Abstract *a);
void ODS_API DrawFrame(Ns *ns, inst::Abstract *a);
void ODS_API DrawImage(Ns *ns, inst::Abstract *a);
void ODS_API ManifestFileEntry(Ns *ns, inst::Abstract *a);
void ODS_API ManifestManifest(Ns *ns, inst::Abstract *a);
void ODS_API MetaCreationDate(Ns *ns, inst::Abstract *a);
void ODS_API MetaDocumentStatistic(Ns *ns, inst::Abstract *a);
void ODS_API MetaEditingCycles(Ns *ns, inst::Abstract *a);
void ODS_API MetaEditingDuration(Ns *ns, inst::Abstract *a);
void ODS_API MetaGenerator(Ns *ns, inst::Abstract *a);
void ODS_API MetaTemplate(Ns *ns, inst::Abstract *a);
void ODS_API NumberBoolean(Ns *ns, inst::Abstract *a);
void ODS_API NumberBooleanStyle(Ns *ns, inst::Abstract *a);
void ODS_API NumberCurrencyStyle(Ns *ns, inst::Abstract *a);
void ODS_API NumberCurrencySymbol(Ns *ns, inst::Abstract *a);
void ODS_API NumberDateStyle(Ns *ns, inst::Abstract *a);
void ODS_API NumberDay(Ns *ns, inst::Abstract *a);
void ODS_API NumberHours(Ns *ns, inst::Abstract *a);
void ODS_API NumberMinutes(Ns *ns, inst::Abstract *a);
void ODS_API NumberSeconds(Ns *ns, inst::Abstract *a);
void ODS_API NumberMonth(Ns *ns, inst::Abstract *a);
void ODS_API NumberNumber(Ns *ns, inst::Abstract *a);
void ODS_API NumberNumberStyle(Ns *ns, inst::Abstract *a);
void ODS_API NumberPercentageStyle(Ns *ns, inst::Abstract *a);
void ODS_API NumberText(Ns *ns, inst::Abstract *a);
void ODS_API NumberTextContent(Ns *ns, inst::Abstract *a);
void ODS_API NumberTextStyle(Ns *ns, inst::Abstract *a);
void ODS_API NumberTimeStyle(Ns *ns, inst::Abstract *a);
void ODS_API NumberYear(Ns *ns, inst::Abstract *a);
void ODS_API OfficeAutomaticStyles(Ns *ns, inst::Abstract *a);
void ODS_API OfficeBody(Ns *ns, inst::Abstract *a);
void ODS_API OfficeDocumentContent(Ns *ns, inst::Abstract *a);
void ODS_API OfficeDocumentMeta(Ns *ns, inst::Abstract *a);
void ODS_API OfficeDocumentStyles(Ns *ns, inst::Abstract *a);
void ODS_API OfficeFontFaceDecls(Ns *ns, inst::Abstract *a);
void ODS_API OfficeMasterStyles(Ns *ns, inst::Abstract *a);
void ODS_API OfficeMeta(Ns *ns, inst::Abstract *a);
void ODS_API OfficeScripts(Ns *ns, inst::Abstract *a);
void ODS_API OfficeSpreadsheet(Ns *ns, inst::Abstract *a);
void ODS_API OfficeStyles(Ns *ns, inst::Abstract *a);
void ODS_API StyleBackgroundImage(Ns *ns, inst::Abstract *a);
void ODS_API StyleDefaultStyle(Ns *ns, inst::Abstract *a);
void ODS_API StyleFontFace(Ns *ns, inst::Abstract *a);
void ODS_API StyleFooter(Ns *ns, inst::Abstract *a);
void ODS_API StyleFooterLeft(Ns *ns, inst::Abstract *a);
void ODS_API StyleFooterStyle(Ns *ns, inst::Abstract *a);
void ODS_API StyleHeader(Ns *ns, inst::Abstract *a);
void ODS_API StyleHeaderFooterProperties(Ns *ns, inst::Abstract *a);
void ODS_API StyleHeaderLeft(Ns *ns, inst::Abstract *a);
void ODS_API StyleHeaderStyle(Ns *ns, inst::Abstract *a);
void ODS_API StyleMap(Ns *ns, inst::Abstract *a);
void ODS_API StyleMasterPage(Ns *ns, inst::Abstract *a);
void ODS_API StylePageLayout(Ns *ns, inst::Abstract *a);
void ODS_API StylePageLayoutProperties(Ns *ns, inst::Abstract *a);
void ODS_API StyleParagraphProperties(Ns *ns, inst::Abstract *a);
void ODS_API StyleRegionLeft(Ns *ns, inst::Abstract *a);
void ODS_API StyleRegionRight(Ns *ns, inst::Abstract *a);
void ODS_API StyleStyle(Ns *ns, inst::Abstract *a);
void ODS_API StyleTableCellProperties(Ns *ns, inst::Abstract *a);
void ODS_API StyleTableColumnProperties(Ns *ns, inst::Abstract *a);
void ODS_API StyleTableProperties(Ns *ns, inst::Abstract *a);
void ODS_API StyleTableRowProperties(Ns *ns, inst::Abstract *a);
void ODS_API StyleTextProperties(Ns *ns, inst::Abstract *a);
void ODS_API SvgDesc(Ns *ns, inst::Abstract *a);
void ODS_API SvgTitle(Ns *ns, inst::Abstract *a);
void ODS_API TableCalculationSettings(Ns *ns, inst::Abstract *a);
void ODS_API TableNamedExpressions(Ns *ns, inst::Abstract *a);
void ODS_API TableTable(Ns *ns, inst::Abstract *a);
void ODS_API TableTableCell(Ns *ns, inst::Abstract *a);
void ODS_API TableTableColumn(Ns *ns, inst::Abstract *a);
void ODS_API TableTableRow(Ns *ns, inst::Abstract *a);
void ODS_API TextDate(Ns *ns, inst::Abstract *a);
void ODS_API TextP(Ns *ns, inst::Abstract *a);
void ODS_API TextPageCount(Ns *ns, inst::Abstract *a);
void ODS_API TextPageNumber(Ns *ns, inst::Abstract *a);
void ODS_API TextS(Ns *ns, inst::Abstract *a);
void ODS_API TextSheetName(Ns *ns, inst::Abstract *a);
void ODS_API TextTime(Ns *ns, inst::Abstract *a);
void ODS_API TextTitle(Ns *ns, inst::Abstract *a);
}} // ods::id::
