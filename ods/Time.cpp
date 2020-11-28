/*
 * Copyright © 2014 f35f22fan@gmail.com
 *
 * Read the LICENSE file that comes with this project for license details.
*/

#include "Time.hpp"
#include "err.hpp"

#include <QtCore/QDebug>
#include <QtCore/QStringRef>

namespace ods	{

Time::Time()
{}

Time::Time(const Time &rhs)
{
	DeepCopy(*this, rhs);
}

Time::Time(const i32 h, const i32 m, const i32 s, const int msec)
{
	hours(h);
	minutes(m);
	seconds(s);
    ms(msec);
}

Time::~Time()
{}

Time*
Time::Clone() const
{
	auto *p = new Time();
	DeepCopy(*p, *this);
	return p;
}

void
Time::DeepCopy(Time &lhs, const Time &rhs)
{
	lhs.ms_ = rhs.ms_;
}

Time*
Time::New(const i32 h, const i32 m, const i32 s)
{
	auto *p = new Time();
	p->ms_ = h * H + m * M + s * S;
	return p;
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
	ms_ += rhs.ms_;
}

Time
Time::operator-(const Time &rhs) const {
	Time d(*this);
	d -= rhs;
	return d;
}

void
Time::operator-=(const ods::Time &rhs)
{
	ms_ -= rhs.ms_;
}

bool
Time::Parse(const QString &orig_str)
{
	i32 hrs = 0;
	i32 min = 0;
	i32 sec = 0;
	i32 ms = 0;
	bool negative = false;
	
	QStringRef str_ref = orig_str.midRef(0);
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
	RET_IF_EQUAL(h_index, -1);
	QStringRef hours = str_ref.mid(0, h_index);
	i32 n = hours.toInt(&ok);
	CHECK_TRUE(ok);
	hrs = n;
	
	str_ref = str_ref.mid(h_index + 1);
	int m_index = str_ref.indexOf('M');
	RET_IF_EQUAL(m_index, -1);
	auto min_ref = str_ref.mid(0, m_index);
	QStringRef minutes = min_ref.mid(0, 2);
	n = minutes.toInt(&ok);
	CHECK_TRUE(ok);
	min = n;
	
	
	str_ref = str_ref.mid(m_index + 1);
	CHECK_TRUE((str_ref.endsWith('S')));
	str_ref = str_ref.left(str_ref.size() - 1);
	
	int index = str_ref.indexOf('.');
	
	if (index == -1) {
		n = str_ref.toInt(&ok);
		CHECK_TRUE(ok);
		sec = n;
	} else {
		QStringRef seconds = str_ref.mid(0, index);
		n = seconds.toInt(&ok);
		CHECK_TRUE(ok);
		sec = n;
		
		str_ref = str_ref.mid(index + 1);
		n = str_ref.toInt(&ok);
		CHECK_TRUE(ok);
		ms = n;
	}
	
//	mtl_info("%d, %d, %d, %d", hrs, min, sec, ms);
	ms_ = hrs * H + min * M + sec * S + ms;
	
	if (negative)
		ms_ = -ms_;
	
	return true;
}

Time::operator QString()
{
	return toString();
}

void Time::ms(const i32 n) {
	ms_ = ms_ - ms() + n;
}

i32 Time::ms() const {
	return ms_ % 1000;
}

i32 Time::seconds() const {
	const i32 n = ms_ - hours() * H - minutes() * M;
	return n / 1000;
}

void Time::seconds(const i32 n) {
	const i32 sec_ms = seconds() * S;
	ms_ = ms_ - sec_ms + n * S;
}

i32 Time::minutes() const {
	i32 m = ms_ - hours() * H;
	return m / M;
}

void Time::minutes(const i32 num) {
	const i32 hrs_ms = hours() * H;
	const i32 no_hrs = ms_ - hrs_ms;
	const i32 min_ms = minutes() * M;
	const i32 below_min = no_hrs - min_ms;
	ms_ = hrs_ms + num * M + below_min;
}

void
Time::hours(const i32 n) {
	ms_ = ms_ % H;
	ms_ += H * n;
}

QString
Time::toString() const
{
	// office:time-value="PT07H14M22.015S" presented as:
	// <text:p>07:14:22.015</text:p>
	QString str = QLatin1String("PT");
	
	if (ms_ < 0)
		str = QChar('-') + str;
	
	str += QString::number(std::abs(hours()));
	str += 'H';
	str += QString::number(std::abs(minutes()));
	str += 'M';
	str += QString::number(std::abs(seconds()));
	const i32 mil = std::abs(ms());
	
	if (mil > 0) {
		str += QChar('.');
		const QString mil_str = QString::number(mil);
		if (mil < 10)
			str += QLatin1String("00") + mil_str;
		else if (mil < 100)
			str += QChar('0') + mil_str;
		else
			str += mil_str;
	}
	str += 'S';
	return str;
}

} // ods::
