#include "util.hh"

#include <ods/ods>

#include <QDir>
#include <QFile>
#include <QStandardPaths>

namespace util {

QString FindFile(const QString &file_name)
{
	QString full_path = QString(ODS2_TEST_DIR)
		+ QLatin1String("/examples/test_files/") + file_name;
	
	auto ba = full_path.toLocal8Bit();
	
	if (QFile(full_path).exists())
	{
		mtl_info("Using file: %s", ba.data());
		return full_path;
	}
	
	mtl_warn("File not found: %s", ba.data());
	return QString();
}

void PrintBorder(ods::Cell *cell, const int row, const int col_index)
{
	mtl_info("Cell at %d:%d...", row, col_index);
	
	if (cell == nullptr)
	{
		mtl_warn("Cell == nullptr");
		return;
	}
	
	ods::inst::StyleStyle *style = cell->GetStyle();
	
	if (style == nullptr)
	{
		mtl_warn("style == nullptr");
		return;
	}
	
	auto *tcp = (ods::inst::StyleTableCellProperties*)
		style->Get(ods::Id::StyleTableCellProperties);
	
	if (tcp == nullptr)
	{
		mtl_warn("no table cell properties");
		return;
	}
	
	ods::attr::Border *next = tcp->border();
	
	if (next == nullptr)
	{
		mtl_info("border is null");
	} else {
		auto ba = next->toString().toLocal8Bit();
		mtl_info("border: %s", ba.data());
	}
	
	next = tcp->border_left();
	
	if (next == nullptr)
	{
		mtl_info("border left is null");
	} else {
		auto ba = next->toString().toLocal8Bit();
		mtl_info("border left: %s", ba.data());
	}
	
	next = tcp->border_top();
	
	if (next == nullptr)
	{
		mtl_info("border top is null");
	} else {
		auto ba = next->toString().toLocal8Bit();
		mtl_info("border top: %s", ba.data());
	}
	
	next = tcp->border_right();
	
	if (next == nullptr)
	{
		mtl_info("border right is null");
	} else {
		auto ba = next->toString().toLocal8Bit();
		mtl_info("border right: %s", ba.data());
	}
	
	next = tcp->border_bottom();
	
	if (next == nullptr)
	{
		mtl_info("border bottom is null");
	} else {
		auto ba = next->toString().toLocal8Bit();
		mtl_info("border bottom: %s", ba.data());
	}
}

void PrintPercentage(ods::Cell *cell)
{
	mtl_info("Cell column index: %d", cell->QueryStart());
	
	if (!cell->is_percentage())
	{
		mtl_warn("Cell != percentage");
		return;
	}
	
	double value = *cell->as_percentage();
	mtl_info("Cell percentage is: %f", value);
	
	auto *style = cell->GetStyle();
	
	if (style == nullptr)
	{
		mtl_warn("Cell has no style");
		return;
	}
	
	auto *ps = style->GetPercentageStyle();
	
	if (ps == nullptr)
	{
		mtl_warn("Cell has no percentage style");
		return;
	}
	
	auto *ns = ps->GetNumberStyle();
	
	if (ns == nullptr)
	{
		mtl_warn("Percentage style has no number style");
		return;
	}
	
	mtl_info("min integer digits: %d, decimal places: %d",
		ns->min_integer_digits(), ns->decimal_places());
	
}

void PrintWidth(ods::inst::TableTableColumn *col)
{
	if (col == nullptr)
	{
		mtl_warn("col == null");
		return;
	}
	
	ods::inst::StyleStyle *style = col->GetStyle();
	
	if (style == nullptr)
	{
		mtl_warn("No style");
		return;
	}
	
	auto *tcp = (ods::inst::StyleTableColumnProperties*)
		style->Get(ods::Id::StyleTableColumnProperties);
	
	if (tcp == nullptr)
	{
		for (ods::StringOrInst *node: *style->nodes())
		{
			if (node->is_string())
			{
				mtl_info("[String]");
				qDebug() << node->as_string();
			} else if (node->is_inst()) {
				mtl_info("[Inst]");
				ods::inst::Abstract *p = node->as_inst();
				mtl_info("func: %p", (void*)p->func());
				mtl_info("tcp func: %p", (void*) ods::id::StyleTableColumnProperties);

				auto ba = p->FullName().toLocal8Bit();
				mtl_info("Full name: %s", ba.data());

			} else {
				mtl_info("[OTHER]");
			}
		}

		const int size = style->nodes()->size();
		mtl_warn("No table column properties, nodes count: %d", size);
		return;
	}
	
	auto *width = tcp->column_width();
	
	if (width == nullptr)
	{
		mtl_warn("No width set");
		return;
	}
	
	auto ba = width->toString().toLocal8Bit();
	mtl_info("Column width: %s", ba.data());
}

void Save(ods::Book *book)//, const char *file_name)
{
	if (!book)
		return;
	
	//QString path = file_name ? file_name : "out.ods";
	QFile file(QDir::home().filePath("out.ods"));
	QString err;
	if (book->Save(file, &err))
	{
		mtl_info("Saved to: %s", qPrintable(file.fileName()));
	} else {
		mtl_warn("%s", qPrintable(err));
	}
}

} // util::
