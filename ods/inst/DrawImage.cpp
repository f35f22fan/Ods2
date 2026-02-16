#include "DrawImage.hpp"

#include "../Book.hpp"
#include "../filename.hxx"
#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

#include "ManifestManifest.hpp"

#include <QImageReader>
#include <QMimeDatabase>

namespace ods::inst {

DrawImage::DrawImage(Abstract *parent, Tag *tag)
: Abstract (parent, parent->ns(), id::DrawImage)
{
	if (tag)
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

void DrawImage::Init(ods::Tag *tag)
{
	tag->Copy(ns_->xlink(), ns::kHref, xlink_href_);
	tag->Copy(ns_->xlink(), ns::kType, xlink_type_);
	tag->Copy(ns_->xlink(), ns::kShow, xlink_show_);
	tag->Copy(ns_->xlink(), ns::kActuate, xlink_actuate_);
}

void DrawImage::ListKeywords(Keywords &list, const LimitTo lt)
{
	inst::AddKeywords({tag_name(),
		ns::kHref, ns::kType, ns::kShow, ns::kActuate}, list);
}

void DrawImage::ListUsedNamespaces(NsHash &list)
{
	Add(ns_->draw(), list);
	Add(ns_->xlink(), list);
}

bool DrawImage::LoadImage(const QString &src_img_path, QSize &sz)
{
	QImageReader reader(src_img_path);
	sz = reader.size();
	
	QString *media_dir_path = book_->GetMediaDirPath();
	MTL_CHECK(media_dir_path);
	
	auto file_info = QFileInfo(src_img_path);
	ci64 max_img_len = 1024 * 1024 * 256; // 256MB
	MTL_CHECK(file_info.size() <= max_img_len);
	
	const QString ext = QLatin1String(".") + file_info.suffix();
	const QString base_name = QLatin1String("image");
	QDir dir(*media_dir_path);
	int i = 0;
	QFile dest_file;
	QString chosen_file_name;
	
	while (true)
	{
		QString file_name = base_name + QString::number(i++) + ext;
		dest_file.setFileName(dir.filePath(file_name));
		if (!dest_file.exists())
		{
			chosen_file_name = file_name;
			break;
		}
	}
	
	if (!QFile::copy(src_img_path, dest_file.fileName()))
	{
		auto from = src_img_path.toLocal8Bit();
		auto to = dest_file.fileName().toLocal8Bit();
		mtl_warn("Failed to copy from:\n%s\nTo:\n%s", from.data(), to.data());
		return false;
	}
	
	auto *manifest = book_->manifest();
	xlink_href_ = ods::filename::MediaDirName;
	xlink_href_.append('/');
	xlink_href_.append(chosen_file_name);
	manifest->AddEntry(xlink_href_, book_->GetMimeTypeForFile(src_img_path));
	
	xlink_type_ = QLatin1String("simple");
	xlink_show_ = QLatin1String("embed");
	xlink_actuate_ = QLatin1String("onLoad");
	
	return true;
}

void DrawImage::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->xlink(), ns::kHref, xlink_href_);
	Write(xml, ns_->xlink(), ns::kType, xlink_type_);
	Write(xml, ns_->xlink(), ns::kShow, xlink_show_);
	Write(xml, ns_->xlink(), ns::kActuate, xlink_actuate_);
	
	WriteNodes(xml);
}

} // ods::inst::
