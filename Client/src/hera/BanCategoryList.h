#pragma once

#include "../common/Qt.h"
#include "../common/library.h"

struct BanFolderCacheFile
{
	QVector<QString> paths;
	friend QDataStream & operator<< (QDataStream &qos, const BanFolderCacheFile &sf)
	{
		qos << sf.paths;
		return qos;
	}
	friend QDataStream & operator>> (QDataStream &qis, BanFolderCacheFile &sf)
	{
		qis >> sf.paths;
		return qis;
	}
};

class LIANGSHIEXPORT BanCategoryList : public QTreeWidget
{
	Q_OBJECT
public:
	BanCategoryList(QWidget *parent = 0);
	~BanCategoryList();

private:
	virtual void keyPressEvent(QKeyEvent *e);

Q_SIGNALS:
	void OnFolderSelected(QString path);

private Q_SLOTS:
	void OnItemClickedTriggered(QTreeWidgetItem *item, int column);

private:
	void AddCustomMaterials();
	void AddInternalMaterials();
	void AddChildFolder(QTreeWidgetItem* parent, QString filePath);

	void LoadCacheFile();
	void SaveCacheFile();

private:
	QTreeWidgetItem* _custom;
	QTreeWidgetItem* _addCustom;
	QString _rootPath;
	BanFolderCacheFile _cacheFile;
};