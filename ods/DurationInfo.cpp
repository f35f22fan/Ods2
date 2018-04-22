/*
 * Copyright © 2014 f35f22fan@gmail.com
 *
 * Read the LICENSE file that comes with this project for license details.
*/

#include "DurationInfo.hpp"

namespace ods	{ // ods::

DurationInfo::DurationInfo()
{}

DurationInfo::~DurationInfo()
{}

void
DurationInfo::CopyFrom(const DurationInfo &r)
{
	data_style_name_ = r.data_style_name();
	order_ = r.order();
	separator_ = r.separator();
	truncate_on_overflow_ = r.truncate_on_overflow();
}

bool
DurationInfo::Equals(const ods::DurationInfo &r) const
{
	if (data_style_name_ != r.data_style_name())
		return false;
	if (hours_number_style_ != r.hours_number_style())
		return false;
	if (minutes_number_style_ != r.minutes_number_style())
		return false;
	if (seconds_number_style_ != r.seconds_number_style())
		return false;
	if (separator_ != r.separator())
		return false;
	if (truncate_on_overflow_ != r.truncate_on_overflow())
		return false;
	return true;
}

QString
DurationInfo::SeparatorAsString() const
{
	if (separator_ == ods::duration::Separator::COLON)
		return QLatin1String(":");
	return QLatin1String(":"); // add more options if ever needed
}

} // ods::
