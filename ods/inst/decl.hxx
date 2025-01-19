#pragma once

#include "../types.hxx"
#include <QHash>

namespace ods { // ods::
using UriId = u16;
namespace inst { // ods::inst::
using NsHash = QHash<UriId, QString>;
class IdAndCount {
public:
	i32 count = 0;
	i32 id = 0;
	
	static IdAndCount FromId(ci32 id) {
		return IdAndCount {.count = 1, .id = id};
	}
	
	inline bool operator < (const IdAndCount &rhs) const noexcept
	{
		return count < rhs.count;
	}
	
	inline bool operator == (const IdAndCount &rhs) const noexcept
	{
		return (id == rhs.id) && (count == rhs.count);
	}
};

using Keywords = QHash<QString, IdAndCount>;

class Abstract;
class DcDate;
class DcTitle;
class DrawFrame;
class DrawImage;
class ManifestFileEntry;
class ManifestManifest;
class MetaCreationDate;
class MetaDocumentStatistic;
class MetaEditingCycles;
class MetaEditingDuration;
class MetaGenerator;
class MetaTemplate;
class NumberBoolean;
class NumberBooleanStyle;
class NumberCurrencyStyle;
class NumberCurrencySymbol;
class NumberDateStyle;
class NumberDay;
class NumberHours;
class NumberMinutes;
class NumberMonth;
class NumberNumber;
class NumberNumberStyle;
class NumberPercentageStyle;
class NumberSeconds;
class NumberText;
class NumberTextContent;
class NumberTextStyle;
class NumberTimeStyle;
class NumberYear;
class OfficeAutomaticStyles;
class OfficeBody;
class OfficeDocumentContent;
class OfficeDocumentMeta;
class OfficeDocumentStyles;
class OfficeFontFaceDecls;
class OfficeMasterStyles;
class OfficeMeta;
class OfficeScripts;
class OfficeSpreadsheet;
class OfficeStyles;
class StyleBackgroundImage;
class StyleDefaultStyle;
class StyleFontFace;
class StyleFooter;
class StyleFooterLeft;
class StyleFooterStyle;
class StyleHeader;
class StyleHeaderFooterProperties;
class StyleHeaderLeft;
class StyleHeaderStyle;
class StyleMap;
class StyleMasterPage;
class StylePageLayout;
class StylePageLayoutProperties;
class StyleParagraphProperties;
class StyleRegionLeft;
class StyleRegionRight;
class StyleStyle;
class StyleTableCellProperties;
class StyleTableColumnProperties;
class StyleTableProperties;
class StyleTableRowProperties;
class StyleTextProperties;
class SvgDesc;
class SvgTitle;
class TableCalculationSettings;
class TableNamedExpressions;
class TableNamedRange;
class TableTableColumn;
class TextDate;
class TextP;
class TextPageCount;
class TextPageNumber;
class TextS;
class TextSheetName;
class TextTime;
class TextTitle;

}} // ods::
