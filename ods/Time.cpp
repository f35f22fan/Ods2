/*
 * Copyright © 2014 f35f22fan@gmail.com
 *
 * Read the LICENSE file that comes with this project for license details.
*/

#include "Time.hpp"
#include "err.hpp"

#include <QtCore/QDebug>

namespace ods	{

Time::Time()
{}

Time::Time(const Time &rhs)
{
	h_ = rhs.h_;
	m_ = rhs.m_;
	s_ = rhs.s_;
}

Time::Time(ci32 h, ci32 m, ci32 s)
{
	Apply(h, m, s);
}

Time::Time(const QTime &t)
{
	Apply(t.hour(), t.minute(), t.second());
}

Time::~Time() {}

bool Time::operator>(const ods::Time &rhs) const {
	return total() > rhs.total();
}
bool Time::operator<(const ods::Time &rhs) const {
	return total() < rhs.total();
}
bool Time::operator>=(const ods::Time &rhs) const {
	return total() >= rhs.total();
}
bool Time::operator<=(const ods::Time &rhs) const {
	return total() <= rhs.total();
}
bool Time::operator==(const ods::Time &rhs) const {
	return total() == rhs.total();
}
bool Time::operator!=(const ods::Time &rhs) const {
	return total() != rhs.total();
}

void Time::Apply(ci32 h, ci32 m, ci32 s) {
	h_ = m_ = s_ = 0;
	add_seconds(s);
	add_minutes(m);
	add_hours(h);
}

Time*
Time::Clone() const
{
	return new Time(h_, m_, s_);
}

Time
Time::operator+(const Time &rhs) const {
	Time d(*this);
	d += rhs;
	return d;
}

void
Time::operator+=(const ods::Time &rhs)
{
	add_hours(rhs.hours());
	add_minutes(rhs.minutes());
	add_seconds(rhs.seconds());
}

Time
Time::operator-(const Time &rhs) const {
	Time d(*this);
	d -= rhs;
	return d;
}

void Time::operator-=(const ods::Time &rhs)
{
	subtract_hours(rhs.h_);
	subtract_minutes(rhs.m_);
	subtract_seconds(rhs.s_);
}

void Time::add_seconds(ci32 s) {
	s_ += s;
	i32 minutes = s_ / 60;
	s_ %= 60;
	if (s_ < 0) {
		s_ = 60 + s_;
		minutes--;
	}
	if (minutes != 0) {
		add_minutes(minutes);
	}
}

void Time::add_minutes(ci32 m) {
	m_ += m;
	i32 hours = m_ / 60;
	m_ %= 60;
	if (m_ < 0) {
		m_ = 60 + m_;
		hours--;
	}
	if (hours != 0) {
		add_hours(hours);
	}
}
void Time::add_hours(ci32 h) {
	h_ += h;
	h_ %= 24;
}

bool Time::Parse(const QString &orig_str)
{
	i32 hrs = 0;
	i32 min = 0;
	i32 sec = 0;
	i32 ms = 0;
	bool negative = false;
	
	QStringView str_ref(orig_str);
	if (str_ref.startsWith(QChar('-'))) {
		negative = true;
		str_ref = str_ref.mid(1);
	}
	// "PT07H14M22.015S", "PT00H00M02S", "-PT10H10M01S"
	if (!str_ref.startsWith(QLatin1String("PT"))) {
		mtl_trace();
		return false;
	}
	str_ref = str_ref.mid(2);
	bool ok;
	
	int h_index = str_ref.indexOf('H');
	MTL_CHECK(h_index != -1);
	QStringView hours = str_ref.mid(0, h_index);
	i32 n = hours.toInt(&ok);
	MTL_CHECK(ok);
	hrs = n;
	
	str_ref = str_ref.mid(h_index + 1);
	int m_index = str_ref.indexOf('M');
	MTL_CHECK(m_index != -1);
	auto min_ref = str_ref.mid(0, m_index);
	QStringView minutes = min_ref.mid(0, 2);
	n = minutes.toInt(&ok);
	MTL_CHECK(ok);
	min = n;
	
	str_ref = str_ref.mid(m_index + 1);
	MTL_CHECK(str_ref.endsWith('S'));
	str_ref = str_ref.left(str_ref.size() - 1);
	
	int index = str_ref.indexOf('.');
	
	if (index == -1) {
		n = str_ref.toInt(&ok);
		MTL_CHECK(ok);
		sec = n;
	} else {
		QStringView seconds = str_ref.mid(0, index);
		n = seconds.toInt(&ok);
		MTL_CHECK(ok);
		sec = n;
		
		str_ref = str_ref.mid(index + 1);
		n = str_ref.toInt(&ok);
		MTL_CHECK(ok);
		ms = n;
	}
	
	if (negative) {
		h_ = -h_;
		m_ = -m_;
		s_ = -s_;
	}
	
	return true;
}

bool Time::ParseSimple(QString input) {
	auto list = input.split(':');
	if (list.size() != 3) {
		return false;
	}
	
	bool ok;
	i32 h = list[0].toInt(&ok);
	if (!ok) return false;
	i32 m = list[1].toInt(&ok);
	if (!ok) return false;
	i32 s = list[2].toInt(&ok);
	if (!ok) return false;
	
	//mtl_info("h=%d, m=%d, s=%d", h, m, s);
	Apply(h, m, s);
	//mtl_info("h=%d, m=%d, s=%d", h_, m_, s_);
	
	return true;
}

void Time::subtract_seconds(ci32 s) {
	s_ -= s;
	if (s_ >= 0)
		return;
	i32 minutes = s_ / 60;
	s_ = 60 - (s_ % 60);
	if (s_ > 0)
		minutes++;
	subtract_minutes(minutes);
}

void Time::subtract_minutes(ci32 m) {
	m_ -= m;
	if (m_ >= 0)
		return;
	i32 hours = m_ / 60;
	m_ = 60 - (m_ % 60);
	if (m_ > 0)
		hours++;
	subtract_hours(hours);
}

void Time::subtract_hours(ci32 h) {
	h_ -= h;
}

Time::operator QString()
{
	return toString();
}

QString
Time::toString() const
{
	// office:time-value="PT07H14M22.015S" presented as:
	// <text:p>07:14:22.015</text:p>
	QString str = QLatin1String("PT");
	
	if (h_ < 0 || m_ < 0 || s_ < 0)
		str = QChar('-') + str;
	
	str += QString::number(std::abs(hours()));
	str += 'H';
	str += QString::number(std::abs(minutes()));
	str += 'M';
	str += QString::number(std::abs(seconds()));
	str += 'S';
	return str;
}

} // ods::
