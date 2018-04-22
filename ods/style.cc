#include "style.hh"

#include "ns.hxx"

namespace ods { // ods::
namespace style { // ods::style::

style::Family
FamilyFromString(const QString &s)
{
	if (s == ods::ns::kTableCell)
		return style::Family::Cell;
	
	if (s == ods::ns::kTableRow)
		return style::Family::Row;
	
	if (s == ods::ns::kTable)
		return style::Family::Table;
	
	if (s == ods::ns::kTableColumn)
		return style::Family::Column;
	
	return style::Family::None;
}

const char*
FamilyToString(const style::Family f)
{
	switch (f)
	{
	case style::Family::Cell: return ods::ns::kTableCell;
	case style::Family::Row: return ods::ns::kTableRow;
	case style::Family::Table: return ods::ns::kTable;
	case style::Family::Column: return ods::ns::kTableColumn;
	default: return nullptr;
	}
}

} // ods::style::
} // ods::
