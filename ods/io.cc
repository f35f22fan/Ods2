#include "io.hh"

#include "AutoDelete.hh"
#include "ByteArray.hpp"

#include <QDir>
#include <QFileInfo>
#include <QProcessEnvironment>
#include <QRegularExpression>

#include <algorithm>
#include <cmath>
#include <bits/stdc++.h> /// std::sort()
#include <sys/xattr.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <zstd.h>

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

bool ReadFile(QStringView full_path, ByteArray &buffer,
	const ReadParams &params)
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

i64 ReadToBuf(cint fd, char *buf, ci64 buf_size,
	const PrintErrors pe)
{
	i64 so_far = 0;
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
				mtl_warn("ReadFile: %s", strerror(errno));
			return -1;
		} else if (chunk == 0) {
			/// Zero indicates the end of file, happens with sysfs files.
			break;
		}
		
		so_far += chunk;
	}
	
	return so_far;
}

/*
void ReadXAttrs(io::File &file, const QByteArray &full_path)
{
	if (!file.can_have_xattr())
		return;

	file.ClearXAttrs();
	QHash<QString, ByteArray> &ext_attrs = file.ext_attrs();
	
	isize buflen = llistxattr(full_path.data(), NULL, 0);
	if (buflen <= 0) {
		return; /// 0 = no attributes, -1 = error
	}
	
	/// Allocate the buffer.
	char *buf = new char[buflen];
	MTL_CHECK_VOID(buf != nullptr);
	
	AutoDeleteArr ad(buf);
	/// Copy the list of attribute keys to the buffer.
	buflen = llistxattr(full_path.data(), buf, buflen);
	MTL_CHECK_VOID(buflen != -1);
	
//	Loop over the list of zero terminated strings with the
//	attribute keys. Use the remaining buffer length to determine
//	the end of the list.
	char *key = buf;
	
	while (buflen > 0)
	{
		/// Determine length of the value.
		isize vallen = lgetxattr(full_path.constData(), key, NULL, 0);
		if (vallen <= 0)
			break;
		
		ByteArray ba;
		ba.alloc(vallen);
		vallen = lgetxattr(full_path.constData(), key, ba.data(), ba.size());
		if (vallen == -1)
		{
			mtl_status(errno);
			break;
		}
		
		ext_attrs.insert(key, ba);
		{
//			auto name = file.name().toLocal8Bit();
//			mtl_info("Ext attr: \"%s\": \"%s\" (%s)", key,
//				qPrintable(ba.toString()), name.data());
		}
		
		/// Forward to next attribute key.
		const isize keylen = strlen(key) + 1;
		buflen -= keylen;
		key += keylen;
	}
} */

bool RemoveXAttr(QStringView full_path, QStringView xattr_name, const PrintErrors pe)
{
	auto file_path_ba = full_path.toLocal8Bit();
	auto xattr_name_ba = xattr_name.toLocal8Bit();
	const bool ok = lremovexattr(file_path_ba.data(), xattr_name_ba.data()) == 0;
	
	if (!ok && (pe == PrintErrors::Yes))
		mtl_warn("lremovexattr on %s: %s", xattr_name_ba.data(), strerror(errno));
	
	return ok;
}

bool SetXAttr(const QString &full_path, const QString &xattr_name,
	const ByteArray &ba, const PrintErrors pe)
{
	auto file_path_ba = full_path.toLocal8Bit();
	auto xattr_name_ba = xattr_name.toLocal8Bit();
	const bool ok = lsetxattr(file_path_ba.data(), xattr_name_ba.data(),
		ba.data(), ba.size(), 0) == 0;
	
	if (!ok && pe == PrintErrors::Yes)
	{
		mtl_warn("lsetxattr on %s: %s, FILE: %s", xattr_name_ba.data(),
			strerror(errno), qPrintable(full_path));
	}
	
	return ok;
}

bool WriteToFile(QStringView full_path, const char *data, ci64 size,
	const PostWrite post_write, mode_t *custom_mode)
{
	auto path = full_path.toLocal8Bit();
	cint fd = open(path.data(), O_LARGEFILE | O_WRONLY | O_CREAT | O_TRUNC,
		(custom_mode == nullptr) ? io::FilePermissions : *custom_mode);
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
	
	switch (post_write) {
	case PostWrite::DoNothing: break;
	case PostWrite::FSync: fsync(fd); break;
	case PostWrite::FDataSync: fdatasync(fd); break;
	}
	
	close(fd);
	
	return true;
}

} // ods::io::


