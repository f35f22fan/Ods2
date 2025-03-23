#pragma once

#include "../decl.hxx"
#include "../err.hpp"
#include "../global.hxx"
#include "../types.hxx"
#include "../Color.hpp"

namespace ods { // ods::

enum class BreakType: i8 {
	Auto,
	Column,
	Page
};

namespace attr { // ods::attr::

class ODS_API VisualBreak
{
public:
	VisualBreak();
	virtual ~VisualBreak();
	
	static VisualBreak* Before() {
		return new VisualBreak();
	}
	
	static VisualBreak* After() {
		auto *vb = new VisualBreak();
		vb->after(true);
		return vb;
	}
	
	VisualBreak* Clone() const;
	
	bool before() const { return before_; }
	void before(cbool flag) { before_ = flag; }
	
	bool after() const { return !before_; }
	void after(cbool flag) { before_ = !flag; }
	
	BreakType break_type() const { return break_type_; }
	void break_type(BreakType bt) { break_type_ = bt; }
	
	static VisualBreak* FromString(const QString &str);
	
	QString toString() const;
	
private:
	NO_ASSIGN_COPY_MOVE(VisualBreak);
	
	BreakType break_type_ = BreakType::Auto;
	bool before_ = true;
};

}} // ods::attr::
