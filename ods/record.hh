#pragma once

#include "ods.hxx"

#include <QVector>

namespace ods {
enum class RecordAction: i8 {
	None = 0,
	Create,
	Delete,
	Update,
};

struct RecordAddress {
	i32 sheet = -1;
	i32 tag_index = -1;
};

enum class What: i16 {
	None = 0,
	Cell,
	String,
	Row,
	Style,
	Table
};

class Record {
public:
	Record();
	virtual ~Record();
	
	
private:
	RecordAction action_ = RecordAction::None;
	RecordAddress address_ = {};
	What what_ = What::None;
};

class Records {
public:
	Records();
	virtual ~Records();
	
	void Add(const Record r);
	
private:
	QVector<Record> vec_;
};

}


