#include <QGuiApplication>

#include <iostream>

#include "examples1.hh"
#include "examples2.hh"
#include "Invoice.hpp"

#include <ods/ods>

int main(int argc, char *argv[])
{
	QGuiApplication qapp(argc, argv);

#if defined(_WIN32)
// On Windows this enables text to be printed to the terminal in colors
	ods::EnableConsoleColors();
#endif

//  TestBug1Feb2025();
//	TestBugJuly2023();
	
	
	// The function calls below come from examples(1/2).cc
	
	
	// SetColumnWidths();
	// ReadColumnWidths();
	
	// CreateFont();
	// ReadFont();
	
	// CreateColorsAndUnderline();
	// ReadColorsAndUnderline();
	
	// CellSpan();
	// ReadCellSpan();
	
	// CreatePercentage();
	// ReadPercentage();
	
	// CreateHAlignment();
	// ReadHAlignment();
	
	// CreateVAlignment();
	// ReadVAlignment();
	
	// CreateBorders();
	// ReadBorders();
	
	// CreateImage();
	// ReadImage();
	
	// CreateDate();
	// ReadDate();
	
	// CreateTime();
	// ReadTime();
	
	// CreateBoolean();
	// ReadBoolean();
	
	// CreateCurrency();
	// ReadCurrency();
	
	// CreateFormula();
	// ReadFormula();
	
	//CreateFormulaFunctions();
	
	// auto *invoice = new sample::Invoice();
	// ods::AutoDelete ad_p(invoice);
	
	
	CreateRowHeight();
	//ReadRowHeight();
	
	
	
	
	
	// Nevermind this block, it's purely for development purposes:
	// ReadWriteNDFF(p->book()->ndff_path());
	// GenerateFunctionsListForGitHub();
	
	return 0;
}
