/*
 * Copyright © 2014 f35f22fan@gmail.com
 *
 * Read the LICENSE file that comes with this project for license details.
*/

#ifndef ODS_DURATIONINFO_HPP_
#define ODS_DURATIONINFO_HPP_

#include "err.hpp"
#include "global.hxx"

namespace ods	{ // ods::

namespace duration { // ods::duration::

enum class Order : quint8 {
	HOURS_MINUTES,
	HOURS_MINUTES_SECONDS
};

enum class Separator : quint8 {
	COLON
};

} // ods::duration::

class ODS_API DurationInfo
{
public:
	DurationInfo();
	virtual ~DurationInfo();

	void
	CopyFrom(const DurationInfo&);

	const QString&
	data_style_name() const { return data_style_name_; }

	void
	data_style_name_set(const QString &s) { data_style_name_ = s; }

	bool
	Equals(const ods::DurationInfo &t) const;

	const QString&
	hours_number_style() const { return hours_number_style_; }

	const QString&
	minutes_number_style() const { return minutes_number_style_; }

	const QString&
	name() const { return name_; }

	ods::duration::Order
	order() const { return order_; }

	void
	order_set(const ods::duration::Order &r) { order_ = r; }

	const QString&
	seconds_number_style() const { return seconds_number_style_; }

	ods::duration::Separator
	separator() const { return separator_; }

	void
	separator_set(const ods::duration::Separator &r) { separator_ = r; }

	QString
	SeparatorAsString() const;

	bool
	truncate_on_overflow() const { return truncate_on_overflow_; }

	void
	truncate_on_overflow_set(const bool r) { truncate_on_overflow_ = r; }

private:
	NO_ASSIGN_COPY_MOVE(DurationInfo);

	QString data_style_name_;
	QString hours_number_style_ = "long";
	QString	minutes_number_style_ = "long";
	QString	seconds_number_style_ = "long";
	QString	name_;
	bool	truncate_on_overflow_ = false;
	ods::duration::Order	order_ = ods::duration::Order::HOURS_MINUTES;
	ods::duration::Separator separator_ = ods::duration::Separator::COLON;

};

} // ods::

#endif
