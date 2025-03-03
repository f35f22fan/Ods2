#pragma once

#include "../ByteArray.hpp"
#include "decl.hxx"
#include "../decl.hxx"
#include "../err.hpp"
#include "../global.hxx"
#include "../ods.hxx"
#include "../inst/Abstract.hpp"
#include "Property.hpp"

namespace ods::ndff {

class Container {
public:
	Container();
	virtual ~Container();
	
	u16 maj_version = 0;
	i16 min_version = -1;
	i64 namespaces_loc = -1;
	i64 dictionary_loc = -1;
	i64 top_files_loc = -1;
	u8 doc_type_len = 0;
	u8 free_space_trailing = 0;
	QString doc_type;
	
	Ns* CreateNs();
	FileEntryInfo* GetTopFile(QStringView filepath) const;
	bool Init(Book *book, QStringView full_path);
	ByteArray &buf() { return buf_; }
	inst::NsHash& ns_hash() { return ns_hash_; }
	inst::Keywords& keywords() { return keywords_; }
	bool GetString(ci32 id, QString &ret_val);
	
	ndff::Op Next(Property &prop, const Op last_op = Op::None,
		QHash<UriId, QVector<Property> > *h = 0,
		CellNote *cn = 0);
	QString NextString();
	bool PrepareFor(FileEntryInfo *fei);
	void PrepareForParsing();
	
	Book* book() const { return book_; }
	Ns* ns() const { return ns_; }
	void SwitchToMainBuf();
	Compression WhatCompressionShouldBeUsed(QStringView file_path, ci64 uncompressed_size) const;
	
private:
	void PrintKeywords();
	void ReadDictionary(ByteArray &buf, ci64 how_much);
	bool ReadDictionaryRegion();
	void ReadNamespaces(ByteArray &buf, ci64 how_much);
	bool ReadNamespacesRegion();
	bool ReadFiles(ci64 files_loc, QVector<FileEntryInfo *> &vec);
	
	Book *book_ = nullptr;
	Ns *ns_ = nullptr;
	QString full_path_;
	ByteArray buf_;
	ByteArray *helper_buf_ = 0;
	inst::NsHash ns_hash_; // using NsHash = QHash<UriId, QString>;
	inst::Keywords keywords_; // using Keywords = QHash<QString, IdAndCount>;
	QHash<i32, QString> id_keyword_;
	QVector<ndff::FileEntryInfo*> top_files_;
	friend class ods::ndff::FileEntryInfo;
};

}
