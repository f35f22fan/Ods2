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
	
	CopyAnOdsFile();
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
	
 //CreateFormulaFunctions();
// new sample::Invoice();

//	GenerateFunctionsListForGitHub();
//	ReadCellRange();
	
	if (false)
	{ // Nevermind this block, it's purely for development purposes
		auto path = QDir::home().filePath("out.ndff");
		ReadWriteNDFF(path);
	}
	
	return 0;
}


