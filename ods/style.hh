#pragma once

#include "err.hpp"
#include "style.hxx"

namespace ods { // ods::
namespace style { // ods::style::

style::Family
FamilyFromString(const QString &s);

const char*
FamilyToString(const style::Family f);

} // ods::style::
} // ods::
