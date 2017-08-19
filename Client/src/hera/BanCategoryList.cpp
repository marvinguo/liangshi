#include "BanCategoryList.h"
#include "../common/FileUtil.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

BanCategoryList::BanCategoryList(QWidget *parent):
QTreeWidget(parent),
_rootPath(qApp->applicationDirPath() + tr("/板书"))
{
	header()->close();
	setMinimumWidth(40);
	connect(this, &BanCategoryList::itemClicked, this, &BanCategoryList::OnItemClickedTriggered);

	AddInternalMaterials();
	_custom = new QTreeWidgetItem(this);
	_custom->setText(0, tr("自定义目录"));

	_addCustom = new QTreeWidgetItem(_custom);
	_addCustom->setText(0, tr("浏览"));
	AddCustomMaterials();
}

BanCategoryList::~BanCategoryList()
{
}

void BanCategoryList::keyPressEvent(QKeyEvent *e)
{
	if(e->key () == Qt::Key_Delete)
	{
		QTreeWidgetItem* item = currentItem();

		if(item->parent() == _custom)
		{
			QString filepath = (item->data(0, Qt::UserRole)).toString();

			for(QVector<QString>::iterator i = _cacheFile.paths.begin(); i != _cacheFile.paths.end(); ++i)
			{
				QString path = *i;
				if(filepath == path)
				{
					_cacheFile.paths.erase(i);
					break;
				}
			}
			SaveCacheFile();
			delete item;
		}
	}
}

void BanCategoryList::OnItemClickedTriggered(QTreeWidgetItem *item, int column)
{
	QString filepath = (item->data(0, Qt::UserRole)).toString();

	if(QFile::exists(filepath))
		Q_EMIT OnFolderSelected(filepath);

	if(item == _addCustom)
	{
		QString folder = QFileDialog::getExistingDirectory(this, tr("选择板书目录"));
		if(!folder.isEmpty())
		{
			QDir dir(folder);
			QTreeWidgetItem* item = new QTreeWidgetItem(_custom);
			item->setData(0, Qt::UserRole, QVariant(folder));
			item->setText(0, dir.dirName());
			AddChildFolder(item, folder);

			_cacheFile.paths.push_back(folder);
			SaveCacheFile();
		}
	}
}

void BanCategoryList::AddCustomMaterials()
{
	LoadCacheFile();
	foreach(QString folder, _cacheFile.paths)
	{
		QDir dir(folder);
		QTreeWidgetItem* item = new QTreeWidgetItem(_custom);
		item->setData(0, Qt::UserRole, QVariant(folder));
		item->setText(0, dir.dirName());
		AddChildFolder(item, folder);
	}
}

void BanCategoryList::AddInternalMaterials()
{
	QDir rootDir(_rootPath);
	QFileInfoList filesList = rootDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

	foreach(QFileInfo fileInfo, filesList)
	{
		if(fileInfo.isDir() && !FileUtil::IsDirDotTwo(fileInfo.fileName()))
		{
			QTreeWidgetItem* item = new QTreeWidgetItem(this);
			item->setData(0, Qt::UserRole, QVariant(fileInfo.absoluteFilePath()));
			item->setText(0, fileInfo.completeBaseName());
			AddChildFolder(item, fileInfo.filePath());
		}
	}
}

void BanCategoryList::AddChildFolder(QTreeWidgetItem* parent, QString filePath)
{
	QDir rootDir(filePath);
	QFileInfoList filesList = rootDir.entryInfoList();

	foreach(QFileInfo fileInfo, filesList)
	{
		if(fileInfo.isDir() && !FileUtil::IsDirDotTwo(fileInfo.fileName()))
		{
			QTreeWidgetItem* child = new QTreeWidgetItem(parent);
			child->setData(0, Qt::UserRole, QVariant(fileInfo.absoluteFilePath()));
			child->setText(0, fileInfo.completeBaseName());
			AddChildFolder(child, fileInfo.filePath());
		}
	}
}

void BanCategoryList::LoadCacheFile()
{
	QString cacheFile = _rootPath + "/.custom";
	if(QFile::exists(cacheFile))
	{
		QFile file(cacheFile);
		if(!file.open(QIODevice::ReadOnly))
			return;

		QDataStream in(&file);
		in.setVersion(QDataStream::Qt_4_0);
		in >> _cacheFile;
		file.close();
	}
}

void BanCategoryList::SaveCacheFile()
{
	QString cacheFile = _rootPath + "/.custom";
	QFile file2(cacheFile);
	if(!file2.open(QIODevice::WriteOnly))
		return;
	QDataStream out(&file2);
	out.setVersion(QDataStream::Qt_4_0);
	out << _cacheFile;
	file2.close();
}