#pragma once

#include "decl.hxx"

#include "../ByteArray.hpp"
#include "../decl.hxx"
#include "../err.hpp"
#include "../id.hh"
#include "../ndff/decl.hxx"
#include "../ods.hxx"
#include "../Prefix.hpp"
#include "../style.hxx"

#include "../StringOrInst.hpp"
#include "../StringOrTag.hpp"

#include <cstdint>
#include <QFile>
#include <QHash>
#include <QXmlStreamWriter>

namespace ods {
using NdffAttrs = QHash<UriId, QVector<ndff::Property>>;
}

namespace ods::inst {

enum class IncludingText: i8 {
	Yes,
	No
};

enum class LimitTo: i8 {
	Used,
	All
};

enum class ClonePart: u8 {
	Text = 1,
	Class = 1 << 1,
};

inline ClonePart operator | (ClonePart a, ClonePart b)
{
	return static_cast<ClonePart>(a | b);
}

inline bool operator & (ClonePart a, ClonePart b) {
	return a & b;
}

inline void Add(const Prefix *prefix, NsHash &list)
{
	if (!list.contains(prefix->id()))
		list.insert(prefix->id(), prefix->uri());
}

void AddKeywords(const QVector<QString> &words, Keywords &words_hash);

class ODS_API Abstract {
public:
	using Bits = u16;
	static const Bits StyleBit              = 1 << 0;
	static const Bits ChangedPropertiesBit  = 1 << 1;
	static const Bits ChangedSubnodesBit    = 1 << 2;
	static const Bits InsideForeignTagBit   = 1 << 3;
	static const Bits InitOkBit             = 1 << 4;
	
	Abstract(Abstract *parent, Ns *ns, ods::id::func f);
	Abstract(const Abstract &cloner);
	virtual ~Abstract();
	
	//==> Style Interface
		virtual QString* data_style_name() { return nullptr; }
		Abstract* GetStyleRecursive(const QString &name);
		bool IsStyle() const { return bits_ & StyleBit; }
		virtual QString* parent_style_name() { return nullptr; }
		virtual QString* style_name() { return nullptr; }
	//<== Style Interface
	
	bool AddText(ods::StringOrTag *sot);
	void Append(Abstract *a, const TakeOwnership to);
	void Append(QStringView s);
	
	Bits& bits() { return bits_; }
	ods::Book* book() const { return book_; }
	bool changed() const;
	bool changed_properties() const {
		return (bits_ & ChangedPropertiesBit) || (loc_within_file_ == -1); }
	bool changed_subnodes() const {
		return (bits_ & ChangedSubnodesBit) || (loc_within_file_ == -1);
	}
	
	bool CheckChanged(const Recursively r);
	virtual Abstract* Clone(Abstract *parent = nullptr) const = 0;
	
	void CopyAttr(NdffAttrs &attrs, Prefix *prefix, QStringView attr_name, QString &result);
	void CopyAttr(NdffAttrs &attrs, ods::Prefix *prefix, QStringView attr_name, ods::Bool &result);
	void CopyAttr(NdffAttrs &attrs, ods::Prefix *prefix, QStringView attr_name, ods::Length **result);
	void CopyAttrI8(NdffAttrs &attrs, Prefix *prefix, QStringView attr_name, i8 &result);
	void CopyAttrI32(NdffAttrs &attrs, Prefix *prefix, QStringView attr_name, i32 &result);
	
	void DeleteNodes();
	virtual QString FullName() const;
	id::func func() const { return func_; }
	void func(const id::func f) { func_ = f; }
	Abstract* Get(const Id id) const;
	inst::StyleStyle* Get(const QString &style_name) const;
	inst::Abstract* GetAnyStyle(const QString &style_name) const;
	const QString* GetString() const;
	
	/* If you only need to know if it has children use this method
	(because it's much faster) instead of calling ListChildren(..)
	and then calling size() on the vector */
	virtual bool has_children(const IncludingText itx = IncludingText::Yes) const;
	
	ods::Id id() const { return id_; }
	void id(const ods::Id n) { id_ = n; }
	
	bool Is(const Id id1, const Id id2 = ods::Id::None) const;
	bool is_root() const { return parent_ == nullptr; }
	bool IsTextP() const { return id_ == Id::TextP; }
	
	virtual void ListChildren(QVector<StringOrInst*> &output,
		const Recursively r = Recursively::No);
	
/** Lists the XML namespaces it currently uses but not
	the namespaces of its children, for the latter one has
	to call its children separately.
	This method is used to add to the document only the
	XML namespaces that are actually used. */
	virtual void ListUsedNamespaces(NsHash &list) = 0;
	
