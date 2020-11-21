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
	
	QVector<Prefix*> v_;
};

} // ods::
