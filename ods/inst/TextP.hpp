#pragma once

#include "Abstract.hpp"
#include "../decl.hxx"
#include "../err.hpp"

namespace ods::inst {

class ODS_API TextP : public Abstract
{
public:
	TextP(Abstract *parent, ods::Tag *tag = 0, ndff::Container *cntr = 0);
	TextP(const TextP &cloner);
	virtual ~TextP();
	
	void
	AppendString(const QString &s);
	
	virtual Abstract*
	Clone(Abstract *parent = nullptr) const override;
	
	const QString *GetFirstString() const;
	
	void ListChildren(QVector<StringOrInst *> &vec, const Recursively r) override {}
	void ListKeywords(Keywords &list, const LimitTo lt) override;
	void ListUsedNamespaces(NsHash &list) override;
	
	// sets first string or appends
	void SetFirstString(const QString &s);
	
	void WriteData(QXmlStreamWriter &xml) override;
	void WriteNDFF(NsHash &h, Keywords &kw, QFileDevice *file, ByteArray *ba) override;

private:
	void Init(ndff::Container *cntr);
	void Init(ods::Tag *tag);
	void Scan(ods::Tag *tag);
};

} // ods::inst::
