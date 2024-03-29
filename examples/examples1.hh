#pragma once

#include <QString>

void TestBugJuly2023();
void CopyAnOdsFile();

//==> Please ignore this
void ReadWriteNDFF(QStringView full_path);
void TestBug();
//<== Please ignore this

void SetColumnWidths();
void ReadColumnWidths();

void CreateFont();
void ReadFont();

void CreateColorsAndUnderline();
void ReadColorsAndUnderline();

void CellSpan();
void ReadCellSpan();

void CreatePercentage();
void ReadPercentage();

void CreateHAlignment();
void ReadHAlignment();

void CreateVAlignment();
void ReadVAlignment();

void CreateBorders();
void ReadBorders();

void CreateImage();
void ReadImage();

void CreateDate();
void ReadDate();

void CreateTime();
void ReadTime();

void CreateBoolean();
void ReadBoolean();

void TestReportedBug();
