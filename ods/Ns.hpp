#pragma once

#include "err.hpp"
#include "decl.hxx"
#include "global.hxx"
#include "inst/decl.hxx"

#include <QXmlStreamReader>

namespace ods { // ods::

class ODS_API Ns
{
public:
	Ns();
	virtual ~Ns();
	
	Prefix* anim() { return anim_; }
	Prefix* chart() { return chart_; }
	Prefix* config() { return config_; }
	Prefix* db() { return db_; }
	Prefix* dc() { return dc_; }
	Prefix* draw() { return draw_; }
	Prefix* fo() { return fo_; }
	Prefix* form() { return form_; }
	Prefix* loext() { return loext_; }
	Prefix* manifest() { return manifest_; }
	Prefix* math() { return math_; }
	Prefix* meta() { return meta_; }
	Prefix* number() { return number_; }
	Prefix* of() { return of_; }
	Prefix* office() { return office_; }
	Prefix* presentation() { return presentation_; }
	Prefix* script() { return script_; }
	Prefix* smil() { return smil_; }
	Prefix* style() { return style_; }
	Prefix* svg() { return svg_; }
	Prefix* table() { return table_; }
	Prefix* text() { return text_; }
	Prefix* xlink() { return xlink_; }
	
	ods::Prefix*
	GetPrefix(const QString &s);
	
	void
	Read(QXmlStreamReader &xml);
	
	void
	WriteNamespaces(QXmlStreamWriter &xml, inst::Abstract *top);

private:
	NO_ASSIGN_COPY_MOVE(Ns);
	
	void AddToVec();
	void Default();
	
	Prefix *anim_ = nullptr;
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
	
	QVector<Prefix*> v_;
};

} // ods::
