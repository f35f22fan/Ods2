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
	
	TestBug1Feb2025();
	
//	TestBugJuly2023();
	
//	CopyAnOdsFile();
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
	
	//CreateFormula();
//	ReadFormula();
	
 //CreateFormulaFunctions();
	
//	GenerateFunctionsListForGitHub();
//	ReadCellRange();
	
	// auto *p = new sample::Invoice(ods::EnableNdff::No);
	// ods::AutoDelete ad_p(p);
	
	if (false)
	{ // Nevermind this block, it's purely for development purposes
		//ReadWriteNDFF(p->book()->ndff_path());
	}
	
	return 0;
}