	virtual void ListKeywords(Keywords &list, const LimitTo lt) = 0;
	
	QVector<StringOrInst*>* nodes() { return &nodes_; }
	
	ods::Ns* ns() const { return ns_; }
	
	void ns(ods::Ns *p) { ns_ = p; }
	
	inst::Abstract* parent() const { return parent_; }
	void parent(inst::Abstract *p) { parent_ = p; }
	
	ods::Prefix* prefix() const { return prefix_; }
	void prefix(ods::Prefix *p) { prefix_ = p; }
	
	void ReadStrings(ndff::Container *cntr);
	void ReadStrings(ndff::Container *cntr, ndff::Op op);
	void ReadStrings(Tag *tag);
	
	void SetString(QStringView s, const ClearTheRest ctr = ClearTheRest::Yes);
	
	const QString& tag_name() const { return tag_name_; }
	void tag_name(const QString &n) { tag_name_ = n; }
	
	void Write(QXmlStreamWriter &xml);
	virtual void WriteData(QXmlStreamWriter &xml) = 0;
	virtual void WriteNDFF(NsHash &h, Keywords &kw, QFileDevice *file, ByteArray *output);
	
	void WriteNdffProp(inst::Keywords &kw, ByteArray &output,
		Prefix *prefix, QString key, QStringView value);
	
	void WriteNdffProp(inst::Keywords &kw, ByteArray &ba,
		ods::Prefix *prefix, QString name, const ods::Bool value);
	
	void WriteNdffProp(inst::Keywords &kw, ByteArray &ba,
		ods::Prefix *prefix, QString name, const ods::Length *value);
	
	void WriteNdffProp(inst::Keywords &kw, ByteArray &ba,
		Prefix *prefix, QString name, cint num, cint except);
	
	void WriteNodes(QXmlStreamWriter &xml);
	
protected:
	
	void changed_properties(const bool b) {
		if (b)
			bits_ |= ChangedPropertiesBit;
		else
			bits_ &= ~ChangedPropertiesBit;
	}
	
	void changed_subnodes(const bool b) {
		if (b)
			bits_ |= ChangedSubnodesBit;
		else
			bits_ &= ~ChangedSubnodesBit;
	}
	
	void CloneChildrenOf(const Abstract *rhs,
		const ClonePart co = ClonePart::Text | ClonePart::Class);

	void Write(QXmlStreamWriter &xml, ods::Prefix *prefix, QStringView name,
		QStringView value);
	
	void Write(QXmlStreamWriter &xml, ods::Prefix *prefix, QStringView name,
		const ods::Bool value);
	
	void Write(QXmlStreamWriter &xml, ods::Prefix *prefix, QStringView name,
		const ods::Length *value);
	
	void Write(QXmlStreamWriter &xml, QStringView str);
	
	void Write(QXmlStreamWriter &xml, Abstract *a);
	
	void Write(QXmlStreamWriter &xml, Prefix *prefix, QStringView name,
		const int num, const int except);
	
	inline void WriteContentFollows(ByteArray &ba) {
		ba.add_u8(ndff::Op::TCF_CMS);
	}
	void WriteNodes(NsHash &h, Keywords &kw, ByteArray &ba, const PrintText pt = PrintText::No);
	inline void WriteSCT(ByteArray &ba) {
		ba.add_u8(ndff::Op::SCT);
	}
    void WriteTag(Keywords &kw, ByteArray &ba);
	
	inline void CloseBasedOnChildren(NsHash &h, Keywords &kw, QFileDevice *file, ByteArray *ba)
	{
		if (has_children(IncludingText::Yes))
			CloseWriteNodesAndClose(h, kw, file, ba);
		else
			CloseTag(file, ba);
	}
	
	inline void CloseTag(QFileDevice *file, ByteArray *ba)
	{
		ba->add_u8(ndff::Op::N32_TE);
	}
	
	inline void CloseWriteNodesAndClose(NsHash &h, Keywords &kw, QFileDevice *file,
		ByteArray *ba, const PrintText pt = PrintText::No)
	{
		MTL_CHECK_VOID(ba != nullptr);
		WriteContentFollows(*ba);
		WriteNodes(h, kw, *ba, pt);
		WriteSCT(*ba);
	}
	
	ods::id::func func_;
	ods::Ns *ns_ = nullptr;
	Abstract *parent_ = nullptr;
	ods::Prefix *prefix_ = nullptr;
	QString tag_name_;
	QVector<StringOrInst*> nodes_;
	ods::Book *book_ = nullptr;
	i64 loc_within_file_ = -1;
	Bits bits_ = 0;
	ods::Id id_ = ods::Id::None;
};

} // ods::inst::

