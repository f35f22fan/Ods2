#pragma once

#include "decl.hxx"
#include "err.hpp"
#include "global.hxx"
#include "id.hh"
#include "ods.hxx"
#include "StringOrTag.hpp"

#include "inst/decl.hxx"

#include <QString>

namespace ods {

class ODS_API StringOrInst
{
public:
	StringOrInst(QStringView s);
	StringOrInst(inst::Abstract *inst, const TakeOwnership to);
	
	virtual ~StringOrInst();
	
	void AppendString(const QString &s);
	inst::Abstract* as_inst() const { return inst_; }
	const QString& as_string() const { return s_; }
	const QString* as_str_ptr() const { return &s_; }
	StringOrInst* Clone() const;
	void DeleteData();
	bool Is(const Id id1, const Id id2 = Id::None) const;
	bool is_inst() const { return inst_ != nullptr; }
	bool is_string() const { return is_string_; }
	void owns_inst(const Owns o) { owns_inst_ = o; }
	void SetInst(inst::Abstract *a, const TakeOwnership to);
	void SetString(QStringView s);
	
private:
	NO_ASSIGN_COPY_MOVE(StringOrInst);
	
	inst::Abstract *inst_ = nullptr;
	QString s_;
	Owns owns_inst_ = Owns::No;
	bool is_string_ = false;
};


} // ods::
