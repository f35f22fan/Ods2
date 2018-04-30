#pragma once

#include "decl.hxx"

#include "../decl.hxx"
#include "../err.hpp"
#include "../id.hh"
#include "../ods.hxx"
#include "../style.hxx"

#include "../StringOrInst.hpp"
#include "../StringOrTag.hpp"

#include <cstdint>
#include <QXmlStreamWriter>

namespace ods { // ods::
namespace inst { // ods::inst::

namespace Bits {
const uint16_t Style = 1 << 0;
}

class ODS_API Abstract {
public:
	
	Abstract(Abstract *parent, Ns *ns, ods::id::func f);
	Abstract(const Abstract &cloner);
	virtual ~Abstract();
	
	//==> Style Interface
		virtual QString*
		data_style_name() { return nullptr; }
		
		Abstract*
		GetStyleRecursive(const QString &name);
		
		bool
		IsStyle() const { return bits_ & Bits::Style; }
		
		virtual QString*
		parent_style_name() { return nullptr; }
		
		virtual QString*
		style_name() { return nullptr; }
	//<== Style Interface
	
	bool
	AddText(ods::StringOrTag *tot);
	
	void
	Append(Abstract *a);
	
	void
	Append(const QString &s);
	
	uint16_t&
	bits() { return bits_; }
	
	ods::Book*
	book() const { return book_; }
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const = 0;
	
	void
	DeleteNodes();
	
	QString
	FullName() const;
	
	id::func
	func() const { return func_; }
	
	void
	func(const id::func f) { func_ = f; }
	
	Abstract*
	Get(const Id id) const;
	
	inst::StyleStyle*
	Get(const QString &style_name) const;
	
	inst::Abstract*
	GetAnyStyle(const QString &style_name) const;
	
	virtual StyleTableCellProperties*
	GetStyleTableCellProperties(const AddIfNeeded ain);
	
	virtual StyleTextProperties*
	GetStyleTextProperties(const AddIfNeeded ain);
	
	ods::Id
	id() const { return id_; }
	
	void
	id(const ods::Id n) { id_ = n; }
	
	bool
	Is(const Id id1, const Id id2 = ods::Id::None) const;
	
	bool
	is_root() const { return parent_ == nullptr; }
	
	bool
	IsTextP() const;
	
	const QVector<StringOrInst*>&
	nodes() const { return nodes_; }
	
	QVector<StringOrInst*>&
	nodes_mut() { return nodes_; }
	
	ods::Ns*
	ns() const { return ns_; }
	
	void
	ns(ods::Ns *p) { ns_ = p; }
	
	inst::Abstract*
	parent() const { return parent_; }
	
	void
	parent(inst::Abstract *p) { parent_ = p; }
	
	ods::Prefix*
	prefix() const { return prefix_; }
	
	void
	prefix(ods::Prefix *p) { prefix_ = p; }
	
	void
	ScanString(Tag *tag);
	
	const QString&
	tag_name() const { return tag_name_; }
	
	void
	tag_name(const QString &n) { tag_name_ = n; }
	
	void
	Write(QXmlStreamWriter &xml);
	
	virtual void
	WriteData(QXmlStreamWriter &xml) = 0;
	
	void
	WriteNodes(QXmlStreamWriter &xml);
	
protected:
	
	void
	Write(QXmlStreamWriter &xml, ods::Prefix *prefix, const char *name,
		const QString &value);
	
	void
	Write(QXmlStreamWriter &xml, QString &str);
	
	void
	Write(QXmlStreamWriter &xml, Prefix *prefix, const char *name,
		const int num, const int except);
	
	void
	Write(QXmlStreamWriter &xml, Abstract *a);
	
	ods::Id id_ = ods::Id::None;
	ods::id::func func_;
	ods::Ns *ns_ = nullptr;
	inst::Abstract *parent_ = nullptr;
	ods::Prefix *prefix_ = nullptr;
	QString tag_name_;
	QVector<StringOrInst*> nodes_;
	ods::Book *book_ = nullptr;
	uint16_t bits_ = 0;
};

}} // ods::inst::

