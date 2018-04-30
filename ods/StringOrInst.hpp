#pragma once

#include "decl.hxx"
#include "global.hxx"
#include "id.hh"
#include "ods.hxx"
#include "StringOrTag.hpp"

#include "inst/decl.hxx"

#include <QString>

namespace ods {

enum class StringOrInstType : quint8 {
	None,
	String,
	Inst,
};

class ODS_API StringOrInst
{
public:
	StringOrInst(const QString &s);
	
	StringOrInst(inst::Abstract *inst);
	
	virtual ~StringOrInst();
	void
	AppendString(const QString &s);
	
	inst::Abstract*
	as_inst() const { return (inst::Abstract*) data_; }
	
	QString*
	as_string() const { return (QString*)data_; }
	
	StringOrInst*
	Clone() const;
	
	void
	DeleteData();
	
	bool
	Is(const Id id1, const Id id2 = Id::None) const;
	
	bool
	is_inst() const { return type_ == StringOrInstType::Inst; }
	
	bool
	is_string() const { return type_ == StringOrInstType::String; }
	
	void
	SetString(const QString &s);
	
private:
	void *data_ = nullptr;
	StringOrInstType type_ = StringOrInstType::None;
};


} // ods::