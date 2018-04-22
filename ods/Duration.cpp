/*
 * Copyright © 2014 f35f22fan@gmail.com
 *
 * Read the LICENSE file that comes with this project for license details.
*/

#include "Duration.hpp"
#include "err.hpp"

#include <QtCore/QDebug>
#include <QtCore/QStringRef>

namespace ods	{

Duration::Duration()
{}

Duration::Duration(const int32_t h, const int32_t m, const int32_t s)
: hours_(h), minutes_(m), seconds_(s)
{}

Duration::~Duration()
{}

Duration*
Duration::Clone() const
{
	auto *p = new Duration();
	p->years_ = years_;
	p->months_ = months_;
	p->days_ = days_;
	p->hours_ = hours_;
	p->minutes_ = minutes_;
	p->seconds_ = seconds_;
	
	return p;
}

void
Duration::Decode(const QString &str)
{
	int last_offset = 1;
	int i = last_offset + 1;
	bool is_month = true;
	bool ok;

	for (; i <= str.size(); i++)
	{
		QStringRef s(&str, last_offset, i - last_offset);
		int n = s.toInt(&ok);
		
		if (ok)
			continue;
		
		s = QStringRef(&str, last_offset, i - last_offset - 1);
		n = s.toInt(&ok);
		last_offset = i;
		const QChar c = str.at(i - 1);

		if (c == 'Y') {
			years_ = n;
		} else if (c == 'M') {
			if (is_month) {
				months_ = n;
			} else {
				minutes_ = n;
			}
		} else if (c == 'D') {
			days_ = n;
		} else if (c == 'H') {
			hours_ = n;
		} else if (c == 'S') {
			seconds_ = n;
		} else if (c == 'T') {
			is_month = false;
		} else {
			mtl_warn("unknown char");
			return;
		}
	}
}

bool
Duration::IsValid() const
{
	return years_ != -1 || months_ != -1 || days_ != -1 ||
		hours_ != -1 || minutes_ != -1 || seconds_ != -1;
}

Duration*
Duration::New(const qint32 h, const qint32 m, const qint32 s)
{
	auto *p = new Duration();
	p->hours_ = h;
	p->minutes_ = m;
	p->seconds_ = s;
	
	return p;
}

bool
Duration::operator==(const ods::Duration &rhs) const
{
	return years_ == rhs.years_ &&
		months_ == rhs.months_ &&
		days_ == rhs.days_ &&
		hours_ == rhs.hours_ &&
		minutes_ == rhs.minutes_ &&
		seconds_ == rhs.seconds_;
}

Duration::operator QString()
{
	return toString();
}

QString
Duration::toString() const
{
	QString str = QLatin1String("PT");
	str += QString::number(hours_);
	str += 'H';
	str += QString::number(minutes_);
	str += 'M';
	str += QString::number(seconds_);
	str += 'S';
	return str;
}

} // ods::
