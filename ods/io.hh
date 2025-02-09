#pragma once

#include "decl.hxx"
#include "err.hpp"
#include "types.hxx"
#include "global.hxx"

#include <chrono>
#include <stdio.h>

#ifdef __linux__
#include <dirent.h>
#include <mutex>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>
#include <sys/sysmacros.h>

//#else
#elif _WIN32
// windows code goes here

#endif





#include <QHash>
#include <QMetaType> /// Q_DECLARE_METATYPE()
#include <QMimeData>
#include <QProcessEnvironment>
#include <QVector>
#include <QUrl>

namespace ods::io {

enum class FileType: i8 {
	Regular,
	Dir,
	Symlink,
	BlockDevice,
	CharDevice,
	Pipe,
	Socket,
	Unknown
};

enum class AppendSlash: i8 {
	Yes,
	No,
};

enum class FirstTime: i8 {
	Yes,
	No
};

enum class PrintErrors: i8 {
	Yes,
	No
};

enum class CanRelyOnStatxSize: i8 {
	Yes,
	No
};

enum class LinkToDir: i8 {
	LinkToDir,
	Dir,
	Neither,
	Error
};

struct ReadParams {
	i64 read_max = -1;
#ifdef __linux__
	mode_t *ret_mode = nullptr;
#endif
	PrintErrors print_errors = PrintErrors::Yes;
	CanRelyOnStatxSize can_rely = CanRelyOnStatxSize::No;
};

enum class PostWrite: i8 {
	DoNothing = 0,
	FSync,
	FDataSync
};

#ifdef __linux__
const auto ExecBits = S_IXUSR | S_IXGRP | S_IXOTH;
const mode_t DirPermissions = S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH;
const mode_t FilePermissions = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
#endif

QString GetParentDirPath(QStringView full_path);

ODS_API bool ReadFile(QString filepath, ByteArray &buffer, ReadParams rp = {});

// returns -1 on error or num read bytes
i64 ReadToBuf(cint fd, char *buf, ci64 buf_size,
	const PrintErrors pe = PrintErrors::No);

//void ReadXAttrs(io::File &file, const QByteArray &full_path);

bool RemoveXAttr(QStringView full_path, QStringView xattr_name,
	const PrintErrors pe);

bool WriteToFile(QStringView full_path, const char *data, ci64 size);

} // ods::io::::

//Q_DECLARE_METATYPE(ods::io::FileEvent);
