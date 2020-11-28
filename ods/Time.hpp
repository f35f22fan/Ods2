/*
 * Copyright © 2014 f35f22fan@gmail.com
 *
 * Read the LICENSE file that comes with this project for license details.
*/

#pragma once

#include <QtGlobal>
#include <QtCore/QString>

#include "global.hxx"
#include "types.hxx"

namespace ods { // ods::

const i32 S = 1000;
const i32 M = S * 60;
const i32 H = M * 60;

class ODS_API Time
{
public:
	Time();
	Time(const Time &rhs);
	Time(const i32 h, const i32 m, const i32 s, const int ms = 0);
	virtual ~Time();
	
	Time* Clone() const;
	bool Parse(const QString &str);
	
	static Time*
	New(const i32 h, const i32 m, const i32 s);
	
	void operator+=(const ods::Time &rhs);
	void operator-=(const ods::Time &rhs);
	Time operator+(const Time &rhs) const;
	Time operator-(const Time &rhs) const;
	
	bool operator>(const ods::Time &rhs) const { return ms_ > rhs.ms_; }
	bool operator<(const ods::Time &rhs) const { return ms_ < rhs.ms_; }
	bool operator>=(const ods::Time &rhs) const { return ms_ >= rhs.ms_; }
	bool operator<=(const ods::Time &rhs) const { return ms_ <= rhs.ms_; }
	bool operator==(const ods::Time &rhs) const { return ms_ == rhs.ms_; }
	bool operator!=(const ods::Time &rhs) const { return ms_ != rhs.ms_; }
	
	operator QString();
	
	i32 hours() const { return ms_ / H; }
	void hours(const i32 n);
	i32 minutes() const;
	void minutes(const i32 n);
	i32 seconds() const;
	void seconds(const i32 n);
	void ms(const i32 n);
	i32 ms() const;
	
	void add_ms(const i32 ms) { ms_ += ms; }
	void add_seconds(const i32 s) { ms_ += s * S; }
	void add_minutes(const i32 m) { ms_ += m * M; }
	void add_hours(const i32 h) { ms_ += h * H; }
	
	void subtract_ms(const i32 ms) { ms_ -= ms; }
	void subtract_seconds(const i32 s) { subtract_ms(s * S); }
	void subtract_minutes(const i32 m) { subtract_ms(m * M); }
	void subtract_hours(const i32 h) { subtract_ms(h * H); }
	
	QString toString() const;
	i32 total_ms() const { return ms_; }
	void total_ms(const i32 n) { ms_ = n; }
	
private:
	static void DeepCopy(Time &lhs, const Time &rhs);
	
	i32 ms_ = 0;
};

} // ods::

