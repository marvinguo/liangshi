#include "BanList.h"
#include "BanFile.h"
#include "../common/ImageUtil.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

BanList::BanList(QWidget *parent):
QListWidget(parent)
{
	setViewMode(QListView::IconMode);
	setResizeMode(QListView::Fixed);
	setIconSize(QSize(32, 32));
	setTextElideMode(Qt::ElideMiddle);
	setSpacing(12);

	connect(this, &BanList::itemDoubleClicked, this, &BanList::OnItemDoubleClickTriggered);
}

BanList::~BanList()
{

}

void BanList::OnFolderTriggered(QString path)
{
	clear();

	QDir rootDir(path);
	QFileInfoList filesList = rootDir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);

	foreach(QFileInfo fileInfo, filesList)
	{
		if(fileInfo.isFile())
		{
			QPixmap pixmap;
			//if(ScFile::LoadScThumbnailOnly(fileInfo.absoluteFilePath(), pixmap))
			if(BanFile::LoadScImageOnly(fileInfo.absoluteFilePath(), pixmap))
			{
				//QIcon icon(pixmap);
				QIcon icon(ImageUtil::ToFixsizeBackgroundPixmap(32, 32, pixmap));
				QListWidgetItem* item = new QListWidgetItem(icon, fileInfo.completeBaseName());
				item->setData(Qt::UserRole, QVariant(fileInfo.absoluteFilePath()));
				addItem(item);
			}
		}
	}
}

void BanList::OnItemDoubleClickTriggered(QListWidgetItem *item)
{
	QString filepath = (item->data(Qt::UserRole)).toString();

	if(QFile::exists(filepath))
	{
		Q_EMIT OnScFileSelect(filepath);
		QWidget* window = topLevelWidget();
		if(window != NULL)
			window->close();
	}
}