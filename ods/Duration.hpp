/*
 * Copyright © 2014 f35f22fan@gmail.com
 *
 * Read the LICENSE file that comes with this project for license details.
*/

#pragma once

#include <QtGlobal>
#include <QtCore/QString>

#include "global.hxx"

namespace ods { // ods::

class ODS_API Duration
{
public:
	Duration();
	Duration(const int32_t h, const int32_t m, const int32_t s);
	virtual ~Duration();
	
	Duration*
	Clone() const;
	
	void
	Decode(const QString &str);
	
	static Duration*
	New(const qint32 h, const qint32 m, const qint32 s);
	
	bool
	operator==(const ods::Duration &rhs) const;
	
	operator QString();
	
	qint32
	days() const { return days_; }
	
	void
	days(const qint32 n) { days_ = n; }
	
	qint32
	hours() const { return hours_; }
	
	void
	hours(const qint32 n) { hours_ = n; }
	
	bool
	IsValid() const;
	
	qint32
	minutes() const { return minutes_; }
	
	void
	minutes(const qint32 n) { minutes_ = n; }
	
	qint32
	months() const { return months_; }
	
	void
	months(const qint32 n) { months_ = n; }
	
	qint32
	seconds() const { return seconds_; }
	
	void
	seconds(const qint32 n) { seconds_ = n; }
	
	QString
	toString() const;
	
	qint32
	years() const { return years_; }
	
	void
	years(const qint32 n) { years_ = n; }
	
private:
	
	qint32 years_ = -1;
	qint32 months_ = -1;
	qint32 days_ = -1;
	qint32 hours_ = -1;
	qint32 minutes_ = -1;
	qint32 seconds_ = -1;
};

} // ods::

