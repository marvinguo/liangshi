#pragma once

#include "../common/Qt.h"
#include "../common/library.h"

class LIANGSHIEXPORT BanList : public QListWidget
{
	Q_OBJECT
public:
	BanList(QWidget *parent = 0);
	~BanList();

Q_SIGNALS:
	void OnScFileSelect(QString filepath);

public Q_SLOTS:
	void OnFolderTriggered(QString path);

private Q_SLOTS:
	void OnItemDoubleClickTriggered(QListWidgetItem *item);
};
