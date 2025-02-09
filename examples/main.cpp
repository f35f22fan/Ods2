#include <QGuiApplication>

#include <iostream>

#include "examples1.hh"
#include "examples2.hh"
#include "Invoice.hpp"

#include <ods/types.hxx>
#include <ods/zip.hh>

int main(int argc, char *argv[])
{
	QGuiApplication qapp(argc, argv);
	
	//TestBug1Feb2025();
	
	// QList<ods::zip::File> files;
	// QString zip_filepath = QDir::home().absoluteFilePath("Downloads/Export_planning2.ods");
	// QString target_dir = QDir::home().absoluteFilePath("ZipOutput");
	// QString err;
	
	// if (!ods::zip::ListFiles(zip_filepath, files, &err)) {
	// 	mtl_warn("%s", qPrintable(err));
	// } else {
	// 	mtl_info("File count: %ld", files.size());
	// }
	
	// QList<QString> filepaths = ods::zip::ExtractFiles(zip_filepath, target_dir, &err);
	// if (!err.isEmpty()) {
	// 	mtl_warn("An error occurred: %s", qPrintable(err));
	// 	return 1;
	// }
	
	// mtl_info("The following files were unpacked:");
	// for (QString path: filepaths) {
	// 	mtl_info("%s", qPrintable(path));
	// }
	
	
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
	
	auto *invoice = new sample::Invoice(ods::EnableNdff::No);
	ods::AutoDelete ad_p(invoice);
	
	if (false)
	{ // Nevermind this block, it's purely for development purposes
		//ReadWriteNDFF(p->book()->ndff_path());
	}
	
	return 0;
}


