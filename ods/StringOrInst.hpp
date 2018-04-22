#pragma once

#include "decl.hxx"
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

class StringOrInst
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
	Is(const id::func f1, const id::func f2 = nullptr) const;
	
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