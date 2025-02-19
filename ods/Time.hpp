/*
 * Copyright © 2014 f35f22fan@gmail.com
 *
 * Read the LICENSE file that comes with this project for license details.
*/

#pragma once

#include <QtGlobal>
#include <QtCore/QString>
#include <QTime>

#include "global.hxx"
#include "types.hxx"

namespace ods { // ods::

const i64 S = 1000;
const i64 M = S * 60;
const i64 H = M * 60;

class ODS_API Time
{
public:
	Time();
	Time(const Time &rhs);
	Time(const QTime &rhs);
	Time(ci32 h, ci32 m, ci32 s);
	virtual ~Time();
	
	void Apply(ci32 h, ci32 m, ci32 s);
	Time* Clone() const;
	bool Parse(const QString &str);
	bool ParseSimple(QString input);
	
	void operator+=(const ods::Time &rhs);
	void operator-=(const ods::Time &rhs);
	Time operator+(const Time &rhs) const;
	Time operator-(const Time &rhs) const;
	
	bool operator>(const ods::Time &rhs) const;
	bool operator<(const ods::Time &rhs) const;
	bool operator>=(const ods::Time &rhs) const;
	bool operator<=(const ods::Time &rhs) const;
	bool operator==(const ods::Time &rhs) const;
	bool operator!=(const ods::Time &rhs) const;
	
	operator QString();
	
	i32 hours() const { return h_; }
	i32 minutes() const { return m_; }
	i32 seconds() const { return s_; }
	
	void add_seconds(ci32 s);
	void add_minutes(ci32 m);
	void add_hours(ci32 h);
	
	void subtract_seconds(ci32 s);
	void subtract_minutes(ci32 m);
	void subtract_hours(ci32 h);
	
	QString toString() const;
	
	bool valid() const {
		return h_ != std::numeric_limits<i32>::min() ||
			m_ != std::numeric_limits<i32>::min() ||
			s_ != std::numeric_limits<i32>::min();
	}
	
private:
	i32 total() const { return h_ * (60 * 60) + m_ * 60 + s_; }
	i32 h_ = std::numeric_limits<i32>::min();;
	i32 m_ = std::numeric_limits<i32>::min();;
	i32 s_ = std::numeric_limits<i32>::min();;
};

} // ods::

