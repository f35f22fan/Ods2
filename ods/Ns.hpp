#pragma once

#include "err.hpp"
#include "decl.hxx"
#include "global.hxx"
#include "inst/decl.hxx"
#include "types.hxx"

#include <QMap>
#include <QXmlStreamReader>

namespace ods { // ods::

enum class WillInitFromXml: i8 {
	Yes,
	No
};

using UriId = u8;

struct UriIds
{ // Not using an enum because these values will differ based
	// on the order of namespaces appearance in documents.
	static const UriId None = 255;
	UriId Animation    = 0;
	UriId Calcext      = 1;
	UriId Chart        = 2;
	UriId Config       = 3;
	UriId Database     = 4;
	UriId Dc           = 5;
	UriId Dr3          = 6;
	UriId Draw         = 7;
	UriId Fo           = 8;
	UriId Form         = 9;
	UriId Loext        = 10;
	UriId Manifest     = 11;
	UriId Math         = 12;
	UriId Meta         = 13;
	UriId Number       = 14;
	UriId Of           = 15;
	UriId Office       = 16;
	UriId Presentation = 17;
	UriId Script       = 18;
	UriId Smil         = 19;
	UriId Style        = 20;
	UriId Svg          = 21;
	UriId Text         = 22;
	UriId Table        = 23;
	UriId Xlink        = 24;
};

class ODS_API Ns
{
public:
	virtual ~Ns();
	
	static Ns* Default();
	static Ns* FromXml(QXmlStreamReader &xml, ci32 file_index);
	
	Prefix* anim() const { return anim_; }
	Prefix* calcext() const { return calcext_; }
	Prefix* chart() const { return chart_; }
	Prefix* config() const { return config_; }
	Prefix* db() const { return db_; }
	Prefix* dc() const { return dc_; }
	Prefix* draw() const { return draw_; }
	Prefix* fo() const { return fo_; }
	Prefix* form() const { return form_; }
	Prefix* loext() const { return loext_; }
	Prefix* manifest() const { return manifest_; }
	Prefix* math() const { return math_; }
	Prefix* meta() const { return meta_; }
	Prefix* number() const { return number_; }
	Prefix* of() const { return of_; }
	Prefix* office() const { return office_; }
	Prefix* presentation() const { return presentation_; }
	Prefix* script() const { return script_; }
	Prefix* smil() const { return smil_; }
	Prefix* style() const { return style_; }
	Prefix* svg() const { return svg_; }
	Prefix* table() const { return table_; }
	Prefix* text() const { return text_; }
	Prefix* xlink() const { return xlink_; }
	
	const UriIds& ids() const { return uri_ids_; }
	
	i32 file_index() const { return file_index_; }
	ods::Prefix* GetPrefix(QStringView s);
	void Read(QXmlStreamReader &xml, ci32 file_index);
	void WriteNamespaces(QXmlStreamWriter &xml, inst::Abstract *top);

private:
	Ns();
	NO_ASSIGN_COPY_MOVE(Ns);
	
	void InitDefault(const WillInitFromXml atr);
	
	Prefix *anim_ = nullptr;
	Prefix *calcext_ = nullptr;
	Prefix *chart_ = nullptr;
	Prefix *config_ = nullptr;
	Prefix *db_ = nullptr;
	Prefix *dc_ = nullptr;
	Prefix *draw_ = nullptr;
	Prefix *fo_ = nullptr;
	Prefix *form_ = nullptr;
	Prefix *loext_ = nullptr;
	Prefix *manifest_ = nullptr;
	Prefix *math_ = nullptr;
	Prefix *meta_ = nullptr;
	Prefix *number_ = nullptr;
	Prefix *of_ = nullptr;
	Prefix *office_ = nullptr;
	Prefix *presentation_ = nullptr;
	Prefix *script_ = nullptr;
	Prefix *smil_ = nullptr;
	Prefix *style_ = nullptr;
	Prefix *svg_ = nullptr;
	Prefix *table_ = nullptr;
	Prefix *text_ = nullptr;
	Prefix *xlink_ = nullptr;
	QString version_;
	
	QVector<Prefix*> prefixes_;
	i32 file_index_ = -1;
	UriIds uri_ids_ = {};
};

} // ods::
