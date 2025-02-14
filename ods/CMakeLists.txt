# author f35f22fan at gmail dot com
# .hh, .cc => functions within namespaces
# .cpp, hpp => classes
# .hxx => (const) declarations

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	message(STATUS "ADDED -funsigned-char Option!")
	add_definitions(-funsigned-char)
endif()

message(STATUS "C++ Compiler: " ${CMAKE_CXX_COMPILER})

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

include_directories("${Ods2_SOURCE_DIR}/libzip/lib")
link_directories("${Ods2_BINARY_DIR}/lib" "${Ods2_BINARY_DIR}/lib/Debug" "${Ods2_BINARY_DIR}/lib/Release")

set(hh_files
	AutoDelete.hh
	cmp_num.hh
	currency.hh
	eval.hh
	function.hh
	id.hh
	io.hh
	ndff/ndff.hh
	ods.hh
	op.hh
	style.hh
	zip.hh
)

foreach(f IN LISTS hh_files)
	set_property(SOURCE ${f} PROPERTY SKIP_AUTOMOC ON)
endforeach()


set(lib_files
	${hh_files}
	Attr.cpp Attr.hpp
	AutoDelete.hh
	Book.cpp Book.hpp
	ByteArray.cpp ByteArray.hpp
	Cell.cpp Cell.hpp cell.hxx
	CellRef.cpp CellRef.hpp
	Color.cpp Color.hpp
	country.hxx
	currency.cc currency.hxx
	decl.hxx
	DurationInfo.cpp DurationInfo.hpp
	err.hpp
	eval.cc
	filename.hxx
	Formula.cpp Formula.hpp formula.hxx
	FormulaError.cpp FormulaError.hpp
	FormulaNode.cpp FormulaNode.hpp
	Function.cpp Function.hpp
	function.cc
	global.hxx
	HAlign.cpp HAlign.hpp
	id.cc
	io.cc
	lang.hxx
	Length.cpp Length.hpp
	LineStyle.cpp LineStyle.hpp line.hxx
	ndff/Container.cpp ndff/Container.hpp
	ndff/FileEntryInfo.cpp ndff/FileEntryInfo.hpp
	ndff/ndff.cc
	ndff/Property.cpp ndff/Property.hpp
	Ns.cpp Ns.hpp ns.hxx
	ods ods.cc ods.hxx
	op.cc
	Prefix.cpp Prefix.hpp
	Reference.cpp Reference.hpp
	Row.cpp Row.hpp
	Sheet.cpp Sheet.hpp
	StringOrInst.cpp StringOrInst.hpp
	StringOrTag.cpp StringOrTag.hpp
	style.cc style.hxx
	Tag.cpp Tag.hpp
	Time.cpp Time.hpp
	types.hxx
	VAlign.cpp VAlign.hpp
	zip.cc
	
	attr/Border.cpp attr/Border.hpp
	attr/decl.hxx
	attr/FoFontStyle.cpp attr/FoFontStyle.hpp
	attr/FoFontWeight.cpp attr/FoFontWeight.hpp
	attr/StyleBorderLineWidth.cpp attr/StyleBorderLineWidth.hpp
	attr/StyleTextUnderlineColor.cpp attr/StyleTextUnderlineColor.hpp
	attr/StyleTextUnderlineWidth.cpp attr/StyleTextUnderlineWidth.hpp

	inst/Abstract.cpp inst/Abstract.hpp
	inst/decl.hxx
	inst/DcDate.cpp inst/DcDate.hpp
	inst/DcTitle.cpp inst/DcTitle.hpp
	inst/DrawFrame.cpp inst/DrawFrame.hpp
	inst/DrawImage.cpp inst/DrawImage.hpp
	inst/ManifestFileEntry.cpp inst/ManifestFileEntry.hpp
	inst/ManifestManifest.cpp inst/ManifestManifest.hpp
	inst/MetaCreationDate.cpp inst/MetaCreationDate.hpp
	inst/MetaDocumentStatistic.cpp inst/MetaDocumentStatistic.hpp
	inst/MetaEditingCycles.cpp inst/MetaEditingCycles.hpp
	inst/MetaEditingDuration.cpp inst/MetaEditingDuration.hpp
	inst/MetaGenerator.cpp inst/MetaGenerator.hpp
	inst/MetaTemplate.cpp inst/MetaTemplate.hpp
	inst/NumberBoolean.cpp inst/NumberBoolean.hpp
	inst/NumberBooleanStyle.cpp inst/NumberBooleanStyle.hpp
	inst/NumberCurrencyStyle.cpp inst/NumberCurrencyStyle.hpp
	inst/NumberCurrencySymbol.cpp inst/NumberCurrencySymbol.hpp
	inst/NumberDay.cpp inst/NumberDay.hpp
	inst/NumberDateStyle.cpp inst/NumberDateStyle.hpp
	inst/NumberHours.cpp inst/NumberHours.hpp
	inst/NumberMinutes.cpp inst/NumberMinutes.hpp
	inst/NumberMonth.cpp inst/NumberMonth.hpp
	inst/NumberNumber.cpp inst/NumberNumber.hpp
	inst/NumberNumberStyle.cpp inst/NumberNumberStyle.hpp
	inst/NumberPercentageStyle.cpp inst/NumberPercentageStyle.hpp
	inst/NumberSeconds.cpp inst/NumberSeconds.hpp
	inst/NumberTimeStyle.cpp inst/NumberTimeStyle.hpp
	inst/NumberText.cpp inst/NumberText.hpp
	inst/NumberTextContent.cpp inst/NumberTextContent.hpp
	inst/NumberTextStyle.cpp inst/NumberTextStyle.hpp
	inst/NumberYear.cpp inst/NumberYear.hpp
	inst/OfficeAutomaticStyles.cpp inst/OfficeAutomaticStyles.hpp
	inst/OfficeBody.cpp inst/OfficeBody.hpp
	inst/OfficeDocumentContent.cpp inst/OfficeDocumentContent.hpp
	inst/OfficeDocumentMeta.cpp inst/OfficeDocumentMeta.hpp
	inst/OfficeDocumentStyles.cpp inst/OfficeDocumentStyles.hpp
	inst/OfficeFontFaceDecls.cpp inst/OfficeFontFaceDecls.hpp
	inst/OfficeMasterStyles.cpp inst/OfficeMasterStyles.hpp
	inst/OfficeMeta.cpp inst/OfficeMeta.hpp
	inst/OfficeScripts.cpp inst/OfficeScripts.hpp
	inst/OfficeSpreadsheet.cpp inst/OfficeSpreadsheet.hpp
	inst/OfficeStyles.cpp inst/OfficeStyles.hpp
	inst/StyleBackgroundImage.cpp inst/StyleBackgroundImage.hpp
	inst/StyleDefaultStyle.cpp inst/StyleDefaultStyle.hpp
	inst/StyleFontFace.cpp inst/StyleFontFace.hpp
	inst/StyleFooter.cpp inst/StyleFooter.hpp
	inst/StyleFooterLeft.cpp inst/StyleFooterLeft.hpp
	inst/StyleFooterStyle.cpp inst/StyleFooterStyle.hpp
	inst/StyleHeader.cpp inst/StyleHeader.hpp
	inst/StyleHeaderFooterProperties.cpp inst/StyleHeaderFooterProperties.hpp
	inst/StyleHeaderLeft.cpp inst/StyleHeaderLeft.hpp
	inst/StyleHeaderStyle.cpp inst/StyleHeaderStyle.hpp
	inst/StyleMap.cpp inst/StyleMap.hpp
	inst/StyleMasterPage.cpp inst/StyleMasterPage.hpp
	inst/StylePageLayout.cpp inst/StylePageLayout.hpp
	inst/StylePageLayoutProperties.cpp inst/StylePageLayoutProperties.hpp
	inst/StyleParagraphProperties.cpp inst/StyleParagraphProperties.hpp
	inst/StyleRegionLeft.cpp inst/StyleRegionLeft.hpp
	inst/StyleRegionRight.cpp inst/StyleRegionRight.hpp
	inst/StyleStyle.cpp inst/StyleStyle.hpp
	inst/StyleTableCellProperties.cpp inst/StyleTableCellProperties.hpp
	inst/StyleTableColumnProperties.cpp inst/StyleTableColumnProperties.hpp
	inst/StyleTableProperties.cpp inst/StyleTableProperties.hpp
	inst/StyleTableRowProperties.cpp inst/StyleTableRowProperties.hpp
	inst/StyleTextProperties.cpp inst/StyleTextProperties.hpp
	inst/SvgDesc.cpp inst/SvgDesc.hpp
	inst/SvgTitle.cpp inst/SvgTitle.hpp
	inst/TableCalculationSettings.cpp inst/TableCalculationSettings.hpp
	inst/TableNamedExpressions.cpp inst/TableNamedExpressions.hpp
	inst/TableNamedRange.cpp inst/TableNamedRange.hpp
	inst/TableTableColumn.cpp inst/TableTableColumn.hpp
	inst/TextDate.cpp inst/TextDate.hpp
	inst/TextP.cpp inst/TextP.hpp
	inst/TextPageCount.cpp inst/TextPageCount.hpp
	inst/TextPageNumber.cpp inst/TextPageNumber.hpp
	inst/TextS.cpp inst/TextS.hpp
	inst/TextSheetName.cpp inst/TextSheetName.hpp
	inst/TextTime.cpp inst/TextTime.hpp
	inst/TextTitle.cpp inst/TextTitle.hpp
)

foreach(f IN LISTS lib_files)
	get_filename_component(b ${f} NAME)
	set_source_files_properties(${f} PROPERTIES COMPILE_DEFINITIONS "SRC_FILE_NAME=\"${b}\"")
endforeach()

if (DEFINED ODS_STATIC)
	set(lib_type STATIC)
else (DEFINED ODS_STATIC)
	set(lib_type SHARED)
	add_definitions(-DODS_SHARED -DODS_BUILDING)
endif (DEFINED ODS_STATIC)

add_library(ods ${lib_type} ${lib_files})
target_link_libraries(ods PRIVATE Qt6::Core Qt${QT_VERSION_MAJOR}::Gui zip z)
