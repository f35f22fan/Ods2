#include "style.hh"

#include "ns.hxx"

namespace ods::style {

style::Family
FamilyFromString(QStringView s)
{
	if (s == ns::kTableCell)
		return style::Family::Cell;
	
	if (s == ns::kTableRow)
		return style::Family::Row;
	
	if (s == ns::kTable)
		return style::Family::Table;
	
	if (s == ns::kTableColumn)
		return style::Family::Column;
	
	return style::Family::None;
}

QStringView FamilyToString(const style::Family f)
{
	switch (f)
	{
	case style::Family::Cell: return ns::kTableCell;
	case style::Family::Row: return ns::kTableRow;
	case style::Family::Table: return ns::kTable;
	case style::Family::Column: return ns::kTableColumn;
	default: return QString();
	}
}

} // ods::style::
