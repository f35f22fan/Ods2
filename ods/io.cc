#include "io.hh"

#include "AutoDelete.hh"
#include "ByteArray.hpp"

#include <QDir>
#include <QFileInfo>
#include <QProcessEnvironment>
#include <QRegularExpression>

#include <algorithm>
#include <cmath>
//#if defined(_MSC_VER)
//#include <__msvc_all_public_headers.hpp>
//#else
//#include <bits/stdc++.h> /// std::sort()
//#endif
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __linux__
#include <unistd.h>
#include <sys/xattr.h>
#include <zstd.h>
#endif

namespace ods::io {

QString
GetParentDirPath(QStringView full_path)
{
	const auto fp = full_path.toString();
	int at = fp.size() - 1;
	
	while ((at > 0) && (fp.at(at) == '/')) {
		at--;
	}
	
	at--;
	
	while ((at >= 0) && (fp.at(at) != '/')) {
		at--;
	}
	
	if (at < 0) {
		return QString();
	}
	
	if (at == 0)
		at = 1;
	
	return fp.mid(0, at);
}

#ifdef __linux__
bool ReadFile(QString full_path, ByteArray &buffer, ReadParams params)
{
	struct statx stx;
	cauto path = full_path.toLocal8Bit();
	cauto flags = 0;// this function must follow symlinks
	cauto fields = STATX_MODE | STATX_SIZE;
	bool statx_ok;
	if (statx(0, path.data(), flags, fields, &stx) != 0)
	{
		if (params.print_errors == PrintErrors::Yes)
			mtl_warn("%s: \"%s\"", strerror(errno), path.data());
		if (params.ret_mode != nullptr)
			*(params.ret_mode) = 0;
		
		statx_ok = false;
	} else {
		statx_ok = true;
		if (params.ret_mode != nullptr)
			*(params.ret_mode) = stx.stx_mode;
	}
	
	cint fd = ::open(path.data(), O_RDONLY);
	if (fd == -1)
		return false;
	
	cauto at = buffer.at();
	ExactSize es;
	if (statx_ok && params.can_rely == CanRelyOnStatxSize::Yes)
	{
		es = ExactSize::Yes;
		buffer.MakeSure(stx.stx_size, ExactSize::Yes);
	} else {
		es = ExactSize::No;
	}
	
	i64 so_far = 0;
	cisize buf_size = 4096 * 4;
	char *buf = new char[buf_size];
	AutoDeleteArr buf_(buf);
	while (true)
	{
		ci64 actually_read = read(fd, buf, buf_size);
		if (actually_read == -1)
		{
			if (errno == EAGAIN)
				continue;
			if (params.print_errors == PrintErrors::Yes)
				mtl_warn("ReadFile: %s", strerror(errno));
			close(fd);
			return false;
		} else if (actually_read == 0) {
			/// Zero indicates the end of file, happens with sysfs files.
			break;
		}
		
		so_far += actually_read;
		
		if (params.read_max != -1 && so_far >= params.read_max)
		{
			so_far -= actually_read;
			break;
		}
		
		buffer.add(buf, actually_read, es);
	}
	
	close(fd);
	buffer.to(at);
	buffer.size(at + so_far); /// needed for buffer.toString()
	
	return true;
}

#else

bool ReadFile(QString full_path, ByteArray& buf, ReadParams params) {
	char * buffer;
	size_t result;
	
	auto fp_ba = full_path.toLocal8Bit();
	FILE* pFile = fopen (fp_ba.data(), "rb" );
	if (pFile==NULL) {
		fputs ("File error",stderr);
		return false;
	}
	
	// obtain file size:
	fseek (pFile , 0 , SEEK_END);
	ci64 lSize = ftell (pFile);
	rewind (pFile);
	
	// allocate memory to contain the whole file:
	buffer = (char*) malloc (sizeof(char)*lSize);
	if (buffer == NULL) {
		fputs ("Memory error",stderr);
		return false;
	}
	
	// copy the file into the buffer:
	result = fread (buffer,1,lSize,pFile);
	if (result != lSize) {
		fputs ("Reading error",stderr);
		return false;
	}
	
	/* the whole file is now loaded in the memory buffer. */
	
	// terminate
	fclose (pFile);
	buf.TakeOver(buffer, lSize, lSize);
	
	return true;
}
#endif

i64 ReadToBuf(cint fd, char *buf, ci64 buf_size,
	const PrintErrors pe)
{
	i64 so_far = 0;
#ifdef __linux__
	while (true)
	{
//mtl_info("Starting ::read()");
		const isize chunk = ::read(fd, buf + so_far, buf_size - so_far);
//mtl_info("Read %ld", chunk);
		if (chunk == -1)
		{
			if (errno == EAGAIN)
				continue;
			if (pe == PrintErrors::Yes)
				mtl_warn("%s", strerror(errno));
			return -1;
		} else if (chunk == 0) {
			/// Zero indicates the end of file, happens with sysfs files.
			break;
		}
		
		so_far += chunk;
	}
#endif
	return so_far;
	
}

bool RemoveXAttr(QStringView full_path, QStringView xattr_name, const PrintErrors pe)
{
	bool ok = false;
#ifdef __linux__
	auto file_path_ba = full_path.toLocal8Bit();
	auto xattr_name_ba = xattr_name.toLocal8Bit();
	ok = lremovexattr(file_path_ba.data(), xattr_name_ba.data()) == 0;
	
	if (!ok && (pe == PrintErrors::Yes))
		mtl_warn("lremovexattr on %s: %s", xattr_name_ba.data(), strerror(errno));
#endif
	return ok;
}

bool SetXAttr(const QString &full_path, const QString &xattr_name,
	const ByteArray &ba, const PrintErrors pe)
{
	bool ok = false;
#ifdef __linux__
	auto file_path_ba = full_path.toLocal8Bit();
	auto xattr_name_ba = xattr_name.toLocal8Bit();
	ok = lsetxattr(file_path_ba.data(), xattr_name_ba.data(),
		ba.data(), ba.size(), 0) == 0;
	
	if (!ok && pe == PrintErrors::Yes)
	{
		mtl_warn("lsetxattr on %s: %s, FILE: %s", xattr_name_ba.data(),
			strerror(errno), qPrintable(full_path));
	}
#endif
	return ok;
}

#ifdef __linux__
bool WriteToFile(QStringView full_path, const char *data, ci64 size)
{
	auto path = full_path.toLocal8Bit();
	cint fd = open(path.data(), O_LARGEFILE | O_WRONLY | O_CREAT | O_TRUNC,
		//(custom_mode == nullptr) ? io::FilePermissions : *custom_mode);
		io::FilePermissions);
	if (fd == -1)
	{
		mtl_status(errno);
		mtl_info("%s", path.data());
		return false;
	}
	
	i64 written = 0;
	i64 ret;
	
	while (written < size) {
		// ssize_t write(int fd, const void *buf, size_t count);
		ret = write(fd, data + written, size - written);
		
		if (ret == -1) {
			if (errno == EAGAIN)
				continue;
			mtl_status(errno);
			mtl_info("%s", path.data());
			close(fd);
			return false;
		}
		
		written += ret;
	}
	
	// switch (post_write) {
	// case PostWrite::DoNothing: break;
	// case PostWrite::FSync: fsync(fd); break;
	// case PostWrite::FDataSync: fdatasync(fd); break;
	// }
	
	close(fd);
	
	return true;
}
#else
bool WriteToFile(QStringView full_path, const char* data, ci64 size) {
//static int use_data(void *data, size_t size, const char *archive) {
	auto name_ba = full_path.toLocal8Bit();
	const char* archive = name_ba.data();
	/* example implementation that writes data to file */
	FILE *fp;
	
	if (data == NULL) {
		if (remove(archive) < 0 && errno != ENOENT) {
			fprintf(stderr, "can't remove %s: %s\n", archive, strerror(errno));
			return false;
		}
		return true;
	}
	
	if ((fp = fopen(archive, "wb")) == NULL) {
		fprintf(stderr, "can't open %s: %s\n", archive, strerror(errno));
		return false;
	}
	if (fwrite(data, 1, size, fp) < size) {
		fprintf(stderr, "can't write %s: %s\n", archive, strerror(errno));
		fclose(fp);
		return false;
	}
	if (fclose(fp) < 0) {
		fprintf(stderr, "can't write %s: %s\n", archive, strerror(errno));
		return false;
	}
	
	return true;
}
#endif

} // ods::io::


