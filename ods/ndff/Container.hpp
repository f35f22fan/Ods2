#pragma once

#include "../ByteArray.hpp"
#include "decl.hxx"
#include "../decl.hxx"
#include "../err.hpp"
#include "../global.hxx"
#include "../inst/Abstract.hpp"
#include "../types.hxx"

namespace ods::ndff {

class Container {
public:
	u16 maj_version = 0;
	i16 min_version = -1;
	i64 namespaces_loc = -1;
	i64 dictionary_loc = -1;
	i64 top_files_loc = -1;
	u8 doc_type_len = 0;
	QString doc_type;
	
	FileEntryInfo* GetTopFile(QString filepath) const;
	bool Init(QStringView full_path);
	ByteArray &buf() { return buf_; }
	inst::NsHash& ns_hash() { return ns_hash_; }
	inst::Keywords& keywords() { return keywords_; }
	bool GetKey(ci32 value, QString &ret_val);
	
private:
	bool ReadDictionary();
	bool ReadNamespaces();
	bool ReadTopFiles(ci64 files_table_loc, QVector<FileEntryInfo *> &vec);
	
	ByteArray buf_;
	inst::NsHash ns_hash_; // using NsHash = QHash<UriId, QString>;
	inst::Keywords keywords_;
	QVector<ndff::FileEntryInfo*> top_files_;
};

}
