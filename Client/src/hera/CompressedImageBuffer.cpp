#include "CompressedImageBuffer.h"
#include "../common/FileUtil.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

void CompressedImageBuffer::SetImage(QPixmap image)
{
	QString filename = QUuid::createUuid().toString()+ ".jpg";
	QString path = FileUtil::GetTempFolder() + filename;
	//网上说，85的质量，效果差不多，体积一半
	//	image.save(path, "JPG", 85);
	image.save(path, "JPG", 100);
	QFile file(path);
	file.open(QIODevice::ReadOnly);
	QByteArray array = file.readAll();
	_imageData = LokiBuffer(array.data(), array.size());
	file.close();
	QFile::remove(path);
}

QPixmap CompressedImageBuffer::GetImage()
{
	QString filename = QUuid::createUuid().toString()+ ".jpg";
	QString path = FileUtil::GetTempFolder() + filename;

	QFile file(path);
	file.open(QIODevice::WriteOnly);
	file.write(_imageData.Data(), _imageData.Size());
	file.flush();
	file.close();

	QPixmap image;
	image.load(path);
	QFile::remove(path);
	return image;
}
