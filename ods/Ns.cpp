#include "Ns.hpp"

#include "ns.hxx"
#include "Prefix.hpp"

#include "inst/Abstract.hpp"

namespace ods { // ods::

Ns::Ns() {
	Default();
	AddToVec();
}

Ns::~Ns() {}

void
Ns::AddToVec()
{
	// optimization: adds most commonly used ones first
	v_.append(table_);
	v_.append(style_);
	v_.append(office_);
	v_.append(number_);
	v_.append(text_);
	v_.append(calcext_);
	v_.append(anim_);
	v_.append(chart_);
	v_.append(config_);
	v_.append(db_);
	v_.append(dc_);
	v_.append(draw_);
	v_.append(fo_);
	v_.append(form_);
	v_.append(loext_);
	v_.append(manifest_);
	v_.append(math_);
	v_.append(meta_);
	v_.append(of_);
	v_.append(presentation_);
	v_.append(script_);
	v_.append(smil_);
	v_.append(svg_);
	v_.append(xlink_);
}

void
Ns::Default()
{
	anim_ = Prefix::Create( ods::UriId::Animation,
		QLatin1String("anim"),
		QLatin1String("urn:oasis:names:tc:opendocument:xmlns:animation:1.0"));
	chart_ = Prefix::Create(
		ods::UriId::Chart,
		QLatin1String("chart"),
		QLatin1String("urn:oasis:names:tc:opendocument:xmlns:chart:1.0"));
	config_ = Prefix::Create(
		ods::UriId::Config,
		QLatin1String("config"),
		QLatin1String("urn:oasis:names:tc:opendocument:xmlns:config:1.0"));
	
	calcext_ = Prefix::Create(
		ods::UriId::Calcext,
		QLatin1String("calcext"),
		QLatin1String("urn:org:documentfoundation:names:experimental:calc:xmlns:calcext:1.0"));
	
	db_ = Prefix::Create(
		ods::UriId::Database,
		QLatin1String("db"),
		QLatin1String("urn:oasis:names:tc:opendocument:xmlns:database:1.0"));
	dc_ = Prefix::Create(
		ods::UriId::Dc,
		QLatin1String("dc"),
		QLatin1String("http://purl.org/dc/elements/1.1/"));
	draw_ = Prefix::Create(
		ods::UriId::Draw,
		QLatin1String("draw"),
		QLatin1String("urn:oasis:names:tc:opendocument:xmlns:drawing:1.0"));
	fo_ = Prefix::Create(
		ods::UriId::Fo,
		QLatin1String("fo"),
		QLatin1String("urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0"));
	form_ = Prefix::Create(
		ods::UriId::Form,
		QLatin1String("form"),
		QLatin1String("urn:oasis:names:tc:opendocument:xmlns:form:1.0"));
	loext_ = Prefix::Create(
		ods::UriId::Loext,
		QLatin1String("loext"),
		QLatin1String("urn:org:documentfoundation:names:experimental:office:xmlns:loext:1.0"));
	manifest_ = Prefix::Create(
		ods::UriId::Manifest,
		QLatin1String("manifest"),
		QLatin1String("urn:oasis:names:tc:opendocument:xmlns:manifest:1.0"));
	math_ = Prefix::Create(
		ods::UriId::Math,
		QLatin1String("math"),
		QLatin1String("http://www.w3.org/1998/Math/MathML"));
	meta_ = Prefix::Create(
		ods::UriId::Meta,
		QLatin1String("meta"),
		QLatin1String("urn:oasis:names:tc:opendocument:xmlns:meta:1.0"));
	number_ = Prefix::Create(
		ods::UriId::Number,
		QLatin1String("number"),
		QLatin1String("urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0"));
	of_ = Prefix::Create(
		ods::UriId::Of,
		QLatin1String("of"),
		QLatin1String("urn:oasis:names:tc:opendocument:xmlns:of:1.2"));
	office_ = Prefix::Create(
		ods::UriId::Office,
		QLatin1String("office"),
		QLatin1String("urn:oasis:names:tc:opendocument:xmlns:office:1.0"));
	presentation_ = Prefix::Create(
		ods::UriId::Presentation,
		QLatin1String("presentation"),
		QLatin1String("urn:oasis:names:tc:opendocument:xmlns:presentation:1.0"));
	script_ = Prefix::Create(
		ods::UriId::Script,
		QLatin1String("script"),
		QLatin1String("urn:oasis:names:tc:opendocument:xmlns:script:1.0"));
	smil_ = Prefix::Create(
		ods::UriId::Smil,
		QLatin1String("smil"),
		QLatin1String("urn:oasis:names:tc:opendocument:xmlns:smil-compatible:1.0"));
	style_ = Prefix::Create(
		ods::UriId::StyleStyle,
		QLatin1String("style"),
		QLatin1String("urn:oasis:names:tc:opendocument:xmlns:style:1.0"));
	svg_ = Prefix::Create(
		ods::UriId::Svg,
		QLatin1String("svg"),
		QLatin1String("urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0"));
	table_ = Prefix::Create(
		ods::UriId::Table,
		QLatin1String("table"),
		QLatin1String("urn:oasis:names:tc:opendocument:xmlns:table:1.0"));
	text_ = Prefix::Create(
		ods::UriId::Text,
		QLatin1String("text"),
		QLatin1String("urn:oasis:names:tc:opendocument:xmlns:text:1.0"));
	xlink_ = Prefix::Create(
		ods::UriId::Xlink,
		QLatin1String("xlink"),
		QLatin1String("http://www.w3.org/1999/xlink"));
}

ods::Prefix*
Ns::GetPrefix(const QString &s)
{
	for (Prefix *x: v_)
	{
		if (x->Is(s))
			return x;
	}
	
	return nullptr;
}

void
Ns::Read(QXmlStreamReader &xml)
{
	auto decls = xml.namespaceDeclarations();
	
	foreach (auto decl, decls)
	{
		QString prefix = decl.prefix().toString();
		QStringRef uri = decl.namespaceUri();
		
		for (Prefix *x: v_)
		{
			if (x->uri() == uri)
			{
				x->set_str(prefix);
				break;
			}
		}
	}
	
	auto attrs = xml.attributes();
	QStringRef ref = attrs.value(office()->With(ods::ns::kVersion));
	
	if (ref.isEmpty())
	{
		/** From the spec:
		  * When an "office:version"-requiring element has "office:version"
		  * omitted, the element is based on a version of the OpenDocument
		  * specification earlier than v1.2.
		**/
		version_ = QLatin1String("1.0");
	} else {
		version_ = ref.toString();
	}
}

void
Ns::WriteNamespaces(QXmlStreamWriter &xml, inst::Abstract *top)
{
	for (auto *x: v_)
	{
		x->Write(xml);
	}
}

} // ods::
