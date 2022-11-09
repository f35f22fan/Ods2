#include <QGuiApplication>

#include <iostream>

#include "examples1.hh"
#include "examples2.hh"
#include "Invoice.hpp"

#include <ods/types.hxx>

int
main(int argc, char *argv[])
{
	QGuiApplication qapp(argc, argv);
//	TestBug();
	
//	SetColumnWidths();
//	ReadColumnWidths();
	
//	CreateFont();
//	ReadFont();
	
//	CreateColorsAndUnderline();
//	ReadColorsAndUnderline();
	
//	CellSpan();
//	ReadCellSpan();
	
//	CreatePercentage();
//	ReadPercentage();
	
//	CreateHAlignment();
//	ReadHAlignment();
	
//	CreateVAlignment();
//	ReadVAlignment();
	
//	CreateBorders();
//	ReadBorders();
	
//	CreateImage();
//	ReadImage();
	
//	CreateDate();
//	ReadDate();
	
//	CreateCurrency();
//	ReadCurrency();
	
//	CreateTime();
//	ReadTime();
	
//	CreateBoolean();
//	ReadBoolean();
	
//	CreateFormula();
//	ReadFormula();
//	ReadFormulaCustom();
	
// CreateFormulaFunctions();
	new sample::Invoice();
	
//	GenerateFunctionsListForGitHub();
//	ReadCellRange();
	
	return 0;
}


