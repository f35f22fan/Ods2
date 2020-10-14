#pragma once

#include "err.hpp"
#include "global.hxx"

namespace ods { // ods::
namespace length { // ods::length::

double
PtToPx(double pt, double dpi);

double
PxToPt(double px, double dpi);

} // ods::length::

enum class Unit : uint8_t
{ // (pc|px|pt|mm|cm|in|em)
	Cm,
	In,
	Mm,
	Pt,
	Px,
	None
};

class ODS_API Length
{
public:
	Length(const double sz, const Unit fm);
	Length(const Length &fs);
	virtual ~Length();
	
	bool
	operator > (Length const &rhs) const;
	
	Length*
	Clone() const;
	
	static Length*
	FromString(const QString &str);
	
	bool
	is_cm() const { return measure_ == Unit::Cm; }
	
	bool
	is_in() const { return measure_ == Unit::In; }
	
	bool
	is_mm() const { return measure_ == Unit::Mm; }
	
	bool
	is_pt() const { return measure_ == Unit::Pt; }
	
	bool
	is_px() const { return measure_ == Unit::Px; }
	
	bool
	is_valid() const { return measure_ != Unit::None; }
	
	Unit
	measure() const { return measure_; }
	
	double
	size() const { return size_; }
	
	double
	toCm() const;
	
	double
	toPt() const;
	
	QString
	toString() const;

private:
	Length();
	
	const char*
	MeasureToString() const;
	
	static Unit
	MeasureFromString(const QString &s);
	
	double size_ = -1.0;
	Unit measure_ = Unit::None;
};

} // ods::
