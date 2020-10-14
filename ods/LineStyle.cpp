#include "LineStyle.hpp"

namespace ods { // ods::

static const auto NoneStr = QStringLiteral("none");
static const auto DashStr = QStringLiteral("dash");

// fo:border="0.99pt dashed #000000"
static const auto DashedStr = QStringLiteral("dashed");
static const auto DotDashStr = QStringLiteral("dot-dash");
static const auto DotDotDashStr = QStringLiteral("dot-dot-dash");
static const auto DottedStr = QStringLiteral("dotted");

// fo:border="0.99pt double #000000"
static const auto DoubleStr = QStringLiteral("double");
static const auto LongDashStr = QStringLiteral("long-dash");
static const auto SolidStr = QStringLiteral("solid");
static const auto WaveStr = QStringLiteral("wave");

LineStyle::LineStyle() {}

LineStyle::LineStyle(const ods::line::Style tus)
: tus_(tus)
{}

LineStyle::~LineStyle() {}

LineStyle*
LineStyle::Clone() const
{
	return new LineStyle(tus_);
}

LineStyle*
LineStyle::FromString(const QString &s)
{
	if (s.isEmpty())
		return nullptr;
	
	if (s == NoneStr)
		return new LineStyle(ods::line::Style::None);
	if (s == DashStr)
		return new LineStyle(ods::line::Style::Dash);
	if (s == DashedStr)
		return new LineStyle(ods::line::Style::Dashed);
	if (s == DotDashStr)
		return new LineStyle(ods::line::Style::DotDash);
	if (s == DotDotDashStr)
		return new LineStyle(ods::line::Style::DotDotDash);
	if (s == DottedStr)
		return new LineStyle(ods::line::Style::Dotted);
	if (s == DoubleStr)
		return new LineStyle(ods::line::Style::Double);
	if (s == LongDashStr)
		return new LineStyle(ods::line::Style::LongDash);
	if (s == SolidStr)
		return new LineStyle(ods::line::Style::Solid);
	if (s == WaveStr)
		return new LineStyle(ods::line::Style::Wave);
	
	mtl_trace();
	return nullptr;
}

QString
LineStyle::toString()
{
	switch(tus_)
	{
	case ods::line::Style::None: return NoneStr;
	case ods::line::Style::Dash: return DashStr;
	case ods::line::Style::Dashed: return DashedStr;
	case ods::line::Style::DotDash: return DotDashStr;
	case ods::line::Style::DotDotDash: return DotDotDashStr;
	case ods::line::Style::Dotted: return DottedStr;
	case ods::line::Style::Double: return DoubleStr;
	case ods::line::Style::LongDash: return LongDashStr;
	case ods::line::Style::Solid: return SolidStr;
	case ods::line::Style::Wave: return WaveStr;
	default: mtl_warn(); return QLatin1String();
	}
}

} // ods::
