#include "Length.hpp"

#include "ods.hh"
#include "ods.hxx"

namespace ods { // ods::

namespace length { // ods::length::

double
PtToPx(double pt, double dpi)
{
	return pt / 72 * dpi;
}

double
PxToPt(double px, double dpi)
{
	return px * 72 / dpi;
}

} // ods::length::

const char *Cm = "cm";
const char *In = "in";
const char *Mm = "mm";
const char *Pt = "pt";
const char *Px = "px";

Length::Length() {}

Length::Length(const double sz, const Measure fm)
: size_(sz), measure_(fm)
{}

Length::Length(const Length &fs)
: size_(fs.size()), measure_(fs.measure())
{}

Length::~Length() {}

bool
Length::operator > (Length const &rhs) const
{
	return toCm() > rhs.toCm();
}

Length*
Length::Clone() const
{
	auto *p = new Length();
	p->size_ = size_;
	p->measure_ = measure_;
	return p;
}

Length*
Length::FromString(const QString &str)
{
	if (str.isEmpty())
		return nullptr;
	
	int at = -1;
	
	for (int i = 0; i < str.size(); i++)
	{
		if (str.at(i) > '9')
		{
			at = i;
			break;
		}
	}
	
	if (at == -1)
	{
		auto ba = str.toLocal8Bit();
		mtl_warn("%s", ba.data());
		return nullptr;
	}
	
	QString num_str = str.left(at);
	bool ok;
	double num = num_str.toDouble(&ok);
	
	if (!ok)
	{
		auto ba = str.toLocal8Bit();
		mtl_warn("%s", ba.data());
		return nullptr;
	}
	
	QString fm_str = str.mid(at);
	auto fm = MeasureFromString(fm_str);
	return new Length(num, fm);
}

Measure
Length::MeasureFromString(const QString &s)
{
	if (s == Cm)
		return Measure::Cm;
	
	if (s == In)
		return Measure::In;
	
	if (s == Mm)
		return Measure::Mm;
	
	if (s == Pt)
		return Measure::Pt;
	
	if (s == Px)
		return Measure::Px;
	
	mtl_warn();
	return Measure::None;
}

const char*
Length::MeasureToString() const
{
	switch(measure_)
	{
	case Measure::Cm: return Cm;
	case Measure::In: return In;
	case Measure::Mm: return Mm;
	case Measure::Pt: return Pt;
	case Measure::Px: return Px;
	default: return nullptr;
	}
}

double
Length::toCm() const
{
	if (is_mm())
		return size_ / 10.0;
	if (is_cm())
		return size_;
	if (is_in())
		return ods::kCmInAnInch * size_;
	if (is_pt())
		return ods::kCmInAPoint * size_;
	
	if (is_px())
	{
		double pt = length::PxToPt(size_, ods::DPI());
		return ods::kCmInAPoint * pt;
	}
	
	mtl_warn();
}

double
Length::toPt() const
{
	if (is_pt())
		return size_;
	return toCm() * ods::kPointsInACm;
}

QString
Length::toString() const
{
	return QString::number(size_).append(MeasureToString());
}

} // ods::
