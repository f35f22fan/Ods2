#pragma once

#include "global.hxx"
#include "err.hpp"
#include <cmath>

namespace ods { // ods::cmp_num::
namespace cmp_num { // ods::cmp_num::

ODS_API bool
ApproximatelyEqual(float a, float b, float epsilon)
{
	return fabs(a - b) <= ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}

ODS_API bool
ApproximatelyEqual(double a, double b, double epsilon)
{
	return fabs(a - b) <= ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}

ODS_API bool
EssentiallyEqual(float a, float b, float epsilon)
{
	return fabs(a - b) <= ( (fabs(a) > fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}

ODS_API bool
EssentiallyEqual(double a, double b, double epsilon)
{
	return fabs(a - b) <= ( (fabs(a) > fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}

ODS_API bool
DefinitelyGreaterThan(float a, float b, float epsilon)
{
	return (a - b) > ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}

ODS_API bool
DefinitelyGreaterThan(double a, double b, double epsilon)
{
	return (a - b) > ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}

ODS_API bool
DefinitelyLessThan(float a, float b, float epsilon)
{
	return (b - a) > ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}

ODS_API bool
DefinitelyLessThan(double a, double b, double epsilon)
{
	return (b - a) > ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}

} // ods::cmp_num::
} // ods::