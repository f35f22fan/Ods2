#include "zip.hh"
#include "io.hh"

#include <QDir>
#include <QDateTime>
#include <QFile>

namespace ods::zip {

QList<QString> ExtractFiles(QString zip_filepath, QString to_dir, QString *error_str) {
	QList<QString> list;
	
	if (!to_dir.endsWith('/')) {
		to_dir.append('/');
	}
	
	const QDir basedir = to_dir;
	QList<ods::zip::File> files;
	if (!ListFiles(zip_filepath, files, error_str)) {
		return list;
	}
	
	for (ods::zip::File file: files) {
		const QString fp = basedir.absoluteFilePath(file.name);
		if (file.isDir()) {
			if (!basedir.mkpath(file.name)) {
				if (error_str) {
					error_str->append("Failed to create dir: ").append(file.name);
				}
				return list;
			}
		} else {
			cint index = file.name.lastIndexOf('/');
			if (index != -1) {
				QString subdirs = file.name.mid(0, index);
				if (!basedir.mkpath(subdirs)) {
					if (error_str) {
						error_str->append("Failed to create dir(s): ").append(subdirs);
					}
					return list;
				}
			}
			if (!io::WriteToFile(fp, file.data.data(), file.data.size())) {
				if (error_str) {
					error_str->append("Failed to write to file: ").append(file.name);
				}
				return list;
			}
		}
		
		list.append(fp);
		
		if (file.validMtime()) {
			QFile qfile(fp);
			QDateTime dt;
			ci64 t = file.mtime;
			dt.setMSecsSinceEpoch(t * 1000);
			if (qfile.open(QIODevice::ReadWrite)) {
				if (!qfile.setFileTime(dt, QFileDevice::FileModificationTime)) {
					// Not a critical error though:
					mtl_warn("Failed to set mtime of file %s", qPrintable(fp));
				}
				qfile.close();
			}
		}
	}
	
	return list;
}

bool ListFiles(QString zip_filepath, QList<ods::zip::File> &list, QString *error_str)
{
	QFile file(zip_filepath);
	MTL_CHECK(file.open(QIODevice::ReadOnly));
	QByteArray ba = file.readAll();
	MTL_CHECK(!ba.isEmpty());
	
	void *data = ba.data();
	const size_t size = ba.size();
	
	zip_error_t error;
	zip_error_init(&error);
	zip_source_t *zip_src;
	// create source from buffer
	if ((zip_src = zip_source_buffer_create(data, size, 1, &error)) == NULL) {
		if (error_str) {
			error_str->append("Can't create source: ");
			error_str->append(zip_error_strerror(&error));
		}
		free(data);
		zip_error_fini(&error);
		return false;
	}
	
	// open zip archive from zip_src
	zip_t *za;
	if ((za = zip_open_from_source(zip_src, 0, &error)) == NULL) {
		if (error_str) {
			error_str->append("Can't open zip from source: ");
			error_str->append(zip_error_strerror(&error));
		}
		zip_source_free(zip_src);
		zip_error_fini(&error);
		return false;
	}
	
	zip_error_fini(&error);
	//zip_source_keep(zip_src); // we'll want to read the data back after zip_close
	ci64 num_entries = zip_get_num_entries(za, 0);
	struct zip_stat sb;
	i64 r, len;
	long long sum;
	cu64 max_chunk_len = 1024 * 4;
	char buffer[max_chunk_len];
	list.resize(num_entries);
	
	for (i64 i = 0; i < num_entries; i++) {
		File &file = list[i];
		
		if (zip_stat_index(za, i, 0, &sb) == 0) {
			if (sb.valid & ZIP_STAT_NAME)
				file.name = sb.name;
			file.size = (sb.valid & ZIP_STAT_SIZE) ? sb.size : -1;
			file.mtime = (sb.valid & ZIP_STAT_MTIME) ? sb.mtime : -1;
			// mtl_info("Name: [%s], ", sb.name);
			// mtl_info("Size: [%llu], ", sb.size);
			// mtl_info("Index: [%llu], ", sb.index);
			// mtl_info("Valid: [%llu], ", sb.valid);
			// mtl_info("mtime: [%u], is dir: %s\n", (unsigned int)sb.mtime,
			// 	(file.isDir() ? "true" : "false"));
			
			zip_file *zipfile = zip_fopen_index(za, i, 0);
			if (!zipfile) {
				if (error_str) {
					error_str->append("Failed to open archive entry [");
					error_str->append(sb.name).append("]: ").append(zip_strerror(za));
				}
				zip_close(za);
				continue;
			}
			
			sum = 0;
			while (sum != sb.size) {
				len = zip_fread(zipfile, buffer, max_chunk_len);
				if (len < 0) {
					if (error_str) {
						error_str->append("Failed to read [%s] data: ").append(sb.name);
					}
					return false;
				}
				file.data.add(buffer, len);
				sum += len;
			}
			zip_fclose(zipfile);
		}
	}
	
	return true;
}

bool CloseArchive(zip_t *archive, bool print_error) {
	if (zip_close(archive) == 0)
		return true;
	
	if (print_error)
		mtl_warn("Failed to close archive: %s", zip_strerror(archive));
	return false;
}

bool AddDir(zip_t *archive, QString dir_path, cint skip_from) {
	if (dir_path.size() > skip_from) {
		auto relative_path = dir_path.mid(skip_from).toLocal8Bit();
		if (!AddDirToArchive(archive, relative_path.data())) {
			return false;
		}
	}
	
	QDir dir(dir_path);
	auto infolist = dir.entryInfoList();
	
	for (QFileInfo entry: infolist) {
		QString fn = entry.fileName();
		if (fn == "." || fn == "..") {
			continue;
		}
		const QString fullpath = entry.absoluteFilePath();
		//mtl_info("filepath: %s", qPrintable(fullpath));
		if (entry.isDir()) {
			if (!AddDir(archive, fullpath, skip_from)) {
				return false;
			}
		} else if (entry.isFile()) {
			auto relative_path = fullpath.mid(skip_from).toLocal8Bit();
			if (!AddFileToArchive(archive, fullpath, relative_path.data())) {
				return false;
			}
		} else {
			mtl_warn("%s is not a file or folder", qPrintable(fullpath));
		}
	}
	
	
	return true;
}

bool CompressDir(QString dir_to_compress, QString zip_filepath, QString *error_str)
{
	zip_t *archive = OpenArchive(zip_filepath);
	if (!archive) {
		if (error_str) {
			error_str->append("Failed to open archive: ").append(zip_filepath);
		}
		return false;
	}
	
	dir_to_compress.replace('\\', '/');
	if (!dir_to_compress.endsWith('/'))
		dir_to_compress.append('/');

	cint rel_path_starts_at = dir_to_compress.size();
	if (!AddDir(archive, dir_to_compress, rel_path_starts_at)) {
		if (error_str) {
			error_str->append("Failed to compress: ").append(dir_to_compress);
		}
		CloseArchive(archive);
		return false;
	}
	
	CloseArchive(archive, true);
	
	return true;
}

zip_t* OpenArchive(QString fullpath, int flags) {
	int error = 0;
	auto path_ba = fullpath.toLocal8Bit();
	zip_t *archive = zip_open(path_ba.data(), flags , &error);
	
	if(!archive) {
		mtl_warn("Could not open or create archive %s", qPrintable(fullpath));
		return 0;
	}
	return archive;
}

bool AddDirToArchive(zip_t *archive, const char *relative_path_inside_zip) {
	//mtl_info("Adding Dir: %s", relative_path_inside_zip);
	ci64 index = zip_dir_add(archive, relative_path_inside_zip, ZIP_FL_ENC_UTF_8);
	if (index < 0) {
		mtl_warn("Failed to add folder to archive: %s", zip_strerror(archive));
		return false;
	}
	
	return true;
}

bool AddFileToArchive(zip_t *archive, QString file_fullpath,
	const char *relative_path_inside_zip)
{
	//mtl_info("Adding File: %s", relative_path_inside_zip);
	ods::ByteArray buf;
	MTL_CHECK(ods::io::ReadFile(file_fullpath, buf, {}));
	i64 bufsize;
	char *data = buf.DisownBuffer(bufsize);
	// last param=1 means zip will free it when done:
	zip_source_t *source = zip_source_buffer(archive, data, bufsize, 1);
	if(source == NULL) {
		mtl_warn("Failed to create source buffer: %s ", zip_strerror(archive));
		return false;
	}
	
	cauto flags = ZIP_FL_OVERWRITE | ZIP_FL_ENC_UTF_8;
	ci64 index = zip_file_add(archive, relative_path_inside_zip, source, flags);
	if(index < 0) {
		zip_source_free(source);
		mtl_warn("Failed to add file to archive: %s ", zip_strerror(archive));
		return false;
	}
	
	return true;
}
} // ods::zip
