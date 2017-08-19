#pragma once

#include "../common/Qt.h"
#include "../common/QtSingleton.h"
#include "BanCategoryList.h"
#include "BanList.h"

class LIANGSHIEXPORT BanBrowserDialog : public QDialog, public QtSingleton<BanBrowserDialog>
{
	Q_OBJECT
public:
	BanBrowserDialog();
	virtual ~BanBrowserDialog();
	void ShowModal();

Q_SIGNALS:
	void OnScFileSelected(QString filepath);

private Q_SLOTS:
	void OnFolderTriggered(QString path);

private:
	QString _lastFolder;
	BanList* _banList;
};

