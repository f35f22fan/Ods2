#include "DrawImage.hpp"

#include "../Book.hpp"
#include "../filename.hxx"
#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

#include "ManifestManifest.hpp"

#include <QImageReader>
#include <QMimeDatabase>

namespace ods { // ods::
namespace inst { // ods::inst::

DrawImage::DrawImage(Abstract *parent, Tag *tag)
: Abstract (parent, parent->ns(), id::DrawImage)
{
	if (tag != nullptr)
		Init(tag);
}

DrawImage::DrawImage(const DrawImage &cloner)
: Abstract(cloner)
{}

DrawImage::~DrawImage() {}

Abstract*
DrawImage::Clone(Abstract *parent) const
{
	auto *p = new DrawImage(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->xlink_href_ = xlink_href_;
	p->xlink_type_ = xlink_type_;
	p->xlink_show_ = xlink_show_;
	p->xlink_actuate_ = xlink_actuate_;
	
	return p;
}

void
DrawImage::Init(ods::Tag *tag)
{
	tag->Copy(ns_->xlink(), ods::ns::kHref, xlink_href_);
	tag->Copy(ns_->xlink(), ods::ns::kType, xlink_type_);
	tag->Copy(ns_->xlink(), ods::ns::kShow, xlink_show_);
	tag->Copy(ns_->xlink(), ods::ns::kActuate, xlink_actuate_);
}

void
DrawImage::LoadImage(const QString &full_path, QSize &sz)
{
	QImageReader reader(full_path);
	sz = reader.size();
	
	QString *media_dir_path = book_->GetMediaDirPath();
	
	if (media_dir_path == nullptr)
	{
		mtl_warn("media_dir_path == nullptr");
		return;
	}
	
	auto info = QFileInfo(full_path);
	const int64_t max_size = 1024 * 1024 * 100; // 100MB
	
	if (info.size() > max_size)
	{
		mtl_warn("Too large");
		return;
	}
	
	const QString extension = QLatin1String(".") + info.suffix();
	const QString base_name = QLatin1String("image");
	QDir dir(*media_dir_path);
	int i = 0;
	QFile file;
	QString chosen_file_name;
	
	while (true)
	{
		QString file_name = base_name + QString::number(i++) + extension;
		
		file.setFileName(dir.filePath(file_name));
		
		if (!file.exists())
		{
			chosen_file_name = file_name;
			break;
		}
	}
	
	if (!QFile::copy(full_path, file.fileName()))
	{
		auto from = full_path.toLocal8Bit();
		auto to = file.fileName().toLocal8Bit();
		mtl_warn("Failed to copy from:\n%s\nTo:\n%s", from.data(), to.data());
		return;
	}
	
	auto *manifest = book_->manifest();
	xlink_href_ = ods::filename::MediaDirName;
	xlink_href_.append('/');
	xlink_href_.append(chosen_file_name);
	manifest->Add(xlink_href_);
	
	xlink_type_ = QLatin1String("simple");
	xlink_show_ = QLatin1String("embed");
	xlink_actuate_ = QLatin1String("onLoad");
}

void
DrawImage::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->xlink(), ods::ns::kHref, xlink_href_);
	Write(xml, ns_->xlink(), ods::ns::kType, xlink_type_);
	Write(xml, ns_->xlink(), ods::ns::kShow, xlink_show_);
	Write(xml, ns_->xlink(), ods::ns::kActuate, xlink_actuate_);
	
	WriteNodes(xml);
}

} // ods::inst::
} // ods::
