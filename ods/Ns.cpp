#include "Ns.hpp"

#include "ns.hxx"
#include "Prefix.hpp"

#include "inst/Abstract.hpp"
#include "ndff/Container.hpp"

#include <algorithm>

namespace ods { // ods::

bool SortPrefixes(Prefix *a, Prefix *b)
{
	if (a->id() == b->id())
		return false;
	
	return a->id() < b->id();
}

Ns::Ns() {}
Ns::~Ns() {
	DeleteData();
}

Ns* Ns::Default()
{
	Ns *ns = new Ns();
	ns->InitDefault(WillInitFromData::No);
	return ns;
}

void Ns::DeleteData()
{
	delete anim_;
	anim_ = 0;
	delete calcext_;
	calcext_ = 0;
	delete chart_;
	chart_ = 0;
	delete config_;
	config_ = 0;
	delete db_;
	db_ = 0;
	delete dc_;
	dc_ = 0;
	delete draw_;
	draw_ = 0;
	delete fo_;
	fo_ = 0;
	delete loext_;
	loext_ = 0;
	delete manifest_;
	manifest_ = 0;
	delete math_;
	math_ = 0;
	delete meta_;
	meta_ = 0;
	delete number_;
	number_ = 0;
	delete of_;
	of_ = 0;
	delete office_;
	office_ = 0;
	delete presentation_;
	presentation_ = 0;
	delete script_;
	script_ = 0;
	delete smil_;
	smil_ = 0;
	delete style_;
	style_ = 0;
	delete svg_;
	svg_ = 0;
	delete table_;
	table_ = 0;
	delete text_;
	text_ = 0;
	delete xlink_;
	xlink_ = 0;
}

Ns* Ns::FromXml(QXmlStreamReader &xml, ci32 file_index)
{
	Ns *ns = new Ns();
	ns->InitDefault(WillInitFromData::Yes);
	ns->SyncWith(xml, file_index);
	return ns;
}

Ns* Ns::FromNDFF(ndff::Container *ndff)
{
	Ns *ns = new Ns();
	ns->InitDefault(WillInitFromData::Yes);
	ns->SyncWith(ndff);
	
	return ns;
}

ods::Prefix*
Ns::GetPrefix(QStringView s)
{
	for (Prefix *x: prefixes_)
	{
		if (x->Is(s))
			return x;
	}
	
	return nullptr;
}

void Ns::InitDefault(const WillInitFromData atr)
{
	anim_ = Prefix::Create(uri_ids_.Animation, QLatin1String("anim"),
		QLatin1String("urn:oasis:names:tc:opendocument:xmlns:animation:1.0"));
	chart_ = Prefix::Create(uri_ids_.Chart, QLatin1String("chart"),
		QLatin1String("urn:oasis:names:tc:opendocument:xmlns:chart:1.0"));
	config_ = Prefix::Create(uri_ids_.Config, QLatin1String("config"),
		QLatin1String("urn:oasis:names:tc:opendocument:xmlns:config:1.0"));
	calcext_ = Prefix::Create(uri_ids_.Calcext, QLatin1String("calcext"),
		QLatin1String("urn:org:documentfoundation:names:experimental:calc:xmlns:calcext:1.0"));
	db_ = Prefix::Create(uri_ids_.Database, QLatin1String("db"),
		QLatin1String("urn:oasis:names:tc:opendocument:xmlns:database:1.0"));
	dc_ = Prefix::Create(uri_ids_.Dc, QLatin1String("dc"),
		QLatin1String("http://purl.org/dc/elements/1.1/"));
	draw_ = Prefix::Create(uri_ids_.Draw, QLatin1String("draw"),
		QLatin1String("urn:oasis:names:tc:opendocument:xmlns:drawing:1.0"));
	fo_ = Prefix::Create(uri_ids_.Fo, QLatin1String("fo"),
		QLatin1String("urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0"));
	form_ = Prefix::Create(uri_ids_.Form, QLatin1String("form"),
		QLatin1String("urn:oasis:names:tc:opendocument:xmlns:form:1.0"));
	loext_ = Prefix::Create(uri_ids_.Loext, QLatin1String("loext"),
		QLatin1String("urn:org:documentfoundation:names:experimental:office:xmlns:loext:1.0"));
	manifest_ = Prefix::Create(uri_ids_.Manifest, QLatin1String("manifest"),
		QLatin1String("urn:oasis:names:tc:opendocument:xmlns:manifest:1.0"));
	math_ = Prefix::Create(uri_ids_.Math, QLatin1String("math"),
		QLatin1String("http://www.w3.org/1998/Math/MathML"));
	meta_ = Prefix::Create(uri_ids_.Meta, QLatin1String("meta"),
		QLatin1String("urn:oasis:names:tc:opendocument:xmlns:meta:1.0"));
	number_ = Prefix::Create(uri_ids_.Number, QLatin1String("number"),
		QLatin1String("urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0"));
	of_ = Prefix::Create(uri_ids_.Of, QLatin1String("of"),
		QLatin1String("urn:oasis:names:tc:opendocument:xmlns:of:1.2"));
	office_ = Prefix::Create(uri_ids_.Office, QLatin1String("office"),
		QLatin1String("urn:oasis:names:tc:opendocument:xmlns:office:1.0"));
	presentation_ = Prefix::Create(uri_ids_.Presentation, QLatin1String("presentation"),
		QLatin1String("urn:oasis:names:tc:opendocument:xmlns:presentation:1.0"));
	script_ = Prefix::Create(uri_ids_.Script, QLatin1String("script"),
		QLatin1String("urn:oasis:names:tc:opendocument:xmlns:script:1.0"));
	smil_ = Prefix::Create(uri_ids_.Smil, QLatin1String("smil"),
		QLatin1String("urn:oasis:names:tc:opendocument:xmlns:smil-compatible:1.0"));
	style_ = Prefix::Create(uri_ids_.Style, QLatin1String("style"),
		QLatin1String("urn:oasis:names:tc:opendocument:xmlns:style:1.0"));
	svg_ = Prefix::Create(uri_ids_.Svg, QLatin1String("svg"),
		QLatin1String("urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0"));
	table_ = Prefix::Create(uri_ids_.Table, QLatin1String("table"),
		QLatin1String("urn:oasis:names:tc:opendocument:xmlns:table:1.0"));
	text_ = Prefix::Create(uri_ids_.Text, QLatin1String("text"),
		QLatin1String("urn:oasis:names:tc:opendocument:xmlns:text:1.0"));
	xlink_ = Prefix::Create(uri_ids_.Xlink, QLatin1String("xlink"),
		QLatin1String("http://www.w3.org/1999/xlink"));
	
	// optimization: adds most commonly used ones first
	prefixes_.append(table_);
	prefixes_.append(style_);
	prefixes_.append(office_);
	prefixes_.append(number_);
	prefixes_.append(text_);
	prefixes_.append(calcext_);
	prefixes_.append(anim_);
	prefixes_.append(chart_);
	prefixes_.append(config_);
	prefixes_.append(db_);
	prefixes_.append(dc_);
	prefixes_.append(draw_);
	prefixes_.append(fo_);
	prefixes_.append(form_);
	prefixes_.append(loext_);
	prefixes_.append(manifest_);
	prefixes_.append(math_);
	prefixes_.append(meta_);
	prefixes_.append(of_);
	prefixes_.append(presentation_);
	prefixes_.append(script_);
	prefixes_.append(smil_);
	prefixes_.append(svg_);
	prefixes_.append(xlink_);
	
	if (atr == WillInitFromData::Yes)
	{
		for (Prefix *prefix: prefixes_)
		{
			prefix->set_id(UriIds::None);
		}
	}
}

void Ns::SyncWith(ndff::Container *ptr)
{
	ndff_ = ptr;
	UriId largest = 0;
	auto &ns_hash = ndff_->ns_hash();
	QString base_name = QLatin1String("ns");
	for (auto it = ns_hash.constBegin(); it != ns_hash.constEnd(); it++)
	{
		cauto decl_uri = it.value();
		for (Prefix *prefix: prefixes_)
		{
	// Almost all URIs start with the same chars and only differ at
	// the end - thus an optimization is to compare them from the end:
			if (prefix->uri().endsWith(decl_uri))
			{
				cauto uri_id = it.key();
				prefix->set_name(base_name + QString::number(uri_id));
				prefix->set_id(uri_id);
				
				if (largest < uri_id)
					largest = uri_id;
				
				break;
			}
		}
	}
}

void Ns::SyncWith(QXmlStreamReader &xml, ci32 file_index)
{
	file_index_ = file_index;
	const auto decls = xml.namespaceDeclarations();
	cint count = decls.size();
	int largest = 0;
	for (int i = 0; i < count; i++)
	{
		const auto &decl = decls[i];
		cauto decl_uri = decl.namespaceUri();
		for (Prefix *prefix: prefixes_)
		{
// Almost all URIs start with the same chars and only differ at
// the end - thus an optimization is to compare them from the end:
			if (prefix->uri().endsWith(decl_uri))
			{
				prefix->set_name(decl.prefix().toString());
				prefix->set_id(i);
				largest = i;
				break;
			}
		}
	}
	
	std::sort(prefixes_.begin(), prefixes_.end(), SortPrefixes);
	
	// Now apply a proper/unique ID to prefixes not found in this XML but
	// available as part of this class:
	int next = 1;
	for (Prefix *prefix: prefixes_)
	{
		if (prefix->id() == UriIds::None)
		{
			prefix->set_id(largest + next);
			next++;
//			mtl_info("(NEW)%d: %s, largest: %d",
//				prefix->id(), qPrintable(prefix->name()), largest);
		}
	}
	
	auto attrs = xml.attributes();
	auto ref = attrs.value(office()->With(ns::kVersion));
	
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

void Ns::WriteNamespaces(QXmlStreamWriter &xml, inst::Abstract *top)
{
	std::sort(prefixes_.begin(), prefixes_.end(), SortPrefixes);
	for (auto *prefix: prefixes_)
	{
		prefix->Write(xml);
	}
}

} // ods::
