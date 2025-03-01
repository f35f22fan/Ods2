/*
 * Copyright © 2014 f35f22fan@gmail.com
 *
 * Read the LICENSE file that comes with this project for license details.
*/

#pragma once

#include <QString>
#include <QtGlobal>

namespace ods::ns {

static const auto False = QStringLiteral("false");
static const auto True = QStringLiteral("true");

static const auto kActuate = QStringLiteral("actuate");
static const auto kApplyStyleName = QStringLiteral("apply-style-name");
static const auto kAuto			 = QStringLiteral("auto");
static const auto kAutomaticFindLabels = QStringLiteral("automatic-find-labels");
static const auto kAutomaticOrder	 = QStringLiteral("automatic-order");
static const auto kAutomaticStyles	 = QStringLiteral("automatic-styles");
static const auto kBackgroundColor	 = QStringLiteral("background-color");
static const auto kBackgroundImage	 = QStringLiteral("background-image");
static const auto kBaseCellAddress	 = QStringLiteral("base-cell-address");
static const auto kBlue			 = QStringLiteral("blue");
static const auto kBody			 = QStringLiteral("body");
static const auto kBold			 = QStringLiteral("bold");
static const auto kBoolean			 = QStringLiteral("boolean");
static const auto kBooleanStyle	 = QStringLiteral("boolean-style");
static const auto kBooleanValue	 = QStringLiteral("boolean-value");
static const auto kBorder			 = QStringLiteral("border");
static const auto kBorderBottom	 = QStringLiteral("border-bottom");
static const auto kBorderLeft		 = QStringLiteral("border-left");
static const auto kBorderLineWidth	 = QStringLiteral("border-line-width");
static const auto kBorderRight		 = QStringLiteral("border-right");
static const auto kBorderTop		 = QStringLiteral("border-top");
static const auto kBottom			 = QStringLiteral("bottom");
static const auto kBreakBefore		 = QStringLiteral("break-before");
static const auto kCalculationSettings = QStringLiteral("calculation-settings");
static const auto kCaseSensitive	 = QStringLiteral("case-sensitive");
static const auto kCellCount		 = QStringLiteral("cell-count");
static const auto kCellRangeAddress = QStringLiteral("cell-range-address");
static const auto kCenter			 = QStringLiteral("center");
static const auto kChart			 = QStringLiteral("chart");
static const auto kClip			 = QStringLiteral("clip");
static const auto kColor			 = QStringLiteral("color");
static const auto kColorMode		 = QStringLiteral("color-mode");
static const auto kColumnWidth		 = QStringLiteral("column-width");
static const auto kCondition		 = QStringLiteral("condition");
static const auto kConfigItem		 = QStringLiteral("config-item");
static const auto kConfigItemMapEntry = QStringLiteral("config-item-map-entry");
static const auto kConfigItemMapIndexed = QStringLiteral("config-item-map-indexed");
static const auto kConfigItemMapNamed = QStringLiteral("config-item-map-named");
static const auto kConfigItemSet	 = QStringLiteral("config-item-set");
static const auto kContrast		 = QStringLiteral("contrast");
static const auto kCountry			 = QStringLiteral("country");
static const auto kCountryAsian	 = QStringLiteral("country-asian");
static const auto kCountryComplex	 = QStringLiteral("country-complex");
static const auto kCoveredTableCell = QStringLiteral("covered-table-cell");
static const auto kCreationDate	 = QStringLiteral("creation-date");
static const auto kCreator			 = QStringLiteral("creator");
static const auto kCurrency		 = QStringLiteral("currency");
static const auto kCurrencyStyle	 = QStringLiteral("currency-style");
static const auto kCurrencySymbol	 = QStringLiteral("currency-symbol");
static const auto kDataStyleName	 = QStringLiteral("data-style-name");
static const auto kDate			 = QStringLiteral("date");
static const auto kDateStyle		 = QStringLiteral("date-style");
static const auto kDateTime		 = QStringLiteral("date-time");
static const auto kDateValue		 = QStringLiteral("date-value");
static const auto kDay				 = QStringLiteral("day");
static const auto kDecimalPlaces	 = QStringLiteral("decimal-places");
static const auto kDefaultCellStyleName = QStringLiteral("default-cell-style-name");
static const auto kDefaultStyle	 = QStringLiteral("default-style");
static const auto kDesc			 = QStringLiteral("desc");
static const auto kDiagonalBlTr	 = QStringLiteral("diagonal-bl-tr");
static const auto kDiagonalTlBr	 = QStringLiteral("diagonal-tl-br");
static const auto kDisplay			 = QStringLiteral("display");
static const auto kDisplayName		 = QStringLiteral("display-name");
static const auto kDocumentContent	 = QStringLiteral("document-content");
static const auto kDocumentMeta	 = QStringLiteral("document-meta");
static const auto kDocumentSettings = QStringLiteral("document-settings");
static const auto kDocumentStatistic = QStringLiteral("document-statistic");
static const auto kDocumentStyles	 = QStringLiteral("document-styles");
static const auto kDouble			 = QStringLiteral("float");
static const auto kDrawingPage		 = QStringLiteral("drawing-page");
static const auto kEditingCycles	 = QStringLiteral("editing-cycles");
static const auto kEditingDuration	 = QStringLiteral("editing-duration");
static const auto kEnd				 = QStringLiteral("end");
static const auto kFalse			 = QStringLiteral("false");
static const auto kFamily			 = QStringLiteral("family");
static const auto kFileEntry		 = QStringLiteral("file-entry");
static const auto kFill			 = QStringLiteral("fill");
static const auto kFontFace		 = QStringLiteral("font-face");
static const auto kFontFaceDecls	 = QStringLiteral("font-face-decls");
static const auto kFontFamily		 = QStringLiteral("font-family");
static const auto kFontFamilyAsian	 = QStringLiteral("font-family-asian");
static const auto kFontFamilyComplex = QStringLiteral("font-family-complex");
static const auto kFontFamilyGeneric = QStringLiteral("font-family-generic");
static const auto kFontFamilyGenericAsian = QStringLiteral("font-family-generic-asian");
static const auto kFontFamilyGenericComplex = QStringLiteral("font-family-generic-complex");
static const auto kFontName		 = QStringLiteral("font-name");
static const auto kFontNameAsian	 = QStringLiteral("font-name-asian");
static const auto kFontNameComplex	 = QStringLiteral("font-name-complex");
static const auto kFontPitch		 = QStringLiteral("font-pitch");
static const auto kFontPitchAsian	 = QStringLiteral("font-pitch-asian");
static const auto kFontPitchComplex = QStringLiteral("font-pitch-complex");
static const auto kFontSize		 = QStringLiteral("font-size");
static const auto kFontSizeAsian	 = QStringLiteral("font-size-asian");
static const auto kFontSizeComplex	 = QStringLiteral("font-size-complex");
static const auto kFontStyle		 = QStringLiteral("font-style"); // e.g. "italic"
static const auto kFontStyleAsian	 = QStringLiteral("font-style-asian");
static const auto kFontStyleComplex = QStringLiteral("font-style-complex");
static const auto kFontWeight		 = QStringLiteral("font-weight"); // e.g. "bold"
static const auto kFontWeightAsian	 = QStringLiteral("font-weight-asian");
static const auto kFontWeightComplex = QStringLiteral("font-weight-complex");
static const auto kFooter			 = QStringLiteral("footer");
static const auto kFooterLeft		 = QStringLiteral("footer-left");
static const auto kFooterStyle		 = QStringLiteral("footer-style");
static const auto kFormula			 = QStringLiteral("formula");
static const auto kFrame			 = QStringLiteral("frame");
static const auto kFullPath		 = QStringLiteral("full-path");
static const auto kGamma			 = QStringLiteral("gamma");
static const auto kGenerator		 = QStringLiteral("generator");
static const auto kGraphicProperties = QStringLiteral("graphic-properties");
static const auto kGreen			 = QStringLiteral("green");
static const auto kGrouping		 = QStringLiteral("grouping");
static const auto kHeader			 = QStringLiteral("header");
static const auto kHeaderFooterProperties = QStringLiteral("header-footer-properties");
static const auto kHeaderLeft		 = QStringLiteral("header-left");
static const auto kHeaderStyle		 = QStringLiteral("header-style");
static const auto kHeight			 = QStringLiteral("height");
static const auto kHours			 = QStringLiteral("hours");
static const auto kHref			 = QStringLiteral("href");
static const auto kHyphenate		 = QStringLiteral("hyphenate");
static const auto kId				 = QStringLiteral("id");
static const auto kImage			 = QStringLiteral("image");
static const auto kImageOpacity	 = QStringLiteral("image-opacity");
static const auto kInitialCreator	 = QStringLiteral("initial-creator");
static const auto kInteger			 = QStringLiteral("float"); // because table-cell office:value-type="float"
static const auto kItalic			 = QStringLiteral("italic");
static const auto kLanguage		 = QStringLiteral("language");
static const auto kLanguageAsian	 = QStringLiteral("language-asian");
static const auto kLanguageComplex	 = QStringLiteral("language-complex");
static const auto kLuminance		 = QStringLiteral("luminance");
static const auto kManifest		 = QStringLiteral("manifest");
static const auto kMap				 = QStringLiteral("map");
static const auto kMarginLeft		 = QStringLiteral("margin-left");
static const auto kMarginRight		 = QStringLiteral("margin-right");
static const auto kMarginBottom	 = QStringLiteral("margin-bottom");
static const auto kMarginTop		 = QStringLiteral("margin-top");
static const auto kMasterPage		 = QStringLiteral("master-page");
static const auto kMasterPageName	 = QStringLiteral("master-page-name");
static const auto kMasterStyles	 = QStringLiteral("master-styles");
static const auto kMediaType		 = QStringLiteral("media-type");
static const auto kMeta			 = QStringLiteral("meta");
static const auto kMimeType		= QStringLiteral("application/vnd.oasis.opendocument.spreadsheet");
static const auto kMiddle			 = QStringLiteral("middle");
static const auto kMinDecimalPlaces = QStringLiteral("min-decimal-places");
static const auto kMinHeight		 = QStringLiteral("min-height");
static const auto kMinIntegerDigits = QStringLiteral("min-integer-digits");
static const auto kMinutes			 = QStringLiteral("minutes");
static const auto kMirror			 = QStringLiteral("mirror");
static const auto kMonth			 = QStringLiteral("month");
static const auto kN100			 = QStringLiteral("100");
static const auto kN200			 = QStringLiteral("200");
static const auto kN300			 = QStringLiteral("300");
static const auto kN400			 = QStringLiteral("400");
static const auto kN500			 = QStringLiteral("500");
static const auto kN600			 = QStringLiteral("600");
static const auto kN700			 = QStringLiteral("700");
static const auto kN800			 = QStringLiteral("800");
static const auto kN900			 = QStringLiteral("900");
static const auto kName				= QStringLiteral("name");
static const auto kNamedExpressions = QStringLiteral("named-expressions");
static const auto kNamedRange		 = QStringLiteral("named-range");
static const auto kNormal			 = QStringLiteral("normal");
static const auto kNumber			 = QStringLiteral("number");
static const auto kNumberStyle		 = QStringLiteral("number-style");
static const auto kNumberColumnsRepeated = QStringLiteral("number-columns-repeated");
static const auto kNumberColumnsSpanned = QStringLiteral("number-columns-spanned");
static const auto kNumberRowsRepeated = QStringLiteral("number-rows-repeated");
static const auto kNumberRowsSpanned = QStringLiteral("number-rows-spanned");
static const auto kObjectCount		 = QStringLiteral("object-count");
static const auto kOblique			 = QStringLiteral("oblique");
static const auto kOpacity			 = QStringLiteral("opacity");
static const auto kP				 = QStringLiteral("p");
static const auto kPadding			 = QStringLiteral("padding");
static const auto kPageCount		 = QStringLiteral("page-count");
static const auto kPageLayout		 = QStringLiteral("page-layout");
static const auto kPageLayoutName	 = QStringLiteral("page-layout-name");
static const auto kPageLayoutProperties = QStringLiteral("page-layout-properties");
static const auto kPageNumber		 = QStringLiteral("page-number");
static const auto kPageUsage		 = QStringLiteral("page-usage");
static const auto kParagraphProperties = QStringLiteral("paragraph-properties");
static const auto kParentStyleName	 = QStringLiteral("parent-style-name");
static const auto kPercentage		 = QStringLiteral("percentage");
static const auto kPercentageStyle	 = QStringLiteral("percentage-style");
static const auto kRed				 = QStringLiteral("red");
static const auto kRegionLeft		 = QStringLiteral("region-left");
static const auto kRegionRight		 = QStringLiteral("region-right");
static const auto kRelHeight		 = QStringLiteral("rel-height");
static const auto kRelWidth		 = QStringLiteral("rel-width");
static const auto kRepeatContent	 = QStringLiteral("repeat-content");
static const auto kRfcLanguageTag	 = QStringLiteral("rfc-language-tag");
static const auto kRotationAngle	 = QStringLiteral("rotation-angle");
static const auto kRowHeight		 = QStringLiteral("row-height");
static const auto kS				 = QStringLiteral("s");
static const auto kScript			 = QStringLiteral("script");
static const auto kScripts			 = QStringLiteral("scripts");
static const auto kSearchWholeCell	 = QStringLiteral("search-criteria-must-apply-to-whole-cell");
static const auto kSeconds			 = QStringLiteral("seconds");
static const auto kSettings		 = QStringLiteral("settings");
static const auto kSheetName		 = QStringLiteral("sheet-name"); //<text:sheet-name>
static const auto kShow			 = QStringLiteral("show");
static const auto kSpan			 = QStringLiteral("span");
static const auto kSpreadsheet		 = QStringLiteral("spreadsheet");
static const auto kStart			 = QStringLiteral("start");
static const auto kString			 = QStringLiteral("string");
static const auto kStroke			 = QStringLiteral("stroke");
static const auto kStyleName		 = QStringLiteral("style-name");
static const auto kStyle			 = QStringLiteral("style");
static const auto kStyles			 = QStringLiteral("styles");
static const auto kTable			 = QStringLiteral("table");
static const auto kTableCell		 = QStringLiteral("table-cell");
static const auto kTableCellProperties = QStringLiteral("table-cell-properties");
static const auto kTableColumn		 = QStringLiteral("table-column");
static const auto kTableColumnProperties = QStringLiteral("table-column-properties");
static const auto kTableName		 = QStringLiteral("table-name");
static const auto kTableRow		 = QStringLiteral("table-row");
static const auto kTableRowProperties = QStringLiteral("table-row-properties");
static const auto kTableCount		 = QStringLiteral("table-count");
static const auto kTableProperties	 = QStringLiteral("table-properties");
static const auto kTabStopDistance	 = QStringLiteral("tab-stop-distance");
static const auto kTemplate		 = QStringLiteral("template");
static const auto kText			 = QStringLiteral("text");
static const auto kTextAlign		 = QStringLiteral("text-align");
static const auto kTextAlignSource	 = QStringLiteral("text-align-source");
static const auto kTextAreaHorizAlign = QStringLiteral("textarea-horizontal-align");
static const auto kTextAreaVertAlign = QStringLiteral("textarea-vertical-align");
static const auto kTextContent		 = QStringLiteral("text-content");
static const auto kTextProperties	 = QStringLiteral("text-properties");
static const auto kTextStyle		 = QStringLiteral("text-style");
static const auto kTextUnderlineColor = QStringLiteral("text-underline-color");
static const auto kTextUnderlineStyle = QStringLiteral("text-underline-style");
static const auto kTextUnderlineWidth = QStringLiteral("text-underline-width");
static const auto kTextXml			 = QStringLiteral("text/xml");
static const auto kTime			 = QStringLiteral("time");
static const auto kTimeStyle		 = QStringLiteral("time-style");
static const auto kTimeValue		 = QStringLiteral("time-value");
static const auto kTitle			 = QStringLiteral("title");
static const auto kTop				 = QStringLiteral("top");
static const auto kTransliterationCountry = QStringLiteral("transliteration-country");
static const auto kTransliterationFormat = QStringLiteral("transliteration-format");
static const auto kTransliterationLanguage = QStringLiteral("transliteration-language");
static const auto kTransliterationStyle = QStringLiteral("transliteration-style");
static const auto kTransparent		 = QStringLiteral("transparent");
static const auto kTrue			 = QStringLiteral("true");
static const auto kTruncateOnOverflow = QStringLiteral("truncate-on-overflow");
static const auto kType			 = QStringLiteral("type");
static const auto kUseOptimalRowHeight = QStringLiteral("use-optimal-row-height");
static const auto kUseRegularExpressions = QStringLiteral("use-regular-expressions");
static const auto kUseWildcards	 = QStringLiteral("use-wildcards");
static const auto kValue			 = QStringLiteral("value");
static const auto kValueType		 = QStringLiteral("value-type");
static const auto kVariable		 = QStringLiteral("variable");
static const auto kVersion			 = QStringLiteral("version");
static const auto kVerticalAlign	 = QStringLiteral("vertical-align");
static const auto kVolatile		 = QStringLiteral("volatile");
static const auto kWidth			 = QStringLiteral("width");
static const auto kWrapOption		 = QStringLiteral("wrap-option");
static const auto kWritingMode		 = QStringLiteral("writing-mode");
static const auto kWritingModeLrtb	 = QStringLiteral("lr-tb");
static const auto kX				 = QStringLiteral("x");
static const auto kY				 = QStringLiteral("y");
static const auto kYear			 = QStringLiteral("year");
static const auto kZIndex			 = QStringLiteral("z-index");

} // ods::ns::


