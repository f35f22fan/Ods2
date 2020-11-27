#pragma once

#include "Reference.hpp"
#include "currency.hxx"
#include "decl.hxx"
#include "inst/decl.hxx"
#include "err.hpp"
#include "Function.hpp"
#include "formula.hxx"
#include "global.hxx"
#include "op.hh"
#include "ods.hxx"

#include <QDate>
#include <QDateTime>

namespace ods {

class ODS_API FormulaNode {
public:
	FormulaNode();
	FormulaNode(const FormulaNode &rhs);
	FormulaNode& operator=(const FormulaNode &rhs);
	virtual ~FormulaNode();
	
	void AdoptDefaultValueFrom(const FormulaNode &rhs);
	bool ApplyMinus(); // Op "-" + number "3" => number "-3";
	void Clear();
	FormulaNode* Clone() const;
	bool ConvertFunctionOrAddressToValue();
	bool InterpretAsBoolean() const;
	void PrintError() const;
	
	ods::Reference* as_reference() const { return data_.reference; }
	ods::Function* as_function() const { return data_.function; }
	double as_double() const { return data_.number; }
	double as_any_double() const;
	
	ods::Op as_op() const { return data_.op; }
	ods::Brace as_brace() const { return data_.brace; }
	ods::Time* as_time() const { return data_.time; }
	QDate* as_date() const { return data_.date; }
	QDateTime* as_date_time() const { return data_.date_time; }
	ods::Currency* as_currency() const { return data_.currency; }
	double as_percentage() const { return data_.percentage; }
	bool as_bool() const { return data_.flag; }
	QString* as_string() const { return data_.s; }
	inst::TableNamedRange* as_named_range() const { return data_.named_range; }
	FormulaError *as_error() const { return data_.error; }
	
	// takes ownership of pointer arguments:
	static FormulaNode* Reference(ods::Reference *a);
	static FormulaNode* Function(ods::Function *f);
	static FormulaNode* Double(const double d);
	static FormulaNode* Op(const ods::Op op);
	static FormulaNode* Brace(ods::Brace p);
	static FormulaNode* Empty() { return new FormulaNode(); }
	static FormulaNode* Bool(bool b);
	static FormulaNode* NamedRange(inst::TableNamedRange *p);
	static FormulaNode* Percentage(double d);
	static FormulaNode* Currency(ods::Currency *c);
	static FormulaNode* Date(QDate *date);
	static FormulaNode* DateTime(QDateTime *p);
	static FormulaNode* String(QString *s);
	static FormulaNode* Time(ods::Time *d);
	static FormulaNode* Error(ods::FormError e);
	static FormulaNode* Error(ods::FormError e, const QString &s);
	static FormulaNode* Error(ods::FormulaError *e);
	
	bool is_any_double() const { return type_ == Type::Double ||
		type_ == Type::Currency || type_ == Type::Percentage; }
	bool is_reference() const { return type_ == Type::Reference; }
	bool is_function() const { return type_ == Type::Function; }
	bool is_double() const { return type_ == Type::Double; }
	bool is_none() const { return type_ == Type::None; }
	bool is_empty() const { return is_none(); }
	bool is_op() const { return type_ == Type::Op; }
	bool is_brace() const { return type_ == Type::Brace; }
	bool is_time() const { return type_ == Type::Time; }
	bool is_date() const { return type_ == Type::Date; }
	bool is_date_time() const { return type_ == Type::DateTime; }
	bool is_currency() const { return type_ == Type::Currency; }
	bool is_percentage() const { return type_ == Type::Percentage; }
	bool is_bool() const { return type_ == Type::Bool; }
	bool is_string() const { return type_ == Type::String; }
	bool is_named_range() const { return type_ == Type::NamedRange; }
	bool is_error() const { return type_ == Type::Error; }
	
	bool Operation(const ods::Op op, FormulaNode *rhs);
	bool OperationAmpersand(const ods::Op op, FormulaNode *rhs);
	bool OperationEquality(const ods::Op op, FormulaNode *rhs_node);
	bool OperationPlusMinus(const ods::Op op, FormulaNode *rhs);
	bool OperationMultDivide(const ods::Op op, FormulaNode *rhs);
	
	void SetAddress(ods::Reference *a) {
		Clear();
		data_.reference = a;
		type_ = Type::Reference;
	}
	
	void SetBool(const bool flag) {
		Clear();
		data_.flag = flag;
		type_ = Type::Bool;
	}
	
	void SetCurrency(ods::Currency *c) {
		Clear();
		data_.currency = c;
		type_ = Type::Currency;
	}
	
	void SetDate(QDate *d) {
		Clear();
		data_.date = d;
		type_ = Type::Date;
	}
	
	void SetDateTime(QDateTime *d) {
		Clear();
		data_.date_time = d;
		type_ = Type::DateTime;
	}
	
	void SetDouble(double d) {
		Clear();
		data_.number = d;
		type_ = Type::Double;
	}
	
	void SetNamedRange(inst::TableNamedRange *p) {
		Clear();
		data_.named_range = p;
		type_ = Type::NamedRange;
	}
	
	void SetNone() {
		Clear();
		type_ = Type::None;
	}
	
	void SetPercentage(double d) {
		Clear();
		data_.percentage = d;
		type_ = Type::Percentage;
	}
	
	void SetString(QString *s) {
		Clear();
		data_.s = s;
		type_ = Type::String;
	}
	
	void SetTime(ods::Time *d) {
		Clear();
		data_.time = d;
		type_ = Type::Time;
	}
	
	bool is_cell_range() const {
		return is_reference() && as_reference()->is_cell_range();
	}
	
	QString toString(const ToStringArgs args = ToStringArgs::None) const;
private:
	
	static void DeepCopy(FormulaNode &dest, const FormulaNode &src);
	
// time, date, currency,
// percentage, bool, string
	enum class Type : u8 {
		None,
		Reference,
		Function,
		Op,
		Double,
		Brace,
		Time,
		Date,
		DateTime,
		Currency,
		Percentage,
		Bool,
		String,
		NamedRange,
		Error,
	};
	
	// each data type must be properly copied in DeepCopy(..)
	union Data {
		ods::Reference *reference;
		ods::Function *function;
		double number;
		ods::Op op;
		ods::Brace brace;
		ods::Time *time;
		ods::FormulaError *error;
		QDate *date;
		QDateTime *date_time;
		ods::Currency *currency;
		double percentage;
		bool flag;
		QString *s;
// The named range is owned by ods::Book, only copy its pointer and never destroy!
		inst::TableNamedRange *named_range;
		Data() { memset(this, 0, sizeof(Data)); }
		~Data() {}
	};
	
	Type type_ = Type::None;
	Data data_;
};

}
