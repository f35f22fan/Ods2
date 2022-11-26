#pragma once

#include "decl.hxx"
#include "err.hpp"
#include "types.hxx"

#include <chrono>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <mutex>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>
#include <sys/sysmacros.h>
#include <zstd.h>

#include <QHash>
#include <QMetaType> /// Q_DECLARE_METATYPE()
#include <QMimeData>
#include <QProcessEnvironment>
#include <QStringRef>
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
	mode_t *ret_mode = nullptr;
	PrintErrors print_errors = PrintErrors::Yes;
	CanRelyOnStatxSize can_rely = CanRelyOnStatxSize::No;
};

enum class PostWrite: i8 {
	DoNothing = 0,
	FSync,
	FDataSync
};

const auto ExecBits = S_IXUSR | S_IXGRP | S_IXOTH;
const mode_t DirPermissions = S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH;
const mode_t FilePermissions = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;

bool ReadFile(QStringView full_path, ByteArray &buffer,
	const ReadParams &params);

// returns -1 on error or num read bytes
i64 ReadToBuf(cint fd, char *buf, ci64 buf_size,
	const PrintErrors pe = PrintErrors::No);

//void ReadXAttrs(io::File &file, const QByteArray &full_path);

bool RemoveXAttr(QStringView full_path, QStringView xattr_name,
	const PrintErrors pe);

bool WriteToFile(const QString &full_path, const char *data, ci64 size,
	const PostWrite post_write = PostWrite::DoNothing,
	mode_t *custom_mode = nullptr);

} // ods::io::::

//Q_DECLARE_METATYPE(ods::io::FileEvent);
