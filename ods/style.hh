#pragma once

#include "err.hpp"
#include "style.hxx"

namespace ods::style {

style::Family
FamilyFromString(QStringView s);

QStringView
FamilyToString(const style::Family f);

} // ods::style::
