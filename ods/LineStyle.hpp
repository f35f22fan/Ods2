#pragma once

#include "err.hpp"
#include "line.hxx"
#include "global.hxx"

namespace ods { // ods::

class ODS_API LineStyle
{
public:
	LineStyle();
	LineStyle(const ods::line::Style);
	virtual ~LineStyle();
	
	LineStyle*
	Clone() const;
	
	static LineStyle*
	FromString(const QString &s);
	
	bool
	is_none() const { return tus_ == ods::line::Style::None; }
	
	bool
	is_dash() const { return tus_ == ods::line::Style::Dash; }
	
	bool
	is_dashed() const { return tus_ == ods::line::Style::Dashed; }
	
	bool
	is_dot_dash() const { return tus_ == ods::line::Style::DotDash; }
	
	bool
	is_dot_dot_dash() const { return tus_ == ods::line::Style::DotDotDash; }
	
	bool
	is_dotted() const { return tus_ == ods::line::Style::Dotted; }
	
	bool
	is_double() const { return tus_ == ods::line::Style::Double; }
	
	bool
	is_long_dash() const { return tus_ == ods::line::Style::LongDash; }
	
	bool
	is_solid() const { return tus_ == ods::line::Style::Solid; }

	bool
	is_valid() const { return tus_ != ods::line::Style::Invalid; }
	
	bool
	is_wave() const { return tus_ == ods::line::Style::Wave; }
	
	void
	Set(const ods::line::Style tus) { tus_ = tus; }
	
	void
	SetNone() { tus_ = ods::line::Style::None; }
	
	void
	SetDash() { tus_ = ods::line::Style::Dash; }
	
	void
	SetDashed() { tus_ = ods::line::Style::Dashed; }
	
	void
	SetDotDash() { tus_ = ods::line::Style::DotDash; }
	
	void
	SetDotDotDash() { tus_ = ods::line::Style::DotDotDash; }
	
	void
	SetDotted() { tus_ = ods::line::Style::Dotted; }
	
	void
	SetDouble() { tus_ = ods::line::Style::Double; }
	
	void
	SetLongDash() { tus_ = ods::line::Style::LongDash; }
	
	void
	SetSolid() { tus_ = ods::line::Style::Solid; }
	
	void
	SetWave() { tus_ = ods::line::Style::Wave; }

	QString
	toString();
	
private:
	
	ods::line::Style tus_ = ods::line::Style::Invalid;
};

} // ods::
