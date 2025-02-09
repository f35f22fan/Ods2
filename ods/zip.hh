#pragma once
/*
  in-memory.c -- modify zip file in memory
  Copyright (C) 2014-2022 Dieter Baron and Thomas Klausner

  This file is part of libzip, a library to manipulate ZIP archives.
  The authors can be contacted at <info@libzip.org>

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:
  1. Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.
  3. The names of the authors may not be used to endorse or promote
     products derived from this software without specific prior
     written permission.

  THIS SOFTWARE IS PROVIDED BY THE AUTHORS ``AS IS'' AND ANY EXPRESS
  OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
  IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
  IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <zip.h>

#include "decl.hxx"
#include "err.hpp"
#include "global.hxx"
#include "ods.hxx"
#include "ByteArray.hpp"

namespace ods::zip {

struct File {
	QString name;
	i64 size = -1;
	time_t mtime = -1; // modification time
	ods::ByteArray data;
	
	bool isDir() const {
		return size == 0 && name.endsWith('/');
	}
	
	bool validMtime() const {
		return mtime != -1;
	}
	
	bool validSize() const {
		return size != -1;
	}
};

bool AddDirToArchive(zip_t *archive, const char *relative_path_inside_zip);
bool AddFileToArchive(zip_t *archive, QString file_fullpath, const char *relative_path_inside_zip);
bool CloseArchive(zip_t *archive, bool print_error = false);
bool CompressDir(QString dir_to_compress, QString zip_filepath, QString *error_str);
QList<QString> ExtractFiles(QString zip_filepath, QString to_dir, QString *report_error = nullptr);
zip_t* OpenArchive(QString fullpath, int flags = ZIP_CREATE | ZIP_TRUNCATE);
bool ListFiles(QString zip_filepath, QList<ods::zip::File> &list, QString *error_str);
}
